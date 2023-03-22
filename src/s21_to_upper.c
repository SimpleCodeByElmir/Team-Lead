#include <stdlib.h>

#include "s21_string.h"

void *s21_to_upper(const char *str) {
  s21_size_t t = 0;
  while (str[t] != 0) {
    t++;
  }
  char *srr = (char *)calloc(t + 1, sizeof(char));
  if (srr == s21_NULL) {
      return s21_NULL;
  }
  for (s21_size_t q = 0; str[q] != 0; q++) {
    srr[q] = str[q];
    if (srr[q] >= 97 && srr[q] <= 122) {
      srr[q] -= 'a' - 'A';
    }
  }
  srr[t] = '\0';
  return srr;
}
