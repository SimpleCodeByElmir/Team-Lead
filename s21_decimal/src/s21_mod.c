#include "s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_getNull(result);
  int flag = 0, scale = 0;
  int sign = s21_checkSign(value_1);
  s21_decimal res = {{0, 0, 0, 0}};

  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
    flag = 3;
  } else if (s21_is_less(value_1, value_2)) {
    *result = value_1;
  } else {
    s21_div(value_1, value_2, &res);
    scale = s21_getScale(value_1);
    s21_mul(res, value_2, &res);
    s21_subSupport(value_1, res, result);
  }
  s21_setScale(result, scale);
  if (s21_getScale(*result) > 28 && flag != 3) flag = sign + 1;
  if (flag != 0) s21_getNull(result);

  return flag;
}
