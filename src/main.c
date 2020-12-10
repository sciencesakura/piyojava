#include "piyojava.h"
#include "util/stack.h"

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
  ClassFile *cf = load_class(vmstack, &UTF8_LITERAL(argv[1]));
  Method_info *main = find_method(cf, &NAT_LITERAL("main", "([Ljava/lang/String;)V"));
  if (main == NULL || !(main->access_flags & ME_ACC_STATIC)) {
    error(L"not found");
  }
  Code_attribute *code = code_attr(main);
  intptr_t variables[code->max_locals];
  intptr_t operands[code->max_stack];
  stack_push(&vmstack, &(Frame) { 0, code, variables, operands, cf->constant_pool });
  execute(vmstack);
  for (u2 i = 0; i < code->max_locals; i++) {
    log_debug(L"locals[%" PRIu16 "]=%" PRIdPTR, i, variables[i]);
  }
  return EXIT_SUCCESS;
}
