#include "s21_decimal.h"

//  Equalize given decimals to one scale.
void s21_equalizer(s21_decimal *a, s21_decimal *b) {
  int sign_a = s21_checkSign(*a);
  int sign_b = s21_checkSign(*b);
  int scale_a = s21_getScale(*a);
  int scale_b = s21_getScale(*b);

  if (scale_a > scale_b) {
    if (s21_increaseScale(b, scale_a - scale_b)) {
      int difference = scale_a - s21_getScale(*b);
      while (difference--) s21_div10(*a, a, 1);
      s21_setScale(a, s21_getScale(*b));
    }
  } else {
    if (s21_increaseScale(a, scale_b - scale_a)) {
      int difference = scale_b - s21_getScale(*a);
      while (difference--) s21_div10(*b, b, 1);
      s21_setScale(b, s21_getScale(*a));
    }
  }
  if (sign_a) s21_setSign(a);
  if (sign_b) s21_setSign(b);
}

int s21_increaseScale(s21_decimal *a, int amount) {
  s21_decimal sup;

  int flag = 1, scale = s21_getScale(*a);
  while (s21_mul_10(*a, &sup) && amount--) {
    scale++;
    *a = sup;
  }
  if (amount == 0) flag = 0;
  s21_setScale(a, scale);

  return flag;
}

int s21_mul_10(s21_decimal a, s21_decimal *rez) {
  int flag;

  if (s21_MaxDigitPos(a, S21_FIRSTDIGIT) > S21_FIRSTDIGIT - 3) {
    flag = 0;
  } else {
    s21_decimal a_1, a_3;
    s21_shift(&a, 1);
    a_1 = a;
    s21_shift(&a, 2);
    a_3 = a;
    flag = s21_addSupport(a_1, a_3, rez);
  }

  return flag;
}

void s21_div10(s21_decimal a, s21_decimal *res, int flag) {
  s21_getNull(res);
  s21_decimal decade = {{10, 0, 0, 0}};
  s21_decimal ch = decade;
  int max = s21_MaxDigitPos(a, S21_FIRSTDIGIT);
  int maxDel = s21_MaxDigitPos(decade, S21_FIRSTDIGIT);

  s21_shift(&ch, max - maxDel);
  while (s21_is_greater_for_div(a, decade)) {
    if (s21_is_greater_for_div(a, ch)) {
      s21_setbit(res, 0);
      if (max > maxDel) {
        s21_shift(res, 1);
      }
      s21_subSupport(a, ch, &a);
    } else {
      s21_shift(res, 1);
    }
    s21_shift(&ch, -1);
    max--;
  }
  s21_decimal one = {{1, 0, 0, 0}};
  s21_shift(&decade, -1);
  if (max > maxDel) s21_shift(res, max - maxDel);
  if (s21_is_greater_for_div(a, decade) && flag) s21_addSupport(*res, one, res);
}

int s21_is_greater_for_div(s21_decimal a, s21_decimal b) {
  int res = 0;

  if (s21_subSupport(a, b, &b) != -1) res = 1;

  return res;
}

int s21_divSupport(s21_decimal a, s21_decimal b, s21_decimal *res) {
  s21_getNull(res);
  s21_decimal ch = b;
  int max = s21_MaxDigitPos(a, S21_FIRSTDIGIT);
  int maxDel = s21_MaxDigitPos(b, S21_FIRSTDIGIT);

  s21_shift(&ch, max - maxDel);
  while (s21_is_greater_for_div(a, b)) {
    if (s21_is_greater_for_div(a, ch)) {
      s21_setbit(res, 0);
      if (max > maxDel) {
        s21_shift(res, 1);
      }
      s21_subSupport(a, ch, &a);
    } else {
      s21_shift(res, 1);
    }
    s21_shift(&ch, -1);
    max--;
  }
  //    s21_decimal one = {{1, 0, 0, 0}};
  //    s21_shift(&b, -1);
  if (max > maxDel) s21_shift(res, max - maxDel);
  //    if (s21_is_greater_for_div(a, b)) s21_addSupport(*res, one, res);

  return 0;
}

int s21_addSupport(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result) {
  s21_getNull(result);
  int rem = 0;
  //  rem - indicates the remaining value

  for (int i = 0; i <= S21_FIRSTDIGIT; i++) {
    if (s21_checkbit(value_1, i) == 1 && s21_checkbit(value_2, i) == 1) {
      if (rem) s21_setbit(result, i);
      rem = 1;
    } else if (s21_checkbit(value_1, i) == 0 && s21_checkbit(value_2, i) == 0) {
      if (rem) s21_setbit(result, i);
      rem = 0;
    } else {
      if (!rem) s21_setbit(result, i);
    }
  }

  return !rem;
}

int s21_subSupport(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result) {
  int sign = 1;

  s21_getNull(result);
  s21_reverse(&value_2);
  s21_setbit(result, 0);
  s21_addSupport(value_2, *result, &value_2);
  s21_getNull(result);
  if (s21_addSupport(value_1, value_2, result)) {
    s21_getNull(&value_2);
    s21_setbit(&value_2, 0);
    s21_reverse(result);
    s21_addSupport(*result, value_2, result);
    sign = -1;
  }
  if (s21_checkNull(*result)) sign = 0;

  return sign;
}

int s21_MaxDigitPos(s21_decimal a, int pos) {
  while (s21_checkbit(a, pos--) == 0 && pos >= 0) {
  }
  if (pos == -1 && s21_checkbit(a, pos + 1) == 0) pos--;

  return ++pos;
}

//  The function wipes off entire s21_decimal instance
void s21_getNull(s21_decimal *a) {
  for (int i = 0; i <= BYTESCALE; i++) a->bits[i] = 0;
}

int s21_checkNull(s21_decimal a) {
  return !(a.bits[0] || a.bits[1] || a.bits[2]);
}

//  The function returns the value of the bit in s21_decimal instance
int s21_checkbit(s21_decimal a, const unsigned int pos) {
  return ((a.bits[pos / 32] & (1 << pos)) != 0);
}

//  The function sets the bit indexed by `pos` to 1
void s21_setbit(s21_decimal *a, const unsigned int pos) {
  if (pos < 128) a->bits[pos / 32] |= (1 << pos);
}

void s21_switchbit(s21_decimal *a, const unsigned int pos) {
  a->bits[pos / 32] ^= (1 << pos);
}

int s21_checkSign(s21_decimal a) { return s21_checkbit(a, S21_SIGNPLACE); }

void s21_setSign(s21_decimal *a) { s21_setbit(a, S21_SIGNPLACE); }

int s21_getScale(s21_decimal a) {
  a.bits[BYTESCALE] = a.bits[BYTESCALE] << 9;
  a.bits[BYTESCALE] = a.bits[BYTESCALE] >> 25;

  return a.bits[BYTESCALE];
}

void s21_setScale(s21_decimal *a, int scale) {
  int sign = s21_checkSign(*a);

  a->bits[BYTESCALE] = scale << 16;
  if (sign == 1) s21_setSign(a);
}

int s21_shift(s21_decimal *a, int direction) {
  int save[BYTESCALE + 1];
  int flag = 0;

  if (direction > 0) {
    for (int j = 0; j < direction; j++) {
      for (int i = 0; i < BYTESCALE; i++) {
        save[i] = s21_checkbit(*a, MAXBIT + (i * sizeof(int) * CHAR_BIT));
        a->bits[i] <<= 1;
      }
      if (save[BYTESCALE - 1] == 1) flag = 1;
      for (int i = 1; i < BYTESCALE; i++)
        if (save[i - 1] == 1) s21_setbit(a, 0 + (i * sizeof(int) * CHAR_BIT));
    }
  } else if (direction < 0) {
    for (int j = 0; j > direction; j--) {
      for (int i = 0; i < BYTESCALE; i++) {
        save[i] = s21_checkbit(*a, i * sizeof(int) * CHAR_BIT);
        a->bits[i] >>= 1;
      }
      if (save[0] == 1) flag = 1;
      for (int i = BYTESCALE; i--;)
        if (save[i] == 1)
          s21_setbit(a, MAXBIT + ((i - 1) * sizeof(int) * CHAR_BIT));
    }
  }

  return flag;
}

void s21_reverse(s21_decimal *a) {
  for (int i = 0; i <= S21_FIRSTDIGIT; i++) s21_switchbit(a, i);
}

int s21_get_exp(const char *src) {
  int result = 0, base = 1;

  for (int i = 8; i > 0; i--) {
    result += src[i] * base;
    base = base * 2;
  }

  return (result - 127);
}

void s21_float_to_string(float src, char *float_str) {
  unsigned int fbits = *((unsigned int *)&src);

  for (unsigned int mask = 0x80000000; mask; mask >>= 1) {
    *float_str = !!(fbits & mask);
    float_str++;
  }
}

int s21_mulSupport(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result) {
  int flag = 0;
  int sign = 0;
  if (s21_checkSign(value_1) != s21_checkSign(value_2)) sign = 1;
  int scale = s21_getScale(value_1) + s21_getScale(value_2);
  s21_getNull(result);
  s21_ldecimal lv1, lv2, res;
  s21_lgetNull(&res);
  s21_to_ldecimal(value_1, &lv1);
  s21_to_ldecimal(value_2, &lv2);
  int max = s21_lgetMaxDigitPos(lv2, S21_LFD);

  for (int i = 0; i <= max; i++) {
    if (s21_checklbit(lv2, i)) s21_laddSupport(res, lv1, &res);
    s21_lshift(&lv1, 1);
  }
  while (scale > 28) {
    s21_ldiv10(res, &res, 1);
    scale--;
  }
  if (s21_to_decimal(res, scale, result)) flag = 1 + sign;
  if (!s21_checkNull(*result)) s21_setScale(result, scale);
  if (sign) s21_setSign(result);

  return flag;
}

int s21_lmul_10(s21_ldecimal a, s21_ldecimal *rez) {
  int flag;

  if (s21_lgetMaxDigitPos(a, S21_LFD) > S21_LFD - 3) {
    flag = 0;
  } else {
    s21_ldecimal a_1, a_3;
    s21_lshift(&a, 1);
    a_1 = a;
    s21_lshift(&a, 2);
    a_3 = a;
    flag = s21_laddSupport(a_1, a_3, rez);
  }

  return flag;
}

int s21_ldivSupport(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result) {
  int flag = 0;
  int sign = 0;
  if (s21_checkSign(value_1) != s21_checkSign(value_2)) sign = 1;
  int scale = s21_getScale(value_1) - s21_getScale(value_2);
  s21_getNull(result);
  s21_ldecimal lv1, lv2, sup, res;
  s21_lgetNull(&res);
  s21_to_ldecimal(value_1, &lv1);
  s21_to_ldecimal(value_2, &lv2);

  while (scale < 0) {
    s21_lmul_10(lv1, &lv1);
    scale++;
  }
  int max, maxDel;
  maxDel = s21_lgetMaxDigitPos(lv2, S21_LFD);
  while (scale < 28 && !s21_lcheckNull(lv1) && res.bits[6] == 0) {
    s21_ldecimal assistRes;
    s21_lgetNull(&assistRes);
    sup = lv2;
    max = s21_lgetMaxDigitPos(lv1, S21_LFD);
    while (max < maxDel && scale < 28) {
      scale++;
      s21_lmul_10(lv1, &lv1);
      s21_lmul_10(res, &res);
      max = s21_lgetMaxDigitPos(lv1, S21_LFD);
    }
    s21_lshift(&sup, max - maxDel);
    while (!s21_lcheckNull(lv1) && max >= maxDel) {
      if (s21_lis_greater_for_div(lv1, sup)) {
        s21_setlbit(&assistRes, 0);
        s21_lsubSupport(lv1, sup, &lv1);
      } else if (scale < 28 && max == maxDel) {
        s21_lmul_10(lv1, &lv1);
        break;
      }
      s21_lshift(&assistRes, 1);
      s21_lshift(&sup, -1);
      max--;
    }
    s21_decimal fl;
    s21_to_decimal(res, 0, &fl);
    if (s21_mul_10(fl, &fl)) {
      if (!s21_lcheckNull(lv1)) scale++;
      s21_to_ldecimal(fl, &res);
      s21_laddSupport(assistRes, res, &res);
    } else {
      scale--;
      break;
    }
  }
  while (max > maxDel) {
    s21_lshift(&res, 1);
    max--;
  }
  if (s21_to_decimal(res, scale, result)) flag = 1 + sign;
  if (!s21_checkNull(*result)) s21_setScale(result, scale);
  if (sign) s21_setSign(result);

  return flag;
}

void s21_ldiv10(s21_ldecimal a, s21_ldecimal *res, int flag) {
  s21_lgetNull(res);
  s21_ldecimal decade = {{10, 0, 0, 0, 0, 0, 0}};
  s21_ldecimal sup = decade;
  int max = s21_lgetMaxDigitPos(a, S21_LFD);
  int maxDel = s21_lgetMaxDigitPos(decade, S21_LFD);

  s21_lshift(&decade, max - maxDel);
  while (!s21_lcheckNull(a) && max >= maxDel) {
    if (s21_lis_greater_for_div(a, decade)) {
      s21_setlbit(res, 0);
      s21_lsubSupport(a, decade, &a);
    }
    s21_lshift(res, 1);
    s21_lshift(&decade, -1);
    max--;
  }
  while (max > maxDel) {
    s21_lshift(res, 1);
    max--;
  }
  s21_ldecimal one = {{1, 0, 0, 0, 0, 0, 0}};
  s21_lshift(&sup, -1);
  if (s21_lis_greater_for_div(a, decade) && flag)
    s21_laddSupport(*res, one, res);
}

int s21_lsubSupport(s21_ldecimal value_1, s21_ldecimal value_2,
                    s21_ldecimal *result) {
  int sign = 1;

  s21_lgetNull(result);
  s21_lreverse(&value_2);
  s21_setlbit(result, 0);
  s21_laddSupport(value_2, *result, &value_2);
  s21_lgetNull(result);
  if (s21_laddSupport(value_1, value_2, result)) {
    s21_lgetNull(&value_2);
    s21_setlbit(&value_2, 0);
    s21_lreverse(result);
    s21_laddSupport(*result, value_2, result);
    sign = -1;
  }
  if (s21_lcheckNull(*result)) sign = 0;

  return sign;
}

int s21_lcheckNull(s21_ldecimal a) {
  return !(a.bits[0] || a.bits[1] || a.bits[2] || a.bits[3] || a.bits[4] ||
           a.bits[5] || a.bits[6]);
}

void s21_lreverse(s21_ldecimal *a) {
  for (int i = 0; i <= S21_LFD; i++) s21_switchlbit(a, i);
}

void s21_switchlbit(s21_ldecimal *a, const unsigned int pos) {
  a->bits[pos / 32] ^= (1 << pos);
}

int s21_lis_greater_for_div(s21_ldecimal a, s21_ldecimal b) {
  int res = 0;

  if (s21_lsubSupport(a, b, &b) != -1) res = 1;

  return res;
}

int s21_laddSupport(s21_ldecimal value_1, s21_ldecimal value_2,
                    s21_ldecimal *result) {
  s21_lgetNull(result);
  int rem = 0;

  for (int i = 0; i <= S21_LFD; i++) {
    if (s21_checklbit(value_1, i) == 1 && s21_checklbit(value_2, i) == 1) {
      if (rem) s21_setlbit(result, i);
      rem = 1;
    } else if (s21_checklbit(value_1, i) == 0 &&
               s21_checklbit(value_2, i) == 0) {
      if (rem) s21_setlbit(result, i);
      rem = 0;
    } else {
      if (!rem) s21_setlbit(result, i);
    }
  }

  return !rem;
}

int s21_lshift(s21_ldecimal *value, int direction) {
  int save[LBYTESCALE + 1];
  int flag = 0;

  if (direction > 0) {
    for (int j = 0; j < direction; j++) {
      for (int i = 0; i <= LBYTESCALE; i++) {
        save[i] = s21_checklbit(*value, MAXBIT + (i * S21INT));
        value->bits[i] <<= 1;
      }
      if (save[LBYTESCALE] == 1) flag = 1;
      for (int i = 1; i <= LBYTESCALE; i++)
        if (save[i - 1] == 1) s21_setlbit(value, 0 + (i * S21INT));
    }
  } else if (direction < 0) {
    for (int j = 0; j > direction; j--) {
      for (int i = 0; i <= LBYTESCALE; i++) {
        save[i] = s21_checklbit(*value, i * S21INT);
        value->bits[i] >>= 1;
      }
      if (save[0] == 1) flag = 1;
      for (int i = LBYTESCALE; i > 0; i--)
        if (save[i] == 1) s21_setlbit(value, MAXBIT + ((i - 1) * S21INT));
    }
  }

  return flag;
}

void s21_to_ldecimal(s21_decimal value, s21_ldecimal *res) {
  s21_lgetNull(res);
  for (int i = 0; i < BYTESCALE; i++) res->bits[i] = value.bits[i];
}

int s21_to_decimal(s21_ldecimal value, int scale, s21_decimal *res) {
  int flag = 0;

  s21_getNull(res);
  if (scale == 0 &&
      (value.bits[3] || value.bits[4] || value.bits[5] || value.bits[6])) {
    flag = 1;
  } else {
    while (scale > 0 &&
           (value.bits[3] || value.bits[4] || value.bits[5] || value.bits[6])) {
      s21_ldiv10(value, &value, 1);
      scale--;
    }
  }
  if (scale == 0 &&
      (value.bits[3] || value.bits[4] || value.bits[5] || value.bits[6])) {
    flag = 1;
  } else {
    for (int i = 0; i < BYTESCALE; i++) res->bits[i] = value.bits[i];
  }

  return flag;
}

void s21_lgetNull(s21_ldecimal *value) {
  for (int i = 0; i <= LBYTESCALE; i++) value->bits[i] = 0;
}

int s21_lgetMaxDigitPos(s21_ldecimal value, int pos) {
  while (s21_checklbit(value, pos--) == 0 && pos >= 0) {
  }
  if (pos == -1 && s21_checklbit(value, pos + 1) == 0) pos--;

  return ++pos;
}

int s21_checklbit(s21_ldecimal value, const unsigned int pos) {
  return ((value.bits[pos / 32] & (1 << pos)) != 0);
}

void s21_setlbit(s21_ldecimal *value, const unsigned int pos) {
  if (pos < 224) value->bits[pos / 32] |= (1 << pos);
}
