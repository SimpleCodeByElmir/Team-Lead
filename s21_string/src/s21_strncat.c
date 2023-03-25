#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t k = 0;
  for (s21_size_t i = 0; *(dest + i) != '\0'; i++) {
    k++;
  }
  for (s21_size_t i = 0; i < n; i++) {
    *((char *)dest + i + k) = *((char *)src + i);
    if (*((char *)src + i) == '\0') {
      *((char *)dest + i + k + 1) = '\0';
      break;
    }
  }
  return (char *)dest;
}
