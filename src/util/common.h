#include <stdnoreturn.h>
#include <wchar.h>

noreturn void error(const wchar_t *restrict message, ...);

void log_error(const wchar_t *restrict message, ...);

void log_debug(const wchar_t *restrict message, ...);
