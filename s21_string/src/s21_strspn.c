#include "s21_string.h"

s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t pol = 0;

  for (s21_size_t i = 0; *(str1 + i) != '\0'; i++) {
    for (s21_size_t j = 0; *(str2 + j) != '\0'; j++) {
      if (*(str2 + j) == *(str1 + i)) {
        pol++;
        j = s21_strlen(str2) - 1;
      }
    }
    if (pol != i + 1) {
      i = s21_strlen(str1) - 1;
    }
  }
  return pol;
}
