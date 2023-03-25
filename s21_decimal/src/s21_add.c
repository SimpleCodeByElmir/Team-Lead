#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sign_v1 = s21_checkSign(value_1);
  int sign_v2 = s21_checkSign(value_2);
  int flag = 0;
  s21_equalizer(&value_1, &value_2);
  int scale = s21_getScale(value_1);
  int sign = 0;
  int supSign;

  if (sign_v1 != sign_v2) {
    if (sign_v2)
      supSign = s21_subSupport(value_1, value_2, result);
    else
      supSign = s21_subSupport(value_2, value_1, result);
    if (supSign == -1) sign = 1;
  } else {
    int check = s21_addSupport(value_1, value_2, result);
    sign = sign_v1;
    if (!check && scale > 0) {
      scale--;
      s21_shift(result, -1);
      s21_setbit(result, S21_FIRSTDIGIT);
      s21_decimal hdec = {{5, 0, 0, 0}};
      s21_divSupport(*result, hdec, result);
    } else if (!check) {
      flag = sign + 1;
      s21_getNull(result);
    }
  }
  if (sign) s21_setSign(result);
  if (!s21_checkNull(*result)) s21_setScale(result, scale);

  return flag;
}
