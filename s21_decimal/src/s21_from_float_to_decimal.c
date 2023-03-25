#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int flag = 0;

  if (src == 1 / 0.0 && src == 0 / 0.0 && src == -1 / 0.0 && src == -0 / 0.0) {
    flag = 1;
  } else {
    s21_getNull(dst);
    int sign = 0;
    if (src < 0) {
      sign = 1;
      src *= -1;
    }
    double src_local = src;
    char float_str[32] = "";
    s21_float_to_string((float)src_local, float_str);
    int exp = s21_get_exp(float_str);
    int scale = 0;
    while (scale < 28 && (int)src_local / (int)pow(2, 22) == 0) {
      src_local *= 10;
      scale++;
    }
    src_local = round(src_local);
    if (scale <= 28 && (exp > -94 && exp < 96)) {
      while (fmod(src_local, 10) == 0 && scale > 0) {
        src_local = src_local / 10;
        scale--;
      }
      s21_float_to_string((float)src_local, float_str);
      exp = s21_get_exp(float_str);
      s21_setbit(dst, exp);
      for (int i = exp - 1, j = 9; i >= 0 && j < 32; i--, j++) {
        if (float_str[j]) s21_setbit(dst, i);
      }
      s21_setScale(dst, scale);
      if (sign == 1) s21_setSign(dst);
    } else {
      flag = 1;
    }
    if (flag != 0) s21_getNull(dst);
  }

  return flag;
}
