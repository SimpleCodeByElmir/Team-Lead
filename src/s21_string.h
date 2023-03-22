#ifndef SRC_S21_STRING_H_
#define SRC_S21_STRING_H_

#ifndef __APPLE__
    #ifndef __GLIBC__
        #define __MUSL__
    #endif
#endif

#include <stdarg.h>

typedef unsigned long s21_size_t;
#define s21_NULL (void*)0


/* Порядок сохранён с задания. Всего 20 основных функций. НЕ МЕНЯТЬ */
void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memmove(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strcat(char *dest, const char *src);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char* s21_strchr(const char *str, int c);
int s21_strcmp(const char *str1, const char *str2);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strcpy(char *dest, const char *src);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
s21_size_t s21_strspn(const char *str1, const char *str2);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
void * s21_trim(const char *src, const char *trim_chars);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);

/* Для s21_sprintf() */

int s21_sprintf(char *target, const char *format, ...);

#define TOKN_SIGN '%'
#define SPECIFS "cdieEfgGosuxXpn%"
#define TOKN_LENS "hlL"
#define FLAGS "-+ #0"
#define PRECIS_SIGN '.'
#define ADDIT_INT_SIGN '*'
#define DIGITS "0123456789"
#define BASE16LOW "0123456789abcdef"
#define BASE16UP "0123456789ABCDEF"
#define BASE2 "01"
#define BASE8 "01234567"
#define PTR_LEN 12
#define PTR_LEN_WITH_0X 14
#ifdef __APPLE__
    #define PTR_NULL_STR ""
#else
    #define PTR_NULL_STR "(nil)"
#endif

int s21_frmt_is_tokn(const char *format);
char *s21_tokn_skip_part(const char *token, unsigned int i);
int s21_tokn_have_flag(const char *token, char flag);
int s21_tokn_get_width(const char *token);
int s21_tokn_get_precision(const char *token);
int s21_tokn_get_str_len(const char *token);
char s21_tokn_get_len(const char *token);
char s21_tokn_get_specif(const char *token);
int s21_trgt_print_ulong(char *target, unsigned long n);
int s21_trgt_print_uldouble(char *target, long double ld, int precis_len);
int s21_trgt_print_e_uldouble(char *target, long double ld, int precis_len,
        char e_sign);
int s21_trgt_print_base_ulong(char *target, unsigned long n,
        const char *base);
int s21_trgt_print_tokn_char(char *target, const char *token, va_list *pargs);
int s21_trgt_print_tokn_num(char *target, const char *token, va_list *pargs);
int s21_trgt_print_tokn_str(char *target, const char *token, va_list *pargs);
int s21_trgt_print_tokn_ratio(char *target, const char *token, va_list *pargs);
int s21_trgt_print_tokn_ptr(char *target, const char *token, va_list *pargs);
int s21_base_unum_get_str_len(unsigned long n, char *base);
int s21_udecim_get_str_len(unsigned long n);
unsigned long s21_ulong_get_pow(unsigned long n, int pow);
        long s21_atol(const char *str);
int s21_uratio_precis_get_str_len(long double ld, int precis_len);
int s21_e_uratio_precis_get_str_len(long double ld, int precis_len);
int s21_e_uratio_get_e(long double ld);


/* sscanf */
int s21_sscanf(const char *stra, const char *format, ...);


/* Для s21_strerror() */

#ifdef __APPLE__

#define ERR_MSGS_COUNT 106
#define ERR_MSG_UNKNOWN "Unknown error: "
#define ERR_MSGS { "Operation not permitted", \
"No such file or directory", \
"No such process", \
"Interrupted system call", \
"Input/output error", \
"Device not configured", \
"Argument list too long", \
"Exec format error", \
"Bad file descriptor", \
"No child processes", \
"Resource deadlock avoided", \
"Cannot allocate memory", \
"Permission denied", \
"Bad address", \
"Block device required", \
"Resource busy", \
"File exists", \
"Cross-device link", \
"Operation not supported by device", \
"Not a directory", \
"Is a directory", \
"Invalid argument", \
"Too many open files in system", \
"Too many open files", \
"Inappropriate ioctl for device", \
"Text file busy", \
"File too large", \
"No space left on device", \
"Illegal seek", \
"Read-only file system", \
"Too many links", \
"Broken pipe", \
"Numerical argument out of domain", \
"Result too large", \
"Resource temporarily unavailable", \
"Operation now in progress", \
"Operation already in progress", \
"Socket operation on non-socket", \
"Destination address required", \
"Message too long", \
"Protocol wrong type for socket", \
"Protocol not available", \
"Protocol not supported", \
"Socket type not supported", \
"Operation not supported", \
"Protocol family not supported", \
"Address family not supported by protocol family", \
"Address already in use", \
"Can't assign requested address", \
"Network is down", \
"Network is unreachable", \
"Network dropped connection on reset", \
"Software caused connection abort", \
"Connection reset by peer", \
"No buffer space available", \
"Socket is already connected", \
"Socket is not connected", \
"Can't send after socket shutdown", \
"Too many references: can't splice", \
"Operation timed out", \
"Connection refused", \
"Too many levels of symbolic links", \
"File name too long", \
"Host is down", \
"No route to host", \
"Directory not empty", \
"Too many processes", \
"Too many users", \
"Disc quota exceeded", \
"Stale NFS file handle", \
"Too many levels of remote in path", \
"RPC struct is bad", \
"RPC version wrong", \
"RPC prog. not avail", \
"Program version wrong", \
"Bad procedure for program", \
"No locks available", \
"Function not implemented", \
"Inappropriate file type or format", \
"Authentication error", \
"Need authenticator", \
"Device power is off", \
"Device error", \
"Value too large to be stored in data type", \
"Bad executable (or shared library)", \
"Bad CPU type in executable", \
"Shared library version mismatch", \
"Malformed Mach-o file", \
"Operation canceled", \
"Identifier removed", \
"No message of desired type", \
"Illegal byte sequence", \
"Attribute not found", \
"Bad message", \
"EMULTIHOP (Reserved)", \
"No message available on STREAM", \
"ENOLINK (Reserved)", \
"No STREAM resources", \
"Not a STREAM", \
"Protocol error", \
"STREAM ioctl timeout", \
"Operation not supported on socket", \
"Policy not found", \
"State not recoverable", \
"Previous owner died", \
"Interface output queue is full"}

#elif defined(__MUSL__)

#define ERR_MSGS_COUNT 132
#define ERR_MSG_UNKNOWN "No error information"
#define ERR_MSGS { \
"No error information", \
"Operation not permitted", \
"No such file or directory", \
"No such process", \
"Interrupted system call", \
"I/O error", \
"No such device or address", \
"Argument list too long", \
"Exec format error", \
"Bad file descriptor", \
"No child process", \
"Resource temporarily unavailable", \
"Out of memory", \
"Permission denied", \
"Bad address", \
"Block device required", \
"Resource busy", \
"File exists", \
"Cross-device link", \
"No such device", \
"Not a directory", \
"Is a directory", \
"Invalid argument", \
"Too many open files in system", \
"No file descriptors available", \
"Not a tty", \
"Text file busy", \
"File too large", \
"No space left on device", \
"Invalid seek", \
"Read-only file system", \
"Too many links", \
"Broken pipe", \
"Domain error", \
"Result not representable", \
"Resource deadlock would occur", \
"Filename too long", \
"No locks available", \
"Function not implemented", \
"Directory not empty", \
"Symbolic link loop", \
"No error information", \
"No message of desired type", \
"Identifier removed", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"Device not a stream", \
"No data available", \
"Device timeout", \
"Out of streams resources", \
"No error information", \
"No error information", \
"No error information", \
"Link has been severed", \
"No error information", \
"No error information", \
"No error information", \
"Protocol error", \
"Multihop attempted", \
"No error information", \
"Bad message", \
"Value too large for data type", \
"No error information", \
"File descriptor in bad state", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"Illegal byte sequence", \
"No error information", \
"No error information", \
"No error information", \
"Not a socket", \
"Destination address required", \
"Message too large", \
"Protocol wrong type for socket", \
"Protocol not available", \
"Protocol not supported", \
"Socket type not supported", \
"Not supported", \
"Protocol family not supported", \
"Address family not supported by protocol", \
"Address in use", \
"Address not available", \
"Network is down", \
"Network unreachable", \
"Connection reset by network", \
"Connection aborted", \
"Connection reset by peer", \
"No buffer space available", \
"Socket is connected", \
"Socket not connected", \
"Cannot send after socket shutdown", \
"No error information", \
"Operation timed out", \
"Connection refused", \
"Host is down", \
"Host is unreachable", \
"Operation already in progress", \
"Operation in progress", \
"Stale file handle", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"Remote I/O error", \
"Quota exceeded", \
"No medium found", \
"Wrong medium type", \
"Operation canceled", \
"No error information", \
"No error information", \
"No error information", \
"No error information", \
"Previous owner died", \
"State not recoverable" \
}

#else  // For GNU/Linux

#define ERR_MSGS_COUNT 134
#define ERR_MSG_UNKNOWN "Unknown error "
#define ERR_MSGS {"Success", \
"Operation not permitted", \
"No such file or directory", \
"No such process", \
"Interrupted system call", \
"Input/output error", \
"No such device or address", \
"Argument list too long", \
"Exec format error", \
"Bad file descriptor", \
"No child processes", \
"Resource temporarily unavailable", \
"Cannot allocate memory", \
"Permission denied", \
"Bad address", \
"Block device required", \
"Device or resource busy", \
"File exists", \
"Invalid cross-device link", \
"No such device", \
"Not a directory", \
"Is a directory", \
"Invalid argument", \
"Too many open files in system", \
"Too many open files", \
"Inappropriate ioctl for device", \
"Text file busy", \
"File too large", \
"No space left on device", \
"Illegal seek", \
"Read-only file system", \
"Too many links", \
"Broken pipe", \
"Numerical argument out of domain", \
"Numerical result out of range", \
"Resource deadlock avoided", \
"File name too long", \
"No locks available", \
"Function not implemented", \
"Directory not empty", \
"Too many levels of symbolic links", \
"Unknown error 41", \
"No message of desired type", \
"Identifier removed", \
"Channel number out of range", \
"Level 2 not synchronized", \
"Level 3 halted", \
"Level 3 reset", \
"Link number out of range", \
"Protocol driver not attached", \
"No CSI structure available", \
"Level 2 halted", \
"Invalid exchange", \
"Invalid request descriptor", \
"Exchange full", \
"No anode", \
"Invalid request code", \
"Invalid slot", \
"Unknown error 58", \
"Bad font file format", \
"Device not a stream", \
"No data available", \
"Timer expired", \
"Out of streams resources", \
"Machine is not on the network", \
"Package not installed", \
"Object is remote", \
"Link has been severed", \
"Advertise error", \
"Srmount error", \
"Communication error on send", \
"Protocol error", \
"Multihop attempted", \
"RFS specific error", \
"Bad message", \
"Value too large for defined data type", \
"Name not unique on network", \
"File descriptor in bad state", \
"Remote address changed", \
"Can not access a needed shared library", \
"Accessing a corrupted shared library", \
".lib section in a.out corrupted", \
"Attempting to link in too many shared libraries", \
"Cannot exec a shared library directly", \
"Invalid or incomplete multibyte or wide character", \
"Interrupted system call should be restarted", \
"Streams pipe error", \
"Too many users", \
"Socket operation on non-socket", \
"Destination address required", \
"Message too long", \
"Protocol wrong type for socket", \
"Protocol not available", \
"Protocol not supported", \
"Socket type not supported", \
"Operation not supported", \
"Protocol family not supported", \
"Address family not supported by protocol", \
"Address already in use", \
"Cannot assign requested address", \
"Network is down", \
"Network is unreachable", \
"Network dropped connection on reset", \
"Software caused connection abort", \
"Connection reset by peer", \
"No buffer space available", \
"Transport endpoint is already connected", \
"Transport endpoint is not connected", \
"Cannot send after transport endpoint shutdown", \
"Too many references: cannot splice", \
"Connection timed out", \
"Connection refused", \
"Host is down", \
"No route to host", \
"Operation already in progress", \
"Operation now in progress", \
"Stale file handle", \
"Structure needs cleaning", \
"Not a XENIX named type file", \
"No XENIX semaphores available", \
"Is a named type file", \
"Remote I/O error", \
"Disk quota exceeded", \
"No medium found", \
"Wrong medium type", \
"Operation canceled", \
"Required key not available", \
"Key has expired", \
"Key has been revoked", \
"Key was rejected by service", \
"Owner died", \
"State not recoverable", \
"Operation not possible due to RF-kill", \
"Memory page has hardware error"}

#endif

#endif  // SRC_S21_STRING_H_

