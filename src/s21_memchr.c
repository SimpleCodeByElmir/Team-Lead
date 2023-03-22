#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *ret = s21_NULL;
  for (s21_size_t i = 0; i < n; i++) {
    if (c == *((char *)str + i)) {
      ret = ((char *)str + i);
      break;
    }
  }
  return ret;
}
