#include "util/stack.h"

void stack_ipush(intptr_t **sp, intptr_t e)
{
  *(*sp)++ = e;
}

intptr_t stack_ipop(intptr_t **sp)
{
  return *--(*sp);
}

intptr_t stack_ipeek(const intptr_t *sp)
{
  return *(sp - 1);
}

void stack_push(intptr_t **sp, void *e)
{
  stack_ipush(sp, (intptr_t)e);
}

void *stack_pop(intptr_t **sp)
{
  return (void *)stack_ipop(sp);
}

void *stack_peek(const intptr_t *sp)
{
  return (void *)stack_ipeek(sp);
}
