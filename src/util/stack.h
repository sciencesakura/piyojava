#ifndef UTIL_STACK_H
#define UTIL_STACK_H

#include <stddef.h>
#include <stdint.h>

void stack_ipush(intptr_t **sp, intptr_t e);

intptr_t stack_ipop(intptr_t **sp);

intptr_t _stack_ipeek_0(const intptr_t *sp);

intptr_t _stack_ipeek_n(const intptr_t *sp, size_t offset);

#define _stack_ipeek(_1, _2, X, ...) X
#define stack_ipeek(...)             _stack_ipeek(__VA_ARGS__, _stack_ipeek_n, _stack_ipeek_0, _)(__VA_ARGS__)

void stack_push(intptr_t **sp, void *e);

void *stack_pop(intptr_t **sp);

void *_stack_peek_0(const intptr_t *sp);

void *_stack_peek_n(const intptr_t *sp, size_t offset);

#define _stack_peek(_1, _2, X, ...) X
#define stack_peek(...)             _stack_peek(__VA_ARGS__, _stack_peek_n, _stack_peek_0, _)(__VA_ARGS__)

#endif
