#include "s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int sign_v1 = s21_checkSign(value_1);
  int sign_v2 = s21_checkSign(value_2);
  int flag = 0;

  if (sign_v1 == sign_v2) {
    s21_equalizer(&value_1, &value_2);
    if (s21_subSupport(value_1, value_2, &value_1) == 0) flag = 1;
  }

  return flag;
}
