#include "piyojava.h"

void print_utf8(const CONSTANT_Utf8_info *utf8)
{
  char s[utf8->length + 1];
  memcpy(s, utf8->bytes, utf8->length);
  s[utf8->length] = '\0';
  wprintf(L"%s", s);
}

void log_classfile(ClassFile *cf)
{
  wprintf(L"magic=0x%08" PRIx32 "\n", cf->magic);
  wprintf(L"minor_version=%" PRIu16 "\n", cf->minor_version);
  wprintf(L"major_version=%" PRIu16 "\n", cf->major_version);
  wprintf(L"constant_pool_count=%" PRIu16 "\n", cf->constant_pool_count);
  wprintf(L"constant_pool=[\n");
  for (u2 i = 0; i < cf->constant_pool_count - 1; i++) {
    Cp_info *tc = cf->constant_pool[i];
    wprintf(L" -tag=%d\n", tc->tag);
    switch (tc->tag) {
    case CONSTANT_Utf8: {
      CONSTANT_Utf8_info *c = (CONSTANT_Utf8_info *)tc;
      wprintf(L"  length=%" PRIu16 "\n", c->length);
      wprintf(L"  bytes=");
      print_utf8(c);
      wprintf(L"\n");
      break;
    }
    case CONSTANT_Class: {
      CONSTANT_Class_info *c = (CONSTANT_Class_info *)tc;
      wprintf(L"  name=");
      print_utf8(c->name);
      wprintf(L"\n");
      break;
    }
    case CONSTANT_Methodref: {
      CONSTANT_Methodref_info *c = (CONSTANT_Methodref_info *)tc;
      wprintf(L"  class=");
      print_utf8(c->class->name);
      wprintf(L"\n");
      wprintf(L"  name_and_type=");
      print_utf8(c->name_and_type->name);
      wprintf(L":");
      print_utf8(c->name_and_type->descriptor);
      wprintf(L"\n");
      break;
    }
    case CONSTANT_NameAndType: {
      CONSTANT_NameAndType_info *c = (CONSTANT_NameAndType_info *)tc;
      wprintf(L"  name=");
      print_utf8(c->name);
      wprintf(L"\n");
      wprintf(L"  descriptor=");
      print_utf8(c->descriptor);
      wprintf(L"\n");
      break;
    }
    default:
      break;
    }
  }
  wprintf(L"]\n");
  wprintf(L"access_flags=0x%04" PRIu16 "\n", cf->access_flags);
  wprintf(L"this_class=...\n");
  wprintf(L"super_class=...\n");
  wprintf(L"interfaces_count=%" PRIu16 "\n", cf->interfaces_count);
  wprintf(L"interfaces=...\n");
  wprintf(L"fields_count=%" PRIu16 "\n", cf->fields_count);
  wprintf(L"fields=...\n");
  wprintf(L"methods_count=%" PRIu16 "\n", cf->methods_count);
  wprintf(L"methods=[\n");
  for (u2 i = 0; i < cf->methods_count; i++) {
    Method_info *me = &cf->methods[i];
    wprintf(L" -access_flags=0x%04" PRIu16 "\n", me->access_flags);
    wprintf(L"  name=");
    print_utf8(me->name);
    wprintf(L"\n");
    wprintf(L"  descriptor=");
    print_utf8(me->descriptor);
    wprintf(L"\n");
    wprintf(L"  attributes_count=%" PRIu16 "\n", me->attributes_count);
    wprintf(L"  attributes=...\n");
  }
  wprintf(L"]\n");
  wprintf(L"attributes_count=%" PRIu16 "\n", cf->attributes_count);
  wprintf(L"attributes=...\n");
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    error(L"piyojava <classfile>");
  }
  ClassFile *cf = parse_class(argv[1]);
  // log_classfile(cf);
  Method_info *main = find_method(cf, ME_ACC_STATIC, "main", "([Ljava/lang/String;)V");
  Code_attribute *code = code_attr(main);
  intptr_t variables[code->max_locals];
  intptr_t operands[code->max_stack];
  Frame *frame = &(Frame) { 0, code, variables, operands, cf->constant_pool };
  execute(frame);
  for (u2 i = 0; i < code->max_locals; i++) {
    debug(L"locals[%" PRIu16 "]=%" PRIdPTR, i, variables[i]);
  }
  return EXIT_SUCCESS;
}

noreturn void error(const wchar_t *message, ...)
{
  va_list params;
  va_start(params, message);
  fwprintf(stderr, L"ERROR: ");
  vfwprintf(stderr, message, params);
  fwprintf(stderr, L"\n");
  abort();
}

void debug(const wchar_t *message, ...)
{
  va_list params;
  va_start(params, message);
  fwprintf(stderr, L"DEBUG: ");
  vfwprintf(stderr, message, params);
  fwprintf(stderr, L"\n");
}
