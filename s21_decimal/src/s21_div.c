#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag;

  if (s21_checkNull(value_2))
    flag = 3;
  else
    flag = s21_ldivSupport(value_1, value_2, result);

  return flag;
}
