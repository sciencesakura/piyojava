#include "piyojava.h"

static void read_attributes(u2 count, void ***ptr, void **constant_pool, FILE *strm);

static void read_bytes(void *ptr, size_t size, FILE *strm)
{
  fread(ptr, 1, size, strm);
}

static void read_u1(u1 *ptr, FILE *strm)
{
  read_bytes(ptr, 1, strm);
}

static void read_u2(u2 *ptr, FILE *strm)
{
  u1 buf[2];
  read_bytes(buf, 2, strm);
  *ptr = (buf[0] << 8) | buf[1];
}

static void read_u4(u4 *ptr, FILE *strm)
{
  u1 buf[4];
  read_bytes(buf, 4, strm);
  *ptr = (buf[0] << 8 * 3) | (buf[1] << 8 * 2) | (buf[2] << 8 * 1) | buf[3];
}

static void read_constant_pool(u2 count, void ***ptr, FILE *strm)
{
  *ptr = calloc(count - 1, sizeof(void *));
  for (u2 i = 0; i < count - 1; i++) {
    u1 tag;
    read_u1(&tag, strm);
    switch (tag) {
    case CONSTANT_Utf8: {
      CONSTANT_Utf8_info *c = malloc(sizeof(CONSTANT_Utf8_info));
      (*ptr)[i] = c;
      c->tag = tag;
      read_u2(&c->length, strm);
      c->bytes = calloc(c->length, 1);
      read_bytes(c->bytes, c->length, strm);
      break;
    }
    case CONSTANT_Integer: {
      CONSTANT_Integer_info *c = malloc(sizeof(CONSTANT_Integer_info));
      (*ptr)[i] = c;
      c->tag = tag;
      u4 bytes;
      read_u4(&bytes, strm);
      c->value = (int32_t)bytes; // TODO check endian
      break;
    }
    case CONSTANT_Class: {
      CONSTANT_Class_info *c = malloc(sizeof(CONSTANT_Class_info));
      (*ptr)[i] = c;
      c->tag = tag;
      read_u2(&c->name_index, strm);
      break;
    }
    case CONSTANT_Methodref: {
      CONSTANT_Methodref_info *c = malloc(sizeof(CONSTANT_Methodref_info));
      (*ptr)[i] = c;
      c->tag = tag;
      read_u2(&c->class_index, strm);
      read_u2(&c->name_and_type_index, strm);
      break;
    }
    case CONSTANT_NameAndType: {
      CONSTANT_NameAndType_info *c = malloc(sizeof(CONSTANT_NameAndType_info));
      (*ptr)[i] = c;
      c->tag = tag;
      read_u2(&c->name_index, strm);
      read_u2(&c->descriptor_index, strm);
      break;
    }
    default:
      error(L"constant_pool[%" PRIu16 "] has unknown tag(%" PRIu8 ")", i + 1, tag);
    }
  }
  for (u2 i = 0; i < count - 1; i++) {
    Cp_info *tc = (*ptr)[i];
    switch (tc->tag) {
    case CONSTANT_Class: {
      CONSTANT_Class_info *c = (CONSTANT_Class_info *)tc;
      c->name = cp(*ptr, c->name_index);
      break;
    }
    case CONSTANT_Methodref: {
      CONSTANT_Methodref_info *c = (CONSTANT_Methodref_info *)tc;
      c->class = cp(*ptr, c->class_index);
      c->name_and_type = cp(*ptr, c->name_and_type_index);
      break;
    }
    case CONSTANT_NameAndType: {
      CONSTANT_NameAndType_info *c = (CONSTANT_NameAndType_info *)tc;
      c->name = cp(*ptr, c->name_index);
      c->descriptor = cp(*ptr, c->descriptor_index);
      break;
    }
    default:
      break;
    }
  }
}

static void read_interfaces(u2 count, CONSTANT_Class_info ***ptr, FILE *strm)
{
  if (count == 0) {
    *ptr = NULL;
    return;
  }
  // TODO
  error(L"interface is unsupported");
}

static void read_code_attribute(void **ptr, CONSTANT_Utf8_info *attribute_name,
                                void **constant_pool, FILE *strm)
{
  Code_attribute *attr = malloc(sizeof(Code_attribute));
  *ptr = attr;
  attr->attribute_name = attribute_name;
  read_u4(&attr->attribute_length, strm);
  read_u2(&attr->max_stack, strm);
  read_u2(&attr->max_locals, strm);
  read_u4(&attr->code_length, strm);
  attr->code = calloc(attr->code_length, 1);
  read_bytes(attr->code, attr->code_length, strm);
  read_u2(&attr->exception_table_length, strm);
  if (attr->exception_table_length != 0) {
    attr->exception_table = calloc(attr->exception_table_length, sizeof(Exception_table));
    for (u2 i = 0; i < attr->exception_table_length; i++) {
      Exception_table *et = &attr->exception_table[i];
      read_u2(&et->start_pc, strm);
      read_u2(&et->end_pc, strm);
      read_u2(&et->handler_pc, strm);
      u2 catch_type;
      read_u2(&catch_type, strm);
      et->catch_type = cp(constant_pool, catch_type);
    }
  }
  read_u2(&attr->attributes_count, strm);
  read_attributes(attr->attributes_count, &attr->attributes, constant_pool, strm);
}

static void read_attributes(u2 count, void ***ptr, void **constant_pool, FILE *strm)
{
  if (count == 0) {
    *ptr = NULL;
    return;
  }
  *ptr = calloc(count, sizeof(void *));
  for (u2 i = 0; i < count; i++) {
    u2 attribute_name_index;
    read_u2(&attribute_name_index, strm);
    CONSTANT_Utf8_info *attribute_name = cp(constant_pool, attribute_name_index);
    if (utf8_has(attribute_name, "Code")) {
      read_code_attribute(&(*ptr)[i], attribute_name, constant_pool, strm);
    } else {
      error(L"attributes[%" PRIu16 "] is unknown", i);
    }
  }
}

static void read_fields(u2 count, Field_info **ptr, void **constant_pool, FILE *strm)
{
  if (count == 0) {
    *ptr = NULL;
    return;
  }
  // TODO
  error(L"field is unsupported");
}

static void read_methods(u2 count, Method_info **ptr, void **constant_pool, FILE *strm)
{
  if (count == 0) {
    *ptr = NULL;
    return;
  }
  *ptr = calloc(count, sizeof(Method_info));
  for (u2 i = 0; i < count; i++) {
    Method_info *me = &(*ptr)[i];
    read_u2(&me->access_flags, strm);
    u2 name_index;
    read_u2(&name_index, strm);
    me->name = cp(constant_pool, name_index);
    u2 descriptor_index;
    read_u2(&descriptor_index, strm);
    me->descriptor = cp(constant_pool, descriptor_index);
    read_u2(&me->attributes_count, strm);
    read_attributes(me->attributes_count, &me->attributes, constant_pool, strm);
  }
}

ClassFile *parse_class(const char *classname)
{
  FILE *strm = fopen(classname, "rb");
  ClassFile *cf = malloc(sizeof(ClassFile));
  read_u4(&cf->magic, strm);
  read_u2(&cf->minor_version, strm);
  read_u2(&cf->major_version, strm);
  read_u2(&cf->constant_pool_count, strm);
  read_constant_pool(cf->constant_pool_count, &cf->constant_pool, strm);
  read_u2(&cf->access_flags, strm);
  u2 this_class;
  read_u2(&this_class, strm);
  cf->this_class = cp(cf->constant_pool, this_class);
  u2 super_class;
  read_u2(&super_class, strm);
  cf->super_class = cp(cf->constant_pool, super_class);
  read_u2(&cf->interfaces_count, strm);
  read_interfaces(cf->interfaces_count, &cf->interfaces, strm);
  read_u2(&cf->fields_count, strm);
  read_fields(cf->fields_count, &cf->fields, cf->constant_pool, strm);
  read_u2(&cf->methods_count, strm);
  read_methods(cf->methods_count, &cf->methods, cf->constant_pool, strm);
  read_u2(&cf->attributes_count, strm);
  read_attributes(cf->attributes_count, &cf->attributes, cf->constant_pool, strm);
  return cf;
}
