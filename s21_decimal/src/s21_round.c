#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int i = s21_getScale(value);
  int sign = s21_checkSign(value);

  while (i--) {
    if (i == 0)
      s21_div10(value, &value, 1);
    else
      s21_div10(value, &value, 0);
  }
  *result = value;
  if (sign) s21_setSign(result);

  return 0;
}
