#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *str_saved = s21_NULL;
  int error = 0;
  if (str == s21_NULL) {
    str = str_saved;
  } else {
    str_saved = str;
  }
  if (str == s21_NULL) {
    error++;
  }
  if (error == 0) {
    /* Пропускаем неподходящую часть str, состоящую из символов-разделителей */
    while (*str && s21_strpbrk(str, delim) == str) {
      str++;
    }
    if (*str == '\0' && error == 0) {
      error++;
    }
    if (error == 0) {
      char *token_end = s21_strpbrk(str, delim);
      if (token_end) {
        *token_end = '\0';
        str_saved = token_end + 1;
      } else {
        str_saved = s21_NULL;
      }
    } else {
      str = s21_NULL;
    }
  } else {
    str = s21_NULL;
  }
  return str;
}
