#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
    s21_size_t k = 0, score = 0;

  for (s21_size_t i = 0; *(str1 + i) != '\0'; i++) {
    if (score == i) {
      for (s21_size_t j = 0; *(str2 + j) != '\0'; j++) {
        if (*(str2 + j) == *(str1 + i)) {
          k = 1;
          break;
        }
      }
      if (k > 0) {
        break;
      }
      score++;
    }
  }
  return score;
}
