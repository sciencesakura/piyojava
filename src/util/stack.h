#ifndef UTIL_STACK_H
#define UTIL_STACK_H

#include <stdint.h>

void stack_ipush(intptr_t **sp, intptr_t e);

intptr_t stack_ipop(intptr_t **sp);

intptr_t stack_ipeek(const intptr_t *sp);

void stack_push(intptr_t **sp, void *e);

void *stack_pop(intptr_t **sp);

void *stack_peek(const intptr_t *sp);

#endif
