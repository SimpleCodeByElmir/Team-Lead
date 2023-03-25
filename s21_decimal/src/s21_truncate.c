#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  *result = value;
  int scale = s21_getScale(value);

  while (scale--) s21_div10(*result, result, 0);
  if (s21_checkSign(value)) s21_setSign(result);

  return 0;
}
