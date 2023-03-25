#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int flag = 0;

  if (!dst || s21_getScale(src) > 28) {
    flag = 1;
  } else {
    *dst = 0.0;
    int scale = s21_getScale(src);
    double tmp = *dst;
    unsigned long base = 1;
    for (int i = 0; i < 96; i++) {
      tmp += s21_checkbit(src, i) * base;
      base = base * 2;
    }
    while (scale != 0) {
      tmp = tmp / 10;
      scale--;
    }
    if (s21_checkSign(src)) tmp *= -1;
    *dst = tmp;
  }

  return flag;
}
