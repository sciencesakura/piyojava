#include "piyojava.h"

void *cp(void **constant_pool, u2 index)
{
  return index == 0 ? NULL : constant_pool[index - 1];
}

bool utf8eq(const CONSTANT_Utf8_info *a, const CONSTANT_Utf8_info *b)
{
  return a == b || (a->length == b->length && memcmp(a->bytes, b->bytes, a->length) == 0);
}

Method_info *find_method(const ClassFile *cf, const CONSTANT_NameAndType_info *nat)
{
  for (u2 i = 0; i < cf->methods_count; i++) {
    Method_info *me = &cf->methods[i];
    if (utf8eq(me->name, nat->name) && utf8eq(me->descriptor, nat->descriptor))
      return me;
  }
  error(L"method was not found");
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
