#include "s21_decimal.h"

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less_or_equal(value_2, value_1);
}