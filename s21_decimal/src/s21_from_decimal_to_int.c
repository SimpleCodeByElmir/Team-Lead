#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  *dst = 0;
  int scale = s21_getScale(src);
  int sign = s21_checkSign(src);
  int flag = 0;

  while (scale > 0 && !s21_checkNull(src)) {
    s21_div10(src, &src, 0);
    scale--;
  }
  if (scale == 0 && src.bits[1] == 0 && src.bits[2] == 0 &&
      src.bits[0] <= 2147483648) {
    if (sign)
      *dst = -src.bits[0];
    else if (src.bits[0] <= 2147483647)
      *dst = src.bits[0];
    else
      flag = 1;
  } else {
    flag = 1;
  }

  return flag;
}
