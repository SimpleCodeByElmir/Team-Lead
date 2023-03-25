#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *ret = (char *)haystack;
  if (needle[0]) {
    while (*haystack && *haystack != *needle) {
      haystack++;
    }
    if (*haystack != '\0') {
      s21_size_t i = 0;
      while (haystack[i] && needle[i] && haystack[i] == needle[i]) {
        i++;
      }
      if (needle[i] != '\0') {
        haystack += i;
        ret = s21_strstr(haystack, needle);
      } else {
        ret = (char *)haystack;
      }
    } else {
      ret = s21_NULL;
    }
  }
  return ret;
}
