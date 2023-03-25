#include "s21_string.h"

char* s21_strrchr(const char *str, int c) {
    char *ret = s21_NULL;

    const char *str_end = str + s21_strlen(str);

    while (str_end != str) {
        if (*str_end == (char)c) {
            /*  Сравниваем, начиная с последнего '\0'-символа   */
            ret = (char *)str_end;
            break;
        }
        str_end--;
    }

    return ret;
}
