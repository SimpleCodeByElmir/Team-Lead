#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int flag = 0;

  if (dst) {
    s21_getNull(dst);
    dst->bits[0] = src;
    if (src < 0) {
      s21_decimal negativeOne = {{1, 0, 0, 0}};
      s21_subSupport(*dst, negativeOne, dst);
      s21_reverse(dst);
      dst->bits[1] = dst->bits[2] = 0;
      s21_setbit(dst, S21_SIGNPLACE);
    }
  } else {
    flag = 1;
  }

  return flag;
}
