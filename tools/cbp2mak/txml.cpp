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
#include "tinyxml.h"

extern "C" {
// ----------------------------------------------------------------------

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#endif

#include "txml.h"

TiXmlNode* load_doc(const char *path, char **err)
{
    TiXmlDocument doc(path);
    *err = NULL;
    if (!doc.LoadFile()) {
        char tmp[1000];
        if (doc.ErrorId() == TiXmlDocument::TIXML_ERROR_OPENING_FILE)
            sprintf(tmp, "%s: %s", path, doc.ErrorDesc());
        else
            sprintf(tmp, "%s:%d:%d: %s", path, doc.ErrorRow(), doc.ErrorCol(), doc.ErrorDesc());
        *err = strdup(tmp);
        if (doc.ErrorId() == TiXmlDocument::TIXML_ERROR_OPENING_FILE)
            return NULL;
    }
    return new TiXmlDocument(doc);
}

void free_doc(TiXmlNode* c)
{
    delete (TiXmlDocument*)c;
}

TiXmlNode *child(TiXmlNode *c)
{
    if (c) {
        c = c->FirstChild();
        while (c && c->Type() != TiXmlNode::ELEMENT)
            c = c->NextSibling();
    }
    return c;
}

TiXmlNode *next(TiXmlNode *c)
{
    if (c)
        do {
            c = c->NextSibling();
        } while (c && c->Type() != TiXmlNode::ELEMENT);
    return c;
}

TiXmlNode* get(TiXmlNode *n, const char *v)
{
    TiXmlNode *c;
    loop_childs(c, n)
        if (0 == strcmp(c->Value(), v))
            break;
    return c;
}

const char *value(TiXmlNode* a) {
    return a->Value();
}

const char *a_get(TiXmlNode* n, const char *name)
{
    TiXmlAttribute* a;
    if (n)
        loop_attr(a, n)
            if (0 == strcmp(a->Name(), name))
                return a->Value();
    return NULL;
}

TiXmlAttribute* a_first(TiXmlNode* n) {
    return n->ToElement()->FirstAttribute();
}

TiXmlAttribute* a_next(TiXmlAttribute* a) {
    return a->Next();
}

const char *a_value(TiXmlAttribute* a) {
    return a->Value();
}

const char *a_name(TiXmlAttribute* a) {
    return a->Name();
}


// ----------------------------------------------------------------------
};

