#include "util/common.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void log(FILE *restrict strm, const wchar_t *restrict prefix,
                const wchar_t *restrict message, va_list arg)
{
  fwprintf(strm, prefix);
  vfwprintf(strm, message, arg);
  fwprintf(strm, L"\n");
}

noreturn void error(const wchar_t *restrict message, ...)
{
  va_list arg;
  va_start(arg, message);
  log(stderr, L"ERROR: ", message, arg);
  va_end(arg);
  exit(EXIT_FAILURE);
}

void log_error(const wchar_t *restrict message, ...)
{
  va_list arg;
  va_start(arg, message);
  log(stderr, L"ERROR: ", message, arg);
  va_end(arg);
}

void log_debug(const wchar_t *restrict message, ...)
{
#ifdef DEBUG
  va_list arg;
  va_start(arg, message);
  log(stderr, L"DEBUG: ", message, arg);
  va_end(arg);
#endif
}
