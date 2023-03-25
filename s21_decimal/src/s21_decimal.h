#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#define MAXBIT 31
#define BYTESCALE 3
#define S21_FIRSTDIGIT (int)(sizeof(int) * CHAR_BIT * BYTESCALE - 1)
#define S21_SIGNPLACE S21_FIRSTDIGIT + MAXBIT + 1
#define LBYTESCALE 6
#define S21INT 32
#define S21_LFD S21INT * LBYTESCALE + MAXBIT
//  BYTESCALE - the index for s21_decimal to get inner unsigned int with scale
//      info
//  S21_FIRSTDIGIT - on x32 computers it equals to 95
//  S21_LFD - S21_FIRSTDIGIT for s21_ldecimal

typedef struct {
    unsigned int bits[4];
} s21_decimal;

typedef struct large {
    unsigned int bits[7];
} s21_ldecimal;

int s21_checkSign(s21_decimal a);
void s21_setSign(s21_decimal *a);
void s21_getNull(s21_decimal *a);
int s21_checkNull(s21_decimal a);
int s21_getScale(s21_decimal a);
int s21_shift(s21_decimal *a, int direction);
void s21_reverse(s21_decimal *a);
int s21_MaxDigitPos(s21_decimal a, int pos);
int s21_is_valid(s21_decimal a);
int s21_addSupport(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_subSupport(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_checkbit(s21_decimal a, const unsigned int pos);
void s21_setbit(s21_decimal *a, const unsigned int pos);
void s21_lsetbit(s21_decimal *a, const unsigned int pos);
void s21_unsetbit(s21_decimal *a, const unsigned int pos);
void s21_switchbit(s21_decimal *a, const unsigned int pos);
int s21_mul_10(s21_decimal a, s21_decimal *rez);
void s21_div10(s21_decimal a, s21_decimal *res, int flag);
int s21_is_greater_for_div(s21_decimal a, s21_decimal b);
int s21_increaseScale(s21_decimal *a, int amount);
void s21_equalizer(s21_decimal *a, s21_decimal *b);
void s21_setScale(s21_decimal *a, int scale);
void s21_float_to_string(float src, char *float_str);
int s21_get_exp(const char *src);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
void s21_setlbit(s21_ldecimal *value, const unsigned int pos);
void s21_switchlbit(s21_ldecimal *a, const unsigned int pos);
void s21_lreverse(s21_ldecimal *a);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
void s21_lgetNull(s21_ldecimal *value);
void s21_to_ldecimal(s21_decimal value, s21_ldecimal *res);
int s21_lgetMaxDigitPos(s21_ldecimal value, int pos);
int s21_checklbit(s21_ldecimal value, const unsigned int pos);
int s21_lshift(s21_ldecimal *value, int direction);
int s21_lcheckNull(s21_ldecimal a);
int s21_lis_greater_for_div(s21_ldecimal a, s21_ldecimal b);
int s21_lsubSupport(s21_ldecimal value_1, s21_ldecimal value_2, s21_ldecimal *result);
int s21_laddSupport(s21_ldecimal value_1, s21_ldecimal value_2, s21_ldecimal *result);
int s21_mulSupport(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_ldivSupport(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_divSupport(s21_decimal a, s21_decimal b, s21_decimal *res);
int s21_to_decimal(s21_ldecimal value, int scale, s21_decimal *res);
int s21_lincreaseScale(s21_ldecimal *a, int *scale, int amount);
int s21_lmul_10(s21_ldecimal a, s21_ldecimal *rez);
void s21_ldiv10(s21_ldecimal a, s21_ldecimal *res, int flag);

#endif  //  SRC_S21_DECIMAL_H_
