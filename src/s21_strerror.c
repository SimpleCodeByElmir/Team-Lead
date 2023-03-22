#include "s21_string.h"

char *s21_strerror(int errnum) {
    int errnum_min;
    int errnum_max;
    char *err_msg_unknown;

#ifdef __APPLE__
    errnum_min = 1;
    errnum_max = ERR_MSGS_COUNT;
    err_msg_unknown = "Unknown error: ";
#else
    errnum_min = 0;
    errnum_max = ERR_MSGS_COUNT - 1;
    err_msg_unknown = "Unknown error ";
#endif

    if (errnum < errnum_min || errnum > errnum_max) {
        static char unknown_str[50];
#if __APPLE__
        if (errnum == 0) {
            err_msg_unknown = "Undefined error: ";
        }
#endif
        s21_sprintf(unknown_str, "%s%d", err_msg_unknown, errnum);
#ifdef __MUSL__
        s21_sprintf(unknown_str, "No error information");
#endif
        return unknown_str;
    }

#ifdef __APPLE__
    errnum -= 1;
#endif


    static const char *strerr[] = ERR_MSGS;
    return (char*)(strerr[errnum]);
}
