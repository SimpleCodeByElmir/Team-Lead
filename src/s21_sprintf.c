#include "s21_string.h"

#include <math.h>
#include <float.h>


int s21_sprintf(char *target, const char *format, ...) {
    const char *target_saved = target;
    va_list args;
    va_start(args, format);

    int is_token = 0;
    while (*format) {
        if (s21_frmt_is_tokn(format) == 1) {
            is_token = 1;
            format++;
       }
        if (is_token == 0) {
            *target = *format;
            target++;
            format++;
        }
        if (is_token == 1) {
            const char *token = format;
            char specif = s21_tokn_get_specif(token);

            int printed = 0;
            if (specif == SPECIFS[0]
                || specif == SPECIFS[15]) {
                printed = s21_trgt_print_tokn_char(target, token, &args);
            }
            if (specif == SPECIFS[1]
                  || specif == SPECIFS[2]
                  || specif == SPECIFS[8]
                  || specif == SPECIFS[10]
                  || specif == SPECIFS[11]
                  || specif == SPECIFS[12]) {
                printed = s21_trgt_print_tokn_num(target, token, &args);
            }
            if (specif == SPECIFS[3]
                  || specif == SPECIFS[4]
                  || specif == SPECIFS[5]
                  || specif == SPECIFS[6]
                  || specif == SPECIFS[7]) {
                printed = s21_trgt_print_tokn_ratio(target, token, &args);
            }
            if (specif == SPECIFS[9]) {
                printed = s21_trgt_print_tokn_str(target, token, &args);
            }
            if (specif == SPECIFS[13]) {
                printed = s21_trgt_print_tokn_ptr(target, token, &args);
            }
            if (specif == SPECIFS[14]) {
                printed = 0;
                int *to_write_here = va_arg(args, int*);
                *to_write_here = target - target_saved;
            }
            target += printed;

            format += s21_tokn_get_str_len(token);
            is_token = 0;
        }
    }
    *target = '\0';

    va_end(args);
    return target - target_saved;
}

char *s21_tokn_skip_part(const char *token, unsigned int i) {
    if (i--)    /* flags */
        while (*token && s21_strchr(FLAGS, *token))
            token++;
    if (i--) {  /* width */
        if (*token == ADDIT_INT_SIGN)
            token++;
        else
            while (*token && s21_strchr(DIGITS, *token))
                token++;
    }
    if (i--) {  /* precision */
        if (*token == PRECIS_SIGN) {
            token++;
            if (*token == ADDIT_INT_SIGN)
                token++;
            else
                while (*token && s21_strchr(DIGITS, *token))
                    token++;
        }
    }
    if (i--)    /* length-specifier */
        if (*token && s21_strchr(TOKN_LENS, *token))
            token++;
    if (i)    /* specifier */
        if (*token && s21_strchr(SPECIFS, *token))
            token++;
    return (char *)token;
}

int s21_tokn_have_flag(const char *token, char flag) {
    int result = 0;
    while (s21_strchr(FLAGS, *token)) {
        if (*token == flag) {
            result = 1;
            break;
        }
        token++;
    }
    return result;
}

int s21_tokn_get_width(const char *token) {
    token = s21_tokn_skip_part(token, 1);
    int result = -1;
    if (*token == ADDIT_INT_SIGN) {
        result = -2;
    }
    if (s21_atol(token)) {
        result = s21_atol(token);
    }
    return result;
}

int s21_tokn_get_precision(const char *token) {
    token = s21_tokn_skip_part(token, 2);
    int result = -1;
    if (*token == PRECIS_SIGN) {
        token++;
        result = -3;
        if (*token == ADDIT_INT_SIGN) {
            result = -2;
        }
        if (s21_atol(token)) {
            result = s21_atol(token);
        } else if (*token == '0') {
            /*  .. == '0' --- because s21_atol() returns 0 as fail, we should
             *      check for 0 value additionally
             */
            result = 0;
        }
    }
    return result;
}

char s21_tokn_get_len(const char *token) {
    char result = '\0';
    token = s21_tokn_skip_part(token, 3);
    if (s21_strchr(TOKN_LENS, *token)) {
        result = *token;
    }
    return result;
}

char s21_tokn_get_specif(const char *token) {
    char result = '\0';
    token = s21_tokn_skip_part(token, 4);
    if (*token && s21_strchr(SPECIFS, *token)) {
        result = *token;
    }
    return result;
}

int s21_tokn_get_str_len(const char *token) {
    return s21_tokn_skip_part(token, 5) - token;
}

int s21_frmt_is_tokn(const char *format) {
    int result = 0;
    if (*format == TOKN_SIGN) {
        format++;
        if (s21_tokn_get_specif(format)) {
            result = 1;
        }
    }
    return result;
}

int s21_trgt_print_ulong(char *target, unsigned long n) {
    const char *target_saved = target;

    int n_len = s21_udecim_get_str_len(n);
    while (n_len > 0) {
        unsigned long divisor = s21_ulong_get_pow(10, n_len - 1);
        unsigned long curr_d = n / divisor;
        char curr_c = '0' + curr_d;
        *target = curr_c;
        target++;
        n %= divisor;
        n_len--;
    }

    return target - target_saved;
}

int s21_trgt_print_uldouble(char *target, long double ld, int precis_len) {
    const char *target_saved = target;

    if (isnan(ld)) {
        s21_memcpy(target, "nan", 3);
        return 3;
    }
    if (ld == INFINITY) {
        s21_memcpy(target, "inf", 3);
        return 3;
    }

    unsigned long decim_part = (long)ld;
    target += s21_trgt_print_ulong(target, decim_part);
    ld -= decim_part;

    if (precis_len == -1) {
        /*  -1 --- indicates flag '#'   */
        *target = '.';
        target++;
    }
    if (precis_len > 0) {
        *target = '.';
        target++;
        long double after_part;
        after_part = ld * s21_ulong_get_pow(10, precis_len);
        after_part = round(after_part);
        int after_part_len = s21_udecim_get_str_len((unsigned long)after_part);
        int zeros_count = precis_len - after_part_len;
        if (zeros_count < 0) {
            zeros_count = 0;
        }
        s21_memset(target, '0', zeros_count);
        target += zeros_count;
        target += s21_trgt_print_ulong(target, (unsigned long)after_part);
    }

    return target - target_saved;
}

int s21_trgt_print_e_uldouble(char *target, long double ld, int precis_len,
        char e_sign) {
    const char *target_saved = target;

    int e = 0;
    ld += LDBL_EPSILON;
    while (ld < 1) {
        e--;
        ld *= 10;
    }
    while (ld > 10) {
        e++;
        ld /= 10;
    }

    target += s21_trgt_print_uldouble(target, ld, precis_len);

    *target = e_sign;
    target++;
    if (e < 0) {
        *target = '-';
        target++;
        e = -e;
    } else {
        *target = '+';
        target++;
    }
    *target = '0' + e / 10;
    target++;
    *target = '0' + e % 10;
    target++;

    return target - target_saved;
}

int s21_trgt_print_base_ulong(char *target, unsigned long n,
        const char *base) {
    const char *target_saved = target;

    int base_n = s21_strlen(base);
    if (n / base_n) {
        target += s21_trgt_print_base_ulong(target, n / base_n, base);
    }
    *target = base[n % base_n];
    target++;

    return target - target_saved;
}

int s21_trgt_print_tokn_char(char *target, const char *token, va_list *pargs) {
    const char *target_saved = target;

    int is_prequel = s21_tokn_have_flag(token, FLAGS[0]);

    int width = s21_tokn_get_width(token);
    if (width == -2) {
        width = va_arg(*pargs, int);
    }
    if (width < 0) {
        is_prequel = 1;
        width = -width;
    }

    int fill_len = width - 1;
    if (fill_len < 0) {
        fill_len = 0;
    }

    int precis_len = s21_tokn_get_precision(token);
    if (precis_len == -1) {
        precis_len = 6;
    }
    if (precis_len == -2) {
        precis_len = va_arg(*pargs, int);
    }
    if (precis_len < 0) {
        precis_len = 0;
    }

    char tokn_specif = s21_tokn_get_specif(token);

    char tokn_c = '\0';
    if (tokn_specif == SPECIFS[0]) {
        tokn_c = (char)va_arg(*pargs, int);
    }
    if (tokn_specif == SPECIFS[15]) {
        tokn_c = TOKN_SIGN;
    }

    char fill_symb = ' ';
    if (s21_tokn_have_flag(token, FLAGS[4]) && is_prequel == 0) {
        fill_symb = '0';
    }

#ifdef __APPLE__
    if (is_prequel == 0) {
        s21_memset(target, fill_symb, fill_len);
        target += fill_len;
    }
    *target = tokn_c;
    target++;
    if (is_prequel) {
        s21_memset(target, fill_symb, fill_len);
        target += fill_len;
    }
#else  // For GNU/Linux (that uses glibc)
    if (tokn_specif == SPECIFS[0]) {
        if (is_prequel == 0) {
            s21_memset(target, fill_symb, fill_len);
            target += fill_len;
        }
        *target = tokn_c;
        target++;
        if (is_prequel) {
            s21_memset(target, fill_symb, fill_len);
            target += fill_len;
        }
    }
    if (tokn_specif == SPECIFS[15]) {
        *target = tokn_c;
        target++;
    }
#endif

    return target - target_saved;
}

int s21_trgt_print_tokn_num(char *target, const char *token, va_list *pargs) {
    const char *target_saved = target;

    int is_prequel = s21_tokn_have_flag(token, FLAGS[0]);

    int width = s21_tokn_get_width(token);
    if (width == -2) {
        width = va_arg(*pargs, int);
    }
    if (width == -1) {
        width = 0;
    }
    if (width < 0) {
        is_prequel = 1;
        width = -width;
    }

    int precis = s21_tokn_get_precision(token);
    int is_precis_provided = 1;
    if (precis == -3) {
        /* -3 --- it means that PRECIS_SIGN was provided */
        precis = 0;
    }
    if (precis == -1) {
        /* -1 --- it means that PRECIS_SIGN was not provided */
        precis = 0;
        is_precis_provided = 0;
    }
    if (precis == -2) {
        precis = va_arg(*pargs, int);
    }
    /*  precis --- works here as minimum digits counter to output */

    long tokn_num = va_arg(*pargs, long);

    if (s21_tokn_get_specif(token) == SPECIFS[1]
            || s21_tokn_get_specif(token) == SPECIFS[2]) {
        tokn_num = (int)tokn_num;
    }
    if (s21_tokn_get_len(token) == TOKN_LENS[0]) {
        tokn_num = (short)tokn_num;
    }

    char sign = '\0';
    if (s21_tokn_have_flag(token, FLAGS[1])) {
        sign = '+';
    }
    if (tokn_num < 0 && s21_tokn_get_specif(token) != SPECIFS[10]) {
        sign = '-';
    }
    if (sign == '\0' && s21_tokn_have_flag(token, FLAGS[2])) {
        sign = ' ';
    }

    char *base = DIGITS;
    char *prefix_0x = s21_NULL;
    if (s21_tokn_get_specif(token) == SPECIFS[8]) {
        base = BASE8;
        prefix_0x = "0";
    }
    if (s21_tokn_get_specif(token) == SPECIFS[11]) {
        base = BASE16LOW;
        prefix_0x = "0x";
    }
    if (s21_tokn_get_specif(token) == SPECIFS[12]) {
        base = BASE16UP;
        prefix_0x = "0X";
    }
    if (s21_tokn_have_flag(token, FLAGS[3]) == 0
        || tokn_num == 0) {
        prefix_0x = s21_NULL;
    }

    unsigned long tokn_unum;
    if (s21_tokn_get_specif(token) == SPECIFS[1]
        || s21_tokn_get_specif(token) == SPECIFS[2]) {
        tokn_unum = tokn_num >= 0 ? tokn_num : -tokn_num;
    } else {
        tokn_unum = tokn_num;
    }

    int tokn_unum_len = s21_base_unum_get_str_len(tokn_unum, base);
    if (is_precis_provided && precis == 0 && tokn_unum == 0) {
        tokn_unum_len = 0;
    }

    int precis_prefix_len = precis - tokn_unum_len;
    if (precis_prefix_len < 0) {
        precis_prefix_len = 0;
    }

    int fill_len = 0;
    if (width) {
        fill_len = width - precis_prefix_len - tokn_unum_len;
    }
    if (sign) {
        fill_len--;
    }
    if (prefix_0x) {
        fill_len -= s21_strlen(prefix_0x);
    }
    if (fill_len < 0) {
        fill_len = 0;
    }

    char fill_symb = ' ';
    if (s21_tokn_have_flag(token, FLAGS[4]) && is_prequel == 0) {
        fill_symb = '0';
    }
    if (precis_prefix_len || precis > 0) {
        fill_symb = ' ';
    }

    if (is_prequel == 0) {
        if (fill_symb == '0' && sign) {
            *target = sign;
            target++;
        }
        if (fill_symb == ' ' || prefix_0x == s21_NULL) {
            s21_memset(target, fill_symb, fill_len);
            target += fill_len;
        }
    }
    if (fill_symb == ' ' && sign) {
        *target = sign;
        target++;
    }
    if (prefix_0x) {
        s21_strcpy(target, prefix_0x);
        target += s21_strlen(prefix_0x);
        if (fill_symb == '0') {
            s21_memset(target, fill_symb, fill_len);
            target += fill_len;
        }
    }
    if (precis_prefix_len) {
        s21_memset(target, '0', precis_prefix_len);
        target += precis_prefix_len;
    }
    if ((is_precis_provided && precis == 0 && tokn_unum == 0) == 0) {
        /*  precis == 0 && tokn_unum == 0 --- because we should not print
         *      value 0 if provided precis is 0
         */
        int printed = s21_trgt_print_base_ulong(target, tokn_unum, base);
        target+= printed;
    }
    if (is_prequel) {
        s21_memset(target, fill_symb, fill_len);
        target += fill_len;
    }

    return target - target_saved;
}

int s21_trgt_print_tokn_str(char *target, const char *token, va_list *pargs) {
    const char *target_saved = target;

    int is_prequel = s21_tokn_have_flag(token, FLAGS[0]);

    int tokn_width = s21_tokn_get_width(token);
    if (tokn_width == -2) {
        tokn_width = va_arg(*pargs, int);
    }
    if (tokn_width == -1) {
        tokn_width = 0;
    }
    if (tokn_width < 0) {
        is_prequel = 1;
        tokn_width = -tokn_width;
    }

    int tokn_precis = s21_tokn_get_precision(token);
    if (tokn_precis == -2) {
        tokn_precis = va_arg(*pargs, int);
    }
    /*  tokn_precis --- works here as str len   */

    char *tokn_str = va_arg(*pargs, char *);
    if (tokn_precis == -1 || tokn_precis > (int)s21_strlen(tokn_str)) {
        tokn_precis = s21_strlen(tokn_str);
    }
    if (tokn_precis < 0) {
        tokn_precis = 0;
    }

    int fill_len = tokn_width - tokn_precis;
    if (fill_len < 0) {
        fill_len = 0;
    }

    if (is_prequel == 0) {
        s21_memset(target, ' ', fill_len);
        target += fill_len;
    }

    s21_strncpy(target, tokn_str, tokn_precis);
    target += tokn_precis;

    if (is_prequel) {
        s21_memset(target, ' ', fill_len);
        target += fill_len;
    }

    return target - target_saved;
}

int s21_trgt_print_tokn_ratio(char *target, const char *token, va_list *pargs) {
    const char *target_saved = target;

    char tokn_specif = s21_tokn_get_specif(token);

    int is_prequel = s21_tokn_have_flag(token, FLAGS[0]);

    int width = s21_tokn_get_width(token);
    if (width == -2) {
        width = va_arg(*pargs, int);
    }
    if (width < 0) {
        is_prequel = 1;
        width = -width;
    }

    int precis_len = s21_tokn_get_precision(token);
    if (precis_len == -1) {
        precis_len = 6;
    }
    if (precis_len == -2) {
        precis_len = va_arg(*pargs, int);
    }
    if (precis_len <= -3) {
        precis_len = 6;
    }
    if (precis_len == 0
            && (tokn_specif == SPECIFS[6] || tokn_specif == SPECIFS[7])) {
        precis_len = 1;
    }

    long double tokn_ratio;
    if (s21_tokn_get_len(token) == TOKN_LENS[2]) {
        tokn_ratio = va_arg(*pargs, long double);
    } else {
        tokn_ratio = va_arg(*pargs, double);
    }

    char sign = '\0';
    if (tokn_ratio >= 0 && s21_tokn_have_flag(token, FLAGS[1])) {
        sign = '+';
    }
    if (tokn_ratio < 0) {
        sign = '-';
        tokn_ratio = -tokn_ratio;
    }

    int is_e_shorter = 0;
    int non_precis_part_len = s21_udecim_get_str_len((unsigned long)tokn_ratio);
    if (tokn_specif == SPECIFS[3]
            || tokn_specif == SPECIFS[4]) {
        non_precis_part_len = 5;
        /*  Числа, наподобие 1.54+e01, имеют 1 символ до запятой и 4 символа
         *      после дробной части
         */
    } else if (tokn_specif == SPECIFS[6]
               || tokn_specif == SPECIFS[7]) {
        if (s21_tokn_have_flag(token, FLAGS[3])) {
            int e = s21_e_uratio_get_e(tokn_ratio);
            e++;
            /*  e++ --- because in "0.052", e will have value of -2, but we
             *      need to substract only 1 from precis_len */
            precis_len = precis_len - e;
            if (precis_len < 0) {
                precis_len = 0;
            }
        } else {
            int actual_precis_len = s21_uratio_precis_get_str_len(tokn_ratio,
                    precis_len);
            int actual_e_precis_len = s21_e_uratio_precis_get_str_len(
                    tokn_ratio, precis_len);

            char buff[500];

            int len1 = s21_trgt_print_uldouble(buff, tokn_ratio,
                    actual_precis_len);
            int len2 = s21_trgt_print_e_uldouble(buff, tokn_ratio,
                    actual_e_precis_len, 'e');
            if (len2 < len1) {
                is_e_shorter = 1;
                precis_len = actual_e_precis_len;
                non_precis_part_len = 5;
                /*  Числа, наподобие 1.54+e01, имеют 1 символ до запятой и 4
                 *      символа после дробной части
                 */
            } else {
                int e = s21_e_uratio_get_e(tokn_ratio);
                precis_len = precis_len - e - 1;
                precis_len = s21_uratio_precis_get_str_len(tokn_ratio,
                        precis_len);
                /*  If you have questions here, read
                 *  https://stackoverflow.com/questions/30658919
                 *      /the-precision-of-printf-with-specifier-g
                 */
            }
        }
    }

    int is_point_forced = s21_tokn_have_flag(token, FLAGS[3]);

    int is_pre_whitespace = 0;
    if (sign == '\0' && s21_tokn_have_flag(token, FLAGS[2])) {
        is_pre_whitespace = 1;
    }

    int fill_len = width - precis_len - non_precis_part_len;
    if (is_pre_whitespace) {
        fill_len--;
    }
    if (precis_len || is_point_forced) {
        /* For a common */
        fill_len--;
    }
    if (sign) {
        fill_len--;
    }
    if (fill_len < 0) {
        fill_len = 0;
    }

    char fill_symb = ' ';
    if (s21_tokn_have_flag(token, FLAGS[4]) && is_prequel == 0) {
        fill_symb = '0';
    }

    char e_sign = '\0';
    if (tokn_specif == SPECIFS[3]
        || tokn_specif == SPECIFS[6]) {
        e_sign = 'e';
    }
    if (tokn_specif == SPECIFS[4]
          || tokn_specif == SPECIFS[7]) {
        e_sign = 'E';
    }

    if (precis_len == 0 && is_point_forced) {
        precis_len = -1;
        /*  -1 --- indicates flag '#'   */
    }

    if (is_pre_whitespace) {
        *target = ' ';
        target++;
    }
    if (is_prequel == 0) {
        if (fill_symb == '0' && sign) {
            *target = sign;
            target++;
        }
        s21_memset(target, fill_symb, fill_len);
        target += fill_len;
    }
    if (fill_symb == ' ' && sign) {
        *target = sign;
        target++;
    }
    if (tokn_specif == SPECIFS[5]) {
        target += s21_trgt_print_uldouble(target, tokn_ratio, precis_len);
    } else if (tokn_specif == SPECIFS[3]
        || tokn_specif == SPECIFS[4]) {
        target += s21_trgt_print_e_uldouble(target, tokn_ratio, precis_len,
                    e_sign);

    } else if (tokn_specif == SPECIFS[6]
          || tokn_specif == SPECIFS[7]) {
        if (is_e_shorter) {
            target += s21_trgt_print_e_uldouble(target, tokn_ratio, precis_len,
                        e_sign);
        } else {
            target += s21_trgt_print_uldouble(target, tokn_ratio, precis_len);
        }
    }
    if (is_prequel) {
        s21_memset(target, fill_symb, fill_len);
        target += fill_len;
    }

    return target - target_saved;
}

int s21_trgt_print_tokn_ptr(char *target, const char *token, va_list *pargs) {
    const char *target_saved = target;

    int is_prequel = s21_tokn_have_flag(token, FLAGS[0]);

    int width = s21_tokn_get_width(token);
    if (width == -2) {
        width = va_arg(*pargs, int);
    }
    if (width == -1) {
        width = 0;
    }
    if (width < 0) {
        is_prequel = 1;
        width = -width;
    }

    int precis = s21_tokn_get_precision(token);
    if (precis == -3) {
        precis = 0;
    }
    if (precis == -2) {
        precis = va_arg(*pargs, int);
    }
    if (precis == -1) {
        precis = 0;
    }
    if (precis < 0) {
        precis = 0;
    }

    s21_size_t p = va_arg(*pargs, s21_size_t);
    /*  s21_size_t --- it's the same as pointer */

    int is_null = 0;
    if (p == 0) {
        is_null = 1;
#ifdef __APPLE__
        precis = 1;
        /*  .. = 1 --- because apple's libc printf 0x0 in NULL case. 1 here is
         *      for end 0 */
#endif
    }

    int p_len = s21_base_unum_get_str_len(p, BASE16LOW);
    if (is_null) {
        p_len = 0;
    }

    int zero_prefix_len = PTR_LEN - p_len;
    if (zero_prefix_len < 0) {
        zero_prefix_len = 0;
    }
#if defined(__APPLE__) || defined(__MUSL__)
    zero_prefix_len = 0;
    /*  ... = 0 --- apple's libc and musl doesn't print prefix extra zeros on
     *      pointers like 0x00052ab3f1c9b. They print just 0x52ab3f1c9b. */
#endif

    int fill_len = width - 2 - zero_prefix_len - p_len;
    /*  ... - 2 --- because "0x" prefix has length of 2 */
    if (is_null) {
#ifndef __APPLE__  // If NOT __APPLE__
        fill_len = width - s21_strlen(PTR_NULL_STR);
#endif

        fill_len -= precis;
    }
    if (fill_len < 0) {
        fill_len = 0;
    }

    if (is_prequel == 0) {
        s21_memset(target, ' ', fill_len);
        target += fill_len;
    }
    if (is_null) {
        s21_strcpy(target, PTR_NULL_STR);
        target += s21_strlen(PTR_NULL_STR);
#ifdef __APPLE__
        *target++ = '0';
        *target++ = 'x';
        s21_memset(target, '0', precis);
        target += precis;
#endif
    } else {
        *target++ = '0';
        *target++ = 'x';
        s21_memset(target, '0', zero_prefix_len);
        target += zero_prefix_len;
        target += s21_trgt_print_base_ulong(target, p, BASE16LOW);
    }
    if (is_prequel) {
        s21_memset(target, ' ', fill_len);
        target += fill_len;
    }

    return target - target_saved;
}

int s21_base_unum_get_str_len(unsigned long n, char *base) {
    int result = 0;
    while (n >= (unsigned long)s21_strlen(base)) {
        result++;
        n /= (unsigned long)s21_strlen(base);
    }
    result++;
    return result;
}

int s21_udecim_get_str_len(unsigned long n) {
    return s21_base_unum_get_str_len(n, DIGITS);
}

unsigned long s21_ulong_get_pow(unsigned long n, int pow) {
    unsigned long result = 0;
    if (pow == 0) {
        result = 1;
    }
    if (pow) {
        result = n;
        pow--;
    }
    while (pow) {
        result *= n;
        pow--;
    }
    return result;
}

long s21_atol(const char *str) {
    long result = 0;
    int is_minus = 0;
    if (*str == '-') {
        is_minus = 1;
        str++;
    }
    while (*str && s21_strchr(DIGITS, *str)) {
        result *= 10;
        result += *str - '0';
        str++;
    }
    return is_minus ? -result : result;
}

int s21_uratio_precis_get_str_len(long double ld, int precis_len) {
    ld -= (long)ld;

    char buff[500];
    s21_trgt_print_uldouble(buff, ld, precis_len);

    int i = precis_len - 1;
    while (buff[2 + i] == '0') {
        i--;
    }
    /*  2 --- чтобы пропустить часть "0." */

    return i + 1;
}

int s21_e_uratio_precis_get_str_len(long double ld, int precis_len) {
    int e = 0;
    ld += LDBL_EPSILON;
    while (ld < 1) {
        e--;
        ld *= 10;
    }
    while (ld > 10) {
        e++;
        ld /= 10;
    }
    return s21_uratio_precis_get_str_len(ld, precis_len);
}

int s21_e_uratio_get_e(long double ld) {
    int e = 0;
    while (ld < 1) {
        e--;
        ld *= 10;
    }
    while (ld > 10) {
        e++;
        ld /= 10;
    }
    return e;
}
