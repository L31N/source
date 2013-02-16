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

#ifndef DLL_EXPORT
#define DLL_EXPORT
#endif

#ifdef __cplusplus
class TiXmlNode;
class TiXmlAttribute;
#else
typedef struct TiXmlNode TiXmlNode;
typedef struct TiXmlAttribute TiXmlAttribute;
#endif

DLL_EXPORT TiXmlNode* load_doc(const char *path, char **err);
DLL_EXPORT void free_doc(TiXmlNode* c);
DLL_EXPORT TiXmlNode* get(TiXmlNode *n, const char *v);
DLL_EXPORT TiXmlNode *child(TiXmlNode *c);
DLL_EXPORT TiXmlNode *next(TiXmlNode *c);

DLL_EXPORT const char *a_get(TiXmlNode* n, const char *name);
DLL_EXPORT TiXmlAttribute* a_first(TiXmlNode* n);
DLL_EXPORT TiXmlAttribute* a_next(TiXmlAttribute* a);

DLL_EXPORT const char *a_value(TiXmlAttribute* a);
DLL_EXPORT const char *a_name(TiXmlAttribute* a);
DLL_EXPORT const char *value(TiXmlNode* a);
DLL_EXPORT const char *txml_error(void);

#define c_new(T) (T*)calloc(1,sizeof(T))
#define loop_childs(c,n) for (c = child(n); c; c = next(c))
#define loop_attr(a,n) for (a = a_first(n); a; a = a_next(a))

