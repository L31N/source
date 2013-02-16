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

#ifdef _WIN32
#define IS_PATHSEP(c) ((c) == '/' || (c) == '\\')
#define PATHSEP "\\"
#define DIRSEP ";"
#define DRIVESEP ":"
#define DOS_PATHS 1
#define IS_DRIVESEP(c) ((c) == ':')
#define QUOTE_CHAR '\"'
#else
#define IS_PATHSEP(c) ((c) == '/')
#define PATHSEP "/"
#define DIRSEP ":"
#define DRIVESEP
#define DOS_PATHS 0
#define IS_DRIVESEP(c) 0
#define QUOTE_CHAR '\\'
#endif

#define IS_SPC(c) ((unsigned char)(c) <= 32)
#define dolist(n,p) for (n=p;n;n=n->next)

typedef struct list_node {
    struct list_node *next;
    void *v;
} list_node;

typedef struct string_node {
    struct string_node *next;
    char str[1];
} string_node;

struct string_node *new_string_node_c(const char *s, int l)
{
    struct string_node *b = (struct string_node *)malloc(sizeof *b + l);
    memcpy(b->str, s, l);
    b->str[l] = 0;
    b->next = NULL;
    return b;
}

struct string_node *new_string_node(const char *s)
{
    return new_string_node_c(s, strlen(s));
}

void reverse_list (void *d)
{
    list_node *a, *b, *c;
    a = *(list_node**)d;
    for (b=NULL; a; c=a->next, a->next=b, b=a, a=c);
    *(list_node**)d = b;
}

void cons_node (void *a0, void *e0)
{
    list_node **pp = (list_node**)a0, *e = (list_node*)e0;
    e->next = *pp, *pp = e;
}

void append_node (void *a0, void *e0)
{
    list_node *l, **pp = (list_node**)a0, *e = (list_node*)e0;
    for ( ;NULL != (l = *pp) ; pp = &l->next);
    *pp = e, e->next=NULL;
}

void freeall(void *p)
{
    list_node *s, *q;
    q = *(list_node**)p;
    while (q) q = (s = q)->next, free(s);
    *(list_node**)p = q;
}

char *file_basename(const char *path)
{
    const char *sa, *se;
    for (se = strchr(sa = path, 0); se > sa; --se)
        if (IS_PATHSEP(se[-1]) || IS_DRIVESEP(se[-1]))
            break;
    return (char*)se;
}

char *file_extension(const char *path)
{
    char *sa, *se, *sf;
    sa = file_basename(path);
    se = sf = strchr(sa, 0);
    while (--se > sa)
        if (*se == '.')
            return se;
    return sf;
}

