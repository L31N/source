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

#include "list.c"
#include "txml.h"

#define APPNAME "xml2c"

FILE *fp = NULL;

struct tx {
    struct tx *next;
    TiXmlNode *n;
    const char *name;
    int init;
};

struct ix {
    struct ix *next;
    const char *v;
    const char *k;
    int s;
};

int file_error (const char *path)
{
    fprintf(stderr, APPNAME ": %s : %s\n", path, strerror(errno));
    return 1;
}

void pr(int n, const char *fmt, ...)
{
    va_list v;
    va_start(v, fmt);
    if (*fmt) {
        while (n)
            fprintf(fp, "  "), --n;
        vfprintf(fp, fmt, v);
    }
    fprintf(fp, "\n");
}

int add_stack(TiXmlNode *n, struct tx **pp)
{
     struct tx *p;

     const char *name = a_value(a_first(n));
     for (p = *pp; p; p = p->next)
         if (0 == strcmp(p->name, name))
             return 0;

     p = c_new(struct tx);
     p->n = n;
     p->name = name;
     p->next = *pp;
     *pp = p;

     return 1;
}

int is_node(TiXmlNode *n)
{
    TiXmlAttribute* a = a_first(n);
    return a && 0 == strcmp(a_name(a), "node");
}

const char* get_s_name(TiXmlNode *n, TiXmlAttribute **pa)
{
    int f = is_node(n);
    const char *s = f ? a_value(a_first(n)) : value(n);
    if (pa) {
        TiXmlAttribute* a = a_first(n);
        *pa = f ? a_next(a) : a;
    }
    return s;
}

void make_stack(TiXmlNode *n, struct tx **pp)
{
    TiXmlNode *c;

    if (is_node(n))
        add_stack(n, pp);
    loop_childs(c, n)
        make_stack(c, pp);
}

// ----------------------------------------------------------------------

void print_func_1 (TiXmlNode *n, int t, struct tx **pp)
{
    TiXmlAttribute* a, *b;
    TiXmlNode *c;
    const char *s_name;
    const char *f;

    s_name = get_s_name(n, &a);
    c = child(n);

    if (a)
        pr(t, "TiXmlAttribute* a;");
    if (c)
        pr(t, "TiXmlNode *c;");

    if (a) {
        pr(t,"");
        pr(t+0, "loop_attr(a, n) {");
        pr(t+1, "const char *v = a_value(a), *k = a_name(a);");
        f = "";
        b = a; do {
            char tmp[40];
            const char *val = a_value(b);
            const char *key = a_name(b);
            const char *e = strchr(val, ',');
            if (e) {
                strcpy(tmp, val)[e-val] = 0;
                val = tmp;
            }
            pr(t+1, "%sif (0 == strcmp(k, \"%s\"))", f, key);
            if (0 == strcmp(val, "int")) {
                pr(t+2, "p->%s = to_int(v);", key);
            } else if (0 == strcmp(val, "str")) {
                pr(t+2, "p->%s = to_str(v);", key);
            } else {
                pr(t+2, "to_%s(&p->%s, v);", val, key);
            }
            f = "else ";
        } while (b = a_next(b), b);
        pr(t+0, "}");
    }

    if (c) {

        pr(t,"");
        pr(t++, "loop_childs(c, n) {");
        pr(t, "const char *v = value(c);");

        f = "";
        loop_childs(c, n) {
            pr(t,"");
            pr(t++, "%sif (0 == strcmp(v, \"%s\")) {", f, value(c));
            if (is_node(c)) {
                s_name = a_value(a_first(c));
                pr(t, "cons_node(&p->%s, read_%s(c));", s_name, s_name);
                add_stack(c, pp);
            } else {
                pr(t, "TiXmlNode *n = c;");
                print_func_1(c, t, pp);
            }
            f = "} else ";
            --t;
        }
        pr(t, "}");
        pr(--t, "}");
    }
}

void make_init(TiXmlNode *n, struct ix **pp)
{
    TiXmlAttribute* a;
    TiXmlNode *c;
    const char *e;

    loop_attr(a, n) {
        const char *val = a_value(a);
        const char *key = a_name(a);
        e = strchr(val, ',');
        if (e) {
            struct ix *i = c_new(struct ix);
            cons_node(pp, i);
            i->v = e+1;
            i->k = key;
            i->s = 0 == strncmp(val, "str,", 4);
        }
    }
    loop_childs(c, n)
        if (!is_node(c))
            make_init(c, pp);
}


void print_func (TiXmlNode *n, int f)
{
    const char *s_name;
    TiXmlAttribute* a;
    TiXmlNode *c;
    int t = 0;

    struct ix *i, *i0 = NULL;
    struct tx *p, *p0 = NULL;

    s_name = get_s_name(n, &a);
    pr(t++, "struct %s *read_%s (TiXmlNode *n)\n{", s_name, s_name);
    if (f) {
        pr(t, "struct %s *p;", s_name);
        pr(0, "");
        pr(t, "n = get(n, \"%s\");", value(n));
        pr(t, "if (!n)");
        pr(t+1, "return NULL;");
        pr(0, "");
        pr(t, "p = c_new(struct %s);", s_name);
    } else {
        pr(t, "struct %s *p = c_new(struct %s);", s_name, s_name);
    }

    make_init(n, &i0);
    dolist (i, i0)
        if (i->s)
            pr(t,"p->%s = \"%s\";", i->k, i->v);
        else
            pr(t,"p->%s = %s;", i->k, i->v);
    freeall(&i0);

    pr(t++, "{");
    print_func_1(n, t, &p0);
    pr(--t, "}");

    if (p0) {
        pr(t,"");
        reverse_list(&p0);
        for (p = p0; p; p = p->next) {
            c = p->n;
            a = a_first(c);
            pr(t, "reverse_list(&p->%s);", a_value(a));
        }
        freeall(&p0);
    }

    pr(t, "return p;");
    pr(--t, "}");
    pr(t, "");
}

// ----------------------------------------------------------------------

void print_struct_1 (TiXmlNode *n, const char *s_name)
{
    TiXmlAttribute* a;
    TiXmlNode *c;

    loop_attr(a, n) {
        char tmp[40];
        const char *val = a_value(a);
        const char *key = a_name(a);
        const char *e = strchr(val, ',');
        if (e) {
            strcpy(tmp, val)[e - val] = 0;
            val = tmp;
        }

        if (0 == strcmp(key, "node"))
            pr(1, "struct %s *next;", s_name);

        else if (0 == strcmp(val, "str"))
            pr(1, "const char *%s;", key);

        else if (0 == strcmp(val, "int"))
            pr(1, "int %s;", key);
        else
            pr(1, "struct %s *%s;", val, key);
    }

    loop_childs(c, n) {
        if (is_node(c)) {
            const char *s_name = a_value(a_first(c));
            pr(1, "struct %s *%s;", s_name, s_name);
        } else {
            print_struct_1(c, NULL);
        }
    }
}

void print_struct (TiXmlNode *n, int f)
{
    TiXmlNode *c;
    const char *s_name;

    struct tx *p, *p0 = NULL;
    make_stack(n, &p0);
    for (p = p0; p; p = p->next) {
        c = p->n;
        if (f) {
            print_func(c, c == n);
        } else {
            s_name = get_s_name(c, NULL);
            pr(0, "struct %s\n{", s_name);
            print_struct_1(c, s_name);
            pr(0, "};");
            pr(0, "");
        }
    }
    while (p0)
        p = p0, p0 = p->next, free(p);
}


// ----------------------------------------------------------------------
// ---
// ----------------------------------------------------------------------

int xml2c(const char *in, const char *out1, const char *out2)
{
    TiXmlNode *c, *d;
    char *err;

    d = load_doc(in, &err);
    if (err)
        fprintf(stderr, APPNAME": %s\n", err);
    if (!d)
        return 1;

    c = child(d);

    fp = stdout;
    if (out1 && strcmp(out1, "-")) {
        fp = fopen(out1, "w");
        if (!fp)
            return file_error(out1);
    }
    print_struct(c, 0);

    if (!fp || (out2 && strcmp(out1, out2))) {
        fp = fopen(out2, "w");
        if (!fp)
            return file_error(out2);
    }
    print_struct(c, 1);

    free_doc(d);
    return 0;
}

// ----------------------------------------------------------------------
// ---
// ----------------------------------------------------------------------

int main (int argc, char **argv)
{
    const char *in = NULL, *out1 = NULL, *out2 = NULL;
    int i;
    for (i = 1; i < argc; ++i)
    {
        char *a = argv[i];
        if (!in) {
            in = a;
        } else if (!out1) {
            out1 = a;
        } else if (!out2) {
            out2 = a;
        } else {
            return 1;
        }
    }

    if (!in)
        return 1;

    return xml2c(in, out1, out2);
}

// ----------------------------------------------------------------------
// ---
// ----------------------------------------------------------------------
