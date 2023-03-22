#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
    char *dest_c = (char*)dest;
    const char *src_c = (char*)src;
    for (s21_size_t i = 0; i < n; i++) {
        dest_c[i] = src_c[i];
    }
    return dest;
}
