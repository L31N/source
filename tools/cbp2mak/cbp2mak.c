/*
  cbp2mak - Copyright © 2008 grischka

  cbp2mak is free software, released under the GNU General Public License
  (GPL version 2). For details see:

  http://www.fsf.org/licenses/gpl.html

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#include <process.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#endif

char *argv_0;

#include "txml.h"
#include "list.c"

#define s_node string_node
#define l_str string_node
#define n_str string_node
#define to_int(s) atoi(s)
const char * to_str(const char *s);
void to_s_node(string_node **pp, const char *s);
void to_l_str(string_node **pp, const char *s);
void to_n_str(string_node **, const char *s);

#include "cbp.h"
#include "cbp.c"
#include "cbw.h"
#include "cbw.c"

#define APPNAME "cbp2mak"

// ----------------------------------------------------------------------
#ifndef _WIN32
int spawnlp(int wait, const char *cmd, ...)
{
    char **argv;
    int i, argc;
    int pid, code;
    va_list vl;

    argc = 0;

    va_start(vl, cmd);
    for (argc = 0; va_arg(vl, const char *); ++argc);
    va_end(vl);

    argv = (char **)malloc((argc + 1) * sizeof *argv);

    va_start(vl, cmd);
    for (i = 0; i < argc; ++i)
        argv[i] = strdup(va_arg(vl, const char *));
    argv[i] = NULL;
    va_end(vl);

    if (0 == (pid = fork())) {
        execvp(cmd, argv);
        fprintf(stderr, "%s: %s", cmd, strerror(errno));
        exit(-1);
    }

    for (i = 0; i < argc; free(argv[i++]));
    free(argv);

    if (pid < 0 || !wait)
        return pid;

    code = -1;
    waitpid(pid, &code, 0);
    return code;
}
#define P_WAIT 1
#else
char *realpath(const char *in, char *buf)
{
    int __stdcall GetFullPathNameA(const char*, int, char *, char **);
    if (NULL == buf)
        buf = malloc(1000);
    GetFullPathNameA(in, 1000, buf, NULL);
    return buf;
}
#endif


// ----------------------------------------------------------------------

int file_error (const char *path)
{
    fprintf(stderr, APPNAME ": %s : %s\n", path, strerror(errno));
    return 1;
}

// ----------------------------------------------------------------------

char *cvt_name(char *buf)
{
    char *d, *s;
    int c;
    s = d = buf;
    if (buf) do {
        c = *s++;
        if (c == ' ' || c == '#')
            c = '_';
        else if (c == '&')
            c = 'n';
        *d++ = c;
    } while (c);
    return buf;
}

char *cvt_value(char *buf)
{
    char *d, *s;
    int c;
    s = d = buf;
    if (buf) do {
        c = *s++;
        if (c == '#') {
            c = '_';
        } else if (c == '$' && s[0] == '(' && s[1] == '#') {
            do {
                if (c == '#')
                    c = '_';
                else if (c >= 'a' && c <= 'z')
                    c -= 32;
                *d++ = c;
                c = *s++;
            } while (c && c != ')');
        }
        *d++ = c;
    } while (c);
    return buf;
}

const char *to_str(const char *s)
{
    if (!strchr(s, '#'))
        return s;
    return cvt_value(strdup(s));
}

const char *to_fslash(char *tmp, const char *str)
{
    char *p = tmp;
    int c;
    do {
        c = *str++;
        *p++ = c == '\\' ? '/' : c;
    } while (c);
    return tmp;
}


const char *to_lwr(char *tmp, const char *str)
{
    char *p = tmp;
    int c;
    do {
        c = *str++;
        *p++ = c >= 'A' && c <= 'Z' ? c + 32 : c;
    } while (c);
    return tmp;
}

void to_s_node(string_node **pp, const char *s)
{
    string_node *sn;
    append_node(pp, sn = new_string_node(s));
    cvt_value(sn->str);
}

void to_n_str(string_node **pp, const char *s)
{
    string_node *sn;
    append_node(pp, sn = new_string_node(s));
    cvt_name(sn->str);
}

void to_l_str(string_node **pp, const char *s)
{
    const char *a, *b;
    string_node *sn = NULL;
    while (*s) {
        b = strchr(a = s, ';');
        if (b)
            s = b + 1;
        else
            b = s = strchr(s, 0);
        cons_node(&sn, new_string_node_c(a, b - a));
        cvt_name(sn->str);
    }
    reverse_list (&sn);
    *pp = sn;
}

struct ofile {
    struct ofile *next;
    struct unit *u;
};

struct context {
    struct cbpfile *cbp;
    struct cbwfile *cbw;
    struct project *p;
    struct target *t;
    struct project_path *pp;
    struct ofile *o;
    struct unit *u;
    FILE *ip;
    FILE *fp;

    int skip;
    int msc;
    int level;
    const char *mak0, *mak;
};

int spawn_cbp2mak(struct context *cx, const char *cbp)
{
    char tmp[200];
    const char *b = file_basename(cbp);
    int n = b - cbp - 1;
    if (n < 0)
        cbp = strcpy(tmp, ".");
    else
        cbp = strcpy(tmp, cbp), tmp[n] = 0;
    fflush(stdout);
    fflush(stderr);
    return spawnlp(P_WAIT, argv_0, argv_0, cx->msc?"-nmake":"-gmake","-C", tmp, b, cx->mak0, NULL);
}

// ----------------------------------------------------------------------
// ---
// ----------------------------------------------------------------------

int do_template(struct context *cx);

int do_expansion(struct context *cx, const char *var, const char *line_start)
{
    char tmp[200];
    struct project *p = cx->p;
    struct target *t = cx->t;
    const char *s = NULL;
    FILE *fp = cx->fp;
    struct ofile *o;
    string_node *sn;

    if (0 == strcmp(var, "self")) {
        fprintf(fp, "%s", argv_0);

    } else if (0 == strcmp(var, "cbp_path")
            || 0 == strcmp(var, "cbp_file")
            || 0 == strcmp(var, "cbp_mak") ) {

        if (NULL == cx->pp) {
            if (var[4] != 'm')
                return 1;
            fprintf(fp, "%s", cx->mak);

        } else {
            char *b;
            strcpy(tmp, cx->pp->filename);
            b = file_basename(tmp);
            if (b > tmp)
                b[-1] = 0;
            if (var[4] == 'm') {
                if (cx->mak0)
                    fprintf(fp, "%s", cx->mak0);
                else
                    fprintf(fp, "%s.mak", b);
            } else if (var[4] == 'p') {
                fprintf(fp, "%s", tmp[0] ? tmp : ".");
            } else {
                fprintf(fp, "%s", b);
            }
        }

    } else if (cx->cbw) {
        if (0 == strcmp(var, "W"))
            fprintf(fp, "%s", cx->cbw->workspace->title);
        else
            return 1;

    } else if (!p) {
        return 1;

    } else if (0 == strcmp(var, "P")) {
        fprintf(fp, "%s", p->title->str);

    } else if (0 == strcmp(var, "custom_make")) {
        if (p->makefile_is_custom) {
            fprintf(cx->fp, "all :\n\n%% :\n\t$(MAKE) $@");
            if (p->makefile)
                fprintf(cx->fp, " -f %s", p->makefile);
            fprintf(cx->fp, "\n");
            ++cx->skip;
            if (do_template(cx))
                return 1;
            --cx->skip;
        }

    } else if (0 == strcmp(var, "targets")) {
        dolist (t, p->build->target)
            if (t->title)
                //fprintf(fp, "%s_target ", t->title->str);
                fprintf(fp, "%s", t->title->str);


    } else if (0 == strcmp(var, "cflags")) {
        struct cc_option *c;
        c = t ? t->cc_option : p->cc_option;
        if (!c) return 0;
        dolist (sn, c->option)
            fprintf(fp, "%s ", sn->str);

    } else if (0 == strcmp(var, "includes")) {
        struct cc_option *c;
        c = t ? t->cc_option : p->cc_option;
        if (!c) return 0;
        dolist (sn, c->directory)
            fprintf(fp, "-I%s ", sn->str);

    } else if (0 == strcmp(var, "rcflags")) {
        struct rc_option *c;
        c = t ? t->rc_option : p->rc_option;
        if (!c) return 0;
        dolist (sn, c->option)
            fprintf(fp, "%s ", sn->str);
        dolist (sn, c->directory)
            fprintf(fp, "-I%s ", sn->str);

    } else if (0 == strcmp(var, "ldflags")) {
        struct ld_option *c;
        c = t ? t->ld_option : p->ld_option;
        if (!c) return 0;
        if (!cx->msc)
            dolist (sn, c->option)
                fprintf(fp, "%s ", sn->str);
        dolist (sn, c->directory)
            if (cx->msc)
                fprintf(fp, "/LIBPATH:%s", sn->str);
            else
                fprintf(fp, "-L%s ", sn->str);

    } else if (0 == strcmp(var, "ldlibs")) {
        struct ld_option *c;
        c = t ? t->ld_option : p->ld_option;
        if (!c) return 0;
        dolist (sn, c->library) {
            s = sn->str;
            if (file_basename(s) != s)
                fprintf(fp, "%s ", s);
            else if (0 == strncmp(s, "lib", 3) && 0 == strcmp(file_extension(s), ".a"))
                fprintf(fp, "-l%.*s ", file_extension(s) - s - 3, s + 3);
            else if (cx->msc)
                fprintf(fp, "%s.lib ", s);
            else
                fprintf(fp, "-l%s ", s);
        }

    } else if (0 == strncmp(var, "before", 6) || 0 == strncmp(var, "after", 5)) {
        struct extra *e;
        const char *nl = NULL;
        const char *a = strstr(var, "always");
        dolist (e, t ? t->extra : p->extra) {
            string_node *sn0;
            if (var[0] == 'b') {
                sn0 = e->before;
                if (e->mode)
                    s = e->mode->before;
            } else {
                sn0 = e->after;
                if (e->mode)
                    s = e->mode->after;
            }
            dolist (sn, sn0)
                if ((NULL != a) == (t && s && 0 == strcmp(s, "always"))) {
                    if (nl)
                        fprintf(fp, "%s%s", nl, line_start);
                    fprintf(fp, "%s", sn->str);
                    nl = "\n";
                }
        }

    } else if (0 == strcmp(var, "environment")) {
        struct environment *e;
        dolist (e, p->build->environment)
            fprintf(fp, "%s = %s\n", e->name, e->value);

    } else if (0 == strcmp(var, "vtargets")) {
        struct v_target *v;
        int all = 0;
        char *a, *b;
        dolist (v, p->v_target) {
            if (! v->alias || ! v->targets)
                continue;
            to_lwr(tmp, v->alias->str);
            a = b = tmp;
            do
                if ((*a = *b) != '_')
                    a++;
            while (*b++);
            a = tmp;

            all += 0 == strcmp(a, "all");
            fprintf(fp, "%s : all.before", a);
            dolist (sn, v->targets)
                //fprintf(fp, " %s_target", sn->str);
                fprintf(fp, " %s", sn->str);
            fprintf(fp, " all.after\n");
        }
        if (!all)
            fprintf(fp, "all: all.before all.targets all.after\n");

    } else if (!t) {
        return 1;

    } else if (0 == strcmp(var, "T")) {
        //fprintf(fp, "%s_target", t->title->str);
        fprintf(fp, "%s", t->title->str);

    } else if (0 == strcmp(var, "output")) {
        fprintf(fp, "%s", t->output);

    } else if (0 == strcmp(var, "ofiles")) {
        dolist (o, cx->o)
            fprintf(fp,  "%s ", o->u->objname);

    } else if (0 == strcmp(var, "dfiles")) {
        dolist (o, cx->o)
            if (o->u->depname)
                fprintf(fp,  "%s ", o->u->depname);

    } else if (0 == strcmp(var, "working_dir")) {
        s = t->working_dir;
        if (s)
            fprintf(fp, "%s", s);

    } else if (0 == strcmp(var, "external_deps")) {
        s = t->external_deps;
        if (s) {
            char *a, *b, *c;
            a = b = c = strdup(s);
            while (b && *b) {
                c = strchr(b, ';');
                if (c)
                    *c++ = 0;
                fprintf(fp, "%s", b);
                b = c;
            }
            free(a);
        }

    } else if (0 == strcmp(var, "create_def")) {
        if (t->createDefFile)
            fprintf(fp, "$(CREATE_DEF)");

    } else if (0 == strcmp(var, "create_lib")) {
        if (t->createStaticLib)
            fprintf(fp, "$(CREATE_LIB)");

    } else if (0 == strcmp(var, "link")) {
        const char *types[4] = { "exe", "con", "lib", "dll" };
        if (t->type < 4)
            fprintf(fp, "%s", types[t->type]);

    } else if (0 == strcmp(var, "command")) {
        if (t->build_cmd)
            fprintf(fp,  "%s ", t->build_cmd->option->str);

    } else if (!cx->u) {
        return 1;

    } else if (0 == strcmp(var, "src")) {
        fprintf(fp, "%s", cx->u->filename);

    } else if (0 == strcmp(var, "obj")) {
        fprintf(fp, "%s", cx->u->objname);

    } else if (0 == strcmp(var, "comp")) {
        s = file_extension(cx->u->filename);
        fprintf(fp, "%s", s + !!s[0]);

    } else {
        return 1;
    }

    return 0;
}

// ----------------------------------------------------------------------
// ---
// ----------------------------------------------------------------------

int do_section(struct context *cx, const char *var, long pos)
{
    char tmp[1000];
    struct project *p = cx->p;
    struct target *t = cx->t;
    struct unit *u;
    struct project_path *pp;
    struct ofile *o;
    int c = 0;

    if (0 == strcmp(var, "workspace")) {
        if (cx->cbw) {
            dolist (pp, cx->cbw->workspace->project_path) {
                if (spawn_cbp2mak(cx, pp->filename)) {
                    pp->filename = NULL;
                    //return 1;
                }
            }
            return do_template(cx);
        } else {
            goto skip;
        }

    } else if (cx->cbw) {
        if (0 == strcmp(var, "projects")) {
            if (cx->level < 2)
                goto skip;
            dolist (pp, cx->cbw->workspace->project_path) {
                if (NULL == pp->filename)
                    continue;
                cx->pp = pp;
                fseek(cx->ip, pos, SEEK_SET);
                if (do_template(cx))
                    return 1;
                cx->pp = NULL;
                ++c;
            }
        } else {
            return 1;
        }

    } else if (!cx->cbp) {
        return 1;

    } else if (0 == strcmp(var, "projects")) {

        dolist (p, cx->cbp->project) {
            if (NULL == p->title)
                continue;

            dolist (u, p->unit) {
                const char *f, *e;
                int l;
                f = u->filename;
                e = file_extension(f);
                if (0 == e[0])
                    continue;
                l = e - f;
                strcat(strcpy(tmp, f) + l, ".");
                if (NULL == strstr(".c.cpp.cxx.rc.", tmp+l))
                    continue;
                f = cx->msc ? (e[1]=='r'?".res":".obj") : ".o";
                strcpy(tmp+l, f), u->objname = strdup(tmp);
                if (e[1] == 'c')
                    strcpy(tmp+l, ".d"), u->depname = strdup(tmp);
            }

            dolist (t, p->build->target) {
                if (NULL == t->title)
                    continue;
                if (t->extension_auto && t->type < 4) {
#ifdef _WIN32
                    const char *types[4] = { "exe", "exe", "a", "dll" };
#else
                    const char *types[4] = { "exe", "exe", "a", "so" };
#endif
                    if (strcmp(types[t->type], "exe")) sprintf(tmp, "%s.%s", t->output, types[t->type]);
                    else sprintf(tmp, "%s", t->output);
                    t->output = strdup(tmp);
                }
            }

            dolist (t, p->build->target) {
                struct target *t2;
                if (NULL == t->title)
                    continue;
                dolist (t2, t->next)
                    if (0 == strcmp(t->output, t2->output))
                        t2->title = NULL;
            }

            cx->p = p;
            fseek(cx->ip, pos, SEEK_SET);
            if (do_template(cx))
                return 1;
            cx->p = NULL;
            ++c;
        }

    } else if (!p) {
        return 1;

    } else if (0 == strcmp(var, "targets")) {
        dolist (t, p->build->target) {
            struct ofile **oo = &cx->o;
            if (NULL == t->title)
                continue;

            dolist (u, p->unit) {
                string_node *sn;
                int x = -1;
                if (0 == u->compile || NULL == u->objname)
                    continue;
                dolist(sn, u->target) {
                    if (0 == strcmp(sn->str, t->title->str))
                        x = 1;
                    else if (x < 0)
                        x = 0;
                }
                if (0 == x)
                    continue;
                o = *oo = c_new(struct ofile);
                oo = &o->next;
                o->u = u;
            }

            cx->t = t;
            fseek(cx->ip, pos, SEEK_SET);
            if (do_template(cx))
                return 1;
            cx->t = NULL;
            freeall(&cx->o);
            ++c;
        }

    } else if (0 == strcmp(var, "units")) {
        dolist (o, cx->o) {
            cx->u = o->u;
            fseek(cx->ip, pos, SEEK_SET);
            if (do_template(cx))
                return 1;
            cx->u = NULL;
            ++c;
        }
    } else {
        return 1;
    }

    if (c)
        return 0;
skip:
    ++cx->skip;
    if (do_template(cx))
        return 1;
    --cx->skip;
    return 0;
}

// ----------------------------------------------------------------------
// ---
// ----------------------------------------------------------------------

int scan_var(char **pp, char *var, int f)
{
    char *d = var, *p = *pp;
    int c;
    while (c = *p++, c) {
        if (f ? IS_SPC(c) : c == ']')
            break;
        if (d - var >= 31)
            return 1;
        *d++ = c;
    }
    *d = 0;
    *pp = p;
    return 0;
}

int do_template(struct context *cx)
{
    char line[1000], var[32], *p;
    int c;

    ++cx->level;
    for (;;) {
        if (!fgets(line, sizeof line, cx->ip)) {
            --cx->level;
            return 0;
        }
        p = strchr(line, 0);
        while (p > line && IS_SPC(p[-1]))
            --p;
        *p = 0;
        //printf("line %s\n", line);

        p = line;
        c = *p++;

        if (c == ']' && *p == ']') {
            if (--cx->level < 1)
                goto err;
            return 0;

        } else if (c == '[' && *p == '[') {
            ++p;
            if (scan_var(&p, var, 1))
                goto err;
            if (cx->skip ? do_template(cx) : do_section(cx, var, ftell(cx->ip)))
                goto err;

        } else if (0 == cx->skip) {
            while (c) {
                if (c == '[') {
                    p[-1] = 0;
                    if (scan_var(&p, var, 0))
                        goto err;
                    if (do_expansion(cx, var, line))
                        goto err;
                } else {
                    fputc(c, cx->fp);
                }
                c = *p++;
            }
            fputc('\n', cx->fp);
        }
    }
err:
    fprintf(stderr, APPNAME": error at '%s'\n", line);
    return 1;
}

// ----------------------------------------------------------------------
// ---
// ----------------------------------------------------------------------

int cbp2mak(const char *path, const char *mak, int msc)
{
    struct context scx, *cx = &scx;
    TiXmlNode *c;
    char tmp_inc[1000];
    char tmp_mak[1000];
    const char *inc;
    int ret;
    char *err;

    c = load_doc(path, &err);
    if (err)
        fprintf(stderr, APPNAME": %s\n", err);
    if (!c)
        return 1;

    memset(cx, 0, sizeof *cx);
    cx->msc = msc;
    cx->mak0 = mak;
    if (!mak)
        mak = strcat(strcpy(tmp_mak, path), ".mak");
    cx->mak = mak;

    cx->fp = strcmp(mak, "-") ? fopen(mak, "w") : stdout;
    if (!cx->fp)
        return file_error(mak);

    printf("-> %s\n", mak);

#ifdef _WIN32
    if (cx->msc)
        inc = "cbp2mak.msc.mak";
    else
        inc = "cbp2mak.mingw.mak";
#else
    inc = "cbp2mak.unix.mak";
#endif

    strcpy(file_basename(strcpy(tmp_inc, argv_0)), inc);
    cx->ip = fopen (tmp_inc, "rb");
    if (!cx->ip)
        return file_error(tmp_inc);

    cx->cbw = read_cbwfile(c);
    cx->cbp = read_cbpfile(c);
    ret = do_template(cx);
    free_doc(c);
    return 0;
}


// ----------------------------------------------------------------------
// ---
// ----------------------------------------------------------------------

int main (int argc, char **argv)
{
    int i, msc = 0;
    const char *file = NULL;
    const char *mak = NULL;
    char *a;

    if (argc < 2) {
usage:
        fprintf(stderr,
            APPNAME" - Copyright 2008 grischka@users.sourceforge.net\n"
            "Converts codeblocks workspace/project files to makefiles\n"
            "Usage: "APPNAME" [-C directory] [-nmake] infile.cbp/.workspace [outfile]\n"
            );
        return 1;
    }

    a = realpath(argv[0], NULL);
    if (!a)
        a = argv[0];
    argv_0 = a;

    for (i = 1; i < argc; ++i)
    {
        a = argv[i];
        if (0 == strcmp(a, "-C")) {
            if (++i == argc)
                goto usage ;
            a = argv[i];
            if (chdir(a) < 0)
                return file_error(a);
        } else if (0 == strcmp(a, "-nmake"))
            msc = 1;
        else if (0 == strcmp(a, "-gmake"))
            msc = 0;
        else if (NULL == file)
            file = a;
        else if (NULL == mak)
            mak = a;
        else
            goto usage;
    }

    if (!file)
        goto usage;
    return cbp2mak(file, mak, msc);
}

// ----------------------------------------------------------------------
