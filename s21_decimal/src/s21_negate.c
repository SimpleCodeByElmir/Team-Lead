#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;

  s21_switchbit(result, S21_SIGNPLACE);

  return 0;
}
