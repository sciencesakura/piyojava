#include "piyojava.h"
#include "util/stack.h"

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

HashTable classes;
HashTable stringpool;

int main(int argc, char **argv)
{
  setlocale(LC_ALL, "");
  if (argc < 2) {
    error(L"piyojava <classfile>");
  }
  intptr_t _vmstack[1024]; // サイズ適当
  intptr_t *vmstack = _vmstack;
  hashtable_init(&classes, 128, utf8hash, utf8eq);
  hashtable_init(&stringpool, 128, utf8hash, utf8eq);
  ClassFile *cf = load_class(vmstack, &UTF8_LITERAL(strlen(argv[1]), argv[1]));
  Method_info *main = find_method(cf, &NAT_LITERAL(4, "main", 22, "([Ljava/lang/String;)V"));
  if (main == NULL || !(main->access_flags & ME_ACC_STATIC)) {
    error(L"not found");
  }
  Code_attribute *code = code_attr(main);
  intptr_t variables[code->max_locals];
  intptr_t operands[code->max_stack];
  stack_push(&vmstack, &(Frame) { 0, code, variables, operands, cf->constant_pool });
  execute(vmstack);
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
#ifdef DEBUG
  va_list params;
  va_start(params, message);
  fwprintf(stderr, L"DEBUG: ");
  vfwprintf(stderr, message, params);
  fwprintf(stderr, L"\n");
#endif
}
