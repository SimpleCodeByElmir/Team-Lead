#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  *result = value;
  s21_decimal one = {{1, 0, 0, 0}};

  s21_truncate(*result, result);
  if (s21_checkSign(*result)) s21_sub(*result, one, result);

  return 0;
}
