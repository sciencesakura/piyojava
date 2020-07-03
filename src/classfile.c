#include "piyojava.h"

void *cp(void **constant_pool, u2 index)
{
  return index == 0 ? NULL : constant_pool[index - 1];
}

size_t utf8hash(const void *utf8)
{
  CONSTANT_Utf8_info *u = (CONSTANT_Utf8_info *)utf8;
  size_t hv = 0;
  for (u2 i = 0; i < u->length; i++) {
    hv = 31 * hv + *(u->bytes + i);
  }
  return hv;
}

bool utf8eq(const void *a, const void *b)
{
  CONSTANT_Utf8_info *ua = (CONSTANT_Utf8_info *)a;
  CONSTANT_Utf8_info *ub = (CONSTANT_Utf8_info *)b;
  return ua == ub || (ua->length == ub->length && memcmp(ua->bytes, ub->bytes, ua->length) == 0);
}

bool utf8has(const CONSTANT_Utf8_info *utf8, size_t len, const void *str)
{
  return utf8->bytes == str || (utf8->length == len && memcmp(utf8->bytes, str, len) == 0);
}

Field_info *find_field(const ClassFile *cf, const CONSTANT_NameAndType_info *nat)
{
  for (u2 i = 0; i < cf->fields_count; i++) {
    Field_info *fi = &cf->fields[i];
    if (utf8eq(fi->name, nat->name))
      return fi;
  }
  return NULL;
}

Method_info *find_method(const ClassFile *cf, const CONSTANT_NameAndType_info *nat)
{
  for (u2 i = 0; i < cf->methods_count; i++) {
    Method_info *me = &cf->methods[i];
    if (utf8eq(me->name, nat->name) && utf8eq(me->descriptor, nat->descriptor))
      return me;
  }
  return NULL;
}

static void *find_attribute(u2 attributes_count, void **attributes, size_t namelen,
                            const char *name)
{
  for (u2 i = 0; i < attributes_count; i++) {
    Attribute_info *attr = attributes[i];
    if (attr->attribute_name->length == namelen
        && memcmp(attr->attribute_name->bytes, name, namelen) == 0)
      return attr;
  }
  error(L"%s_attribute was not found", name);
}

Code_attribute *code_attr(const Method_info *method)
{
  return find_attribute(method->attributes_count, method->attributes, 4, "Code");
}
