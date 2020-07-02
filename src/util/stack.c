#include "util/stack.h"

void stack_ipush(intptr_t **sp, intptr_t e)
{
  *(*sp)++ = e;
}

intptr_t stack_ipop(intptr_t **sp)
{
  return *--(*sp);
}

intptr_t _stack_ipeek_0(const intptr_t *sp)
{
  return *(sp - 1);
}

intptr_t _stack_ipeek_n(const intptr_t *sp, size_t offset)
{
  return *(sp - offset - 1);
}

void stack_push(intptr_t **sp, void *e)
{
  stack_ipush(sp, (intptr_t)e);
}

void *stack_pop(intptr_t **sp)
{
  return (void *)stack_ipop(sp);
}

void *_stack_peek_0(const intptr_t *sp)
{
  return (void *)_stack_ipeek_0(sp);
}

void *_stack_peek_n(const intptr_t *sp, size_t offset)
{
  return (void *)_stack_ipeek_n(sp, offset);
}
