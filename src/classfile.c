#include "piyojava.h"

void *cp(void **constant_pool, u2 index)
{
  return index == 0 ? NULL : constant_pool[index - 1];
}

bool utf8_has(const CONSTANT_Utf8_info *utf8, const char *str)
{
  return utf8->length == strlen(str) && memcmp(utf8->bytes, str, utf8->length) == 0;
}

Method_info *find_method(const ClassFile *cf, u2 access_flag, const char *name, const char *descriptor)
{
  for (u2 i = 0; i < cf->methods_count; i++) {
    Method_info *me = &cf->methods[i];
    if (me->access_flags & access_flag && utf8_has(me->name, name) && utf8_has(me->descriptor, descriptor)) return me;
  }
  error(L"method(%s:%s) was not found", name, descriptor);
}

void *find_attribute(u2 attributes_count, void **attributes, const char *name)
{
  for (u2 i = 0; i < attributes_count; i++) {
    Attribute_info *attr = attributes[i];
    if (utf8_has(attr->attribute_name, name)) return attr;
  }
  error(L"%s_attribute was not found", name);
}
