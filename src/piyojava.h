#ifndef PIYOJAVA_H
#define PIYOJAVA_H

#include "util/hashtable.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <wchar.h>

typedef int32_t jint;
typedef wchar_t jchar;
typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef enum Constant_tag Constant_tag;
typedef struct ClassFile ClassFile;
typedef struct Cp_info Cp_info;
typedef struct CONSTANT_Utf8_info CONSTANT_Utf8_info;
typedef struct CONSTANT_Integer_info CONSTANT_Integer_info;
// typedef struct CONSTANT_Float_info CONSTANT_Float_info;
// typedef struct CONSTANT_Long_info CONSTANT_Long_info;
// typedef struct CONSTANT_Double_info CONSTANT_Double_info;
typedef struct CONSTANT_Class_info CONSTANT_Class_info;
// typedef struct CONSTANT_String_info CONSTANT_String_info;
typedef struct CONSTANT_Fieldref_info CONSTANT_Fieldref_info;
typedef struct CONSTANT_Methodref_info CONSTANT_Methodref_info;
// typedef struct CONSTANT_InterfaceMethodref_info CONSTANT_InterfaceMethodref_info;
typedef struct CONSTANT_NameAndType_info CONSTANT_NameAndType_info;
// typedef struct CONSTANT_MethodHandle_info CONSTANT_MethodHandle_info;
// typedef struct CONSTANT_MethodType_info CONSTANT_MethodType_info;
// typedef struct CONSTANT_InvokeDynamic_info CONSTANT_InvokeDynamic_info;
typedef struct Field_info Field_info;
typedef struct Method_info Method_info;
typedef struct Attribute_info Attribute_info;
typedef struct Code_attribute Code_attribute;
typedef struct Exception_table Exception_table;
typedef struct Frame Frame;

enum Constant_tag {
  CONSTANT_Utf8 = 1,
  CONSTANT_Integer = 3,
  CONSTANT_Float = 4,
  CONSTANT_Long = 5,
  CONSTANT_Double = 6,
  CONSTANT_Class = 7,
  CONSTANT_String = 8,
  CONSTANT_Fieldref = 9,
  CONSTANT_Methodref = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_NameAndType = 12,
  CONSTANT_MethodHandle = 15,
  CONSTANT_MethodType = 16,
  CONSTANT_InvokeDynamic = 18,
};

struct ClassFile {
  u4 magic;
  u2 minor_version;
  u2 major_version;
  u2 constant_pool_count;
  void **constant_pool;
  u2 access_flags;
  CONSTANT_Class_info *this_class;
  CONSTANT_Class_info *super_class;
  u2 interfaces_count;
  CONSTANT_Class_info **interfaces;
  u2 fields_count;
  Field_info *fields;
  u2 methods_count;
  Method_info *methods;
  u2 attributes_count;
  void **attributes;
};

struct Cp_info {
  Constant_tag tag;
};

struct CONSTANT_Utf8_info {
  Constant_tag tag;
  u2 length;
  u1 *bytes;
};

struct CONSTANT_Integer_info {
  Constant_tag tag;
  jint value;
};

struct CONSTANT_Class_info {
  Constant_tag tag;
  u2 name_index;
  CONSTANT_Utf8_info *name;
};

struct CONSTANT_Fieldref_info {
  Constant_tag tag;
  u2 class_index;
  CONSTANT_Class_info *class;
  u2 name_and_type_index;
  CONSTANT_NameAndType_info *name_and_type;
};

struct CONSTANT_Methodref_info {
  Constant_tag tag;
  u2 class_index;
  CONSTANT_Class_info *class;
  u2 name_and_type_index;
  CONSTANT_NameAndType_info *name_and_type;
};

struct CONSTANT_NameAndType_info {
  Constant_tag tag;
  u2 name_index;
  CONSTANT_Utf8_info *name;
  u2 descriptor_index;
  CONSTANT_Utf8_info *descriptor;
};

struct Field_info {
  u2 access_flags;
  CONSTANT_Utf8_info *name;
  CONSTANT_Utf8_info *descriptor;
  u2 attributes_count;
  void **attributes;
  intptr_t staticval;
};

struct Method_info {
  u2 access_flags;
  CONSTANT_Utf8_info *name;
  CONSTANT_Utf8_info *descriptor;
  u2 attributes_count;
  void **attributes;
  u1 args_size;
};

struct Attribute_info {
  CONSTANT_Utf8_info *attribute_name;
  u4 attribute_length;
  u1 *info;
};

struct Code_attribute {
  CONSTANT_Utf8_info *attribute_name;
  u4 attribute_length;
  u2 max_stack;
  u2 max_locals;
  u4 code_length;
  u1 *code;
  u2 exception_table_length;
  Exception_table *exception_table;
  u2 attributes_count;
  void **attributes;
};

struct Exception_table {
  u2 start_pc;
  u2 end_pc;
  u2 handler_pc;
  CONSTANT_Class_info *catch_type;
};

struct Frame {
  u4 pc;
  Code_attribute *code;
  intptr_t *variables;
  intptr_t *operands;
  void **constant_pool;
};

#define FI_ACC_STATIC 0x0008
#define ME_ACC_STATIC 0x0008
#define ME_ACC_NATIVE 0x0100

extern HashTable classes;

void debug(const wchar_t *message, ...);

noreturn void error(const wchar_t *message, ...);

ClassFile *load_class(intptr_t *vmstack, CONSTANT_Utf8_info *name);

ClassFile *parse_class(size_t length, const void *name);

void *cp(void **constant_pool, u2 index);

size_t utf8hash(const void *utf8);

bool utf8eq(const void *a, const void *b);

bool utf8has(const CONSTANT_Utf8_info *utf8, size_t len, const void *str);

Field_info *find_field(const ClassFile *cf, const CONSTANT_NameAndType_info *nat);

Method_info *find_method(const ClassFile *cf, const CONSTANT_NameAndType_info *nat);

Code_attribute *code_attr(const Method_info *method);

void execute(intptr_t *vmstack);

#endif
