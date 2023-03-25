#include <stdlib.h>

#include "s21_string.h"

void *s21_to_lower(const char *str) {
    void *result = s21_NULL;

    s21_size_t len = 0;
    if (str) {
        len = s21_strlen(str);
    }

    char *scopy = s21_NULL;
    if (str) {
        scopy = calloc(len + 1, sizeof(char));
    }

    if (scopy) {
        s21_size_t i;
        for (i = 0; str[i] != '\0'; i++) {
            scopy[i] = str[i];
        }
        scopy[i] = '\0';

        s21_size_t j = 0;
        while (scopy[j]) {
            if (scopy[j] >= 65 && scopy[j] <= 90) {
                scopy[j] = scopy[j] + 32;
            }
            j++;
        }
        result = scopy;
    }

    return result;
}
