#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char buf[256];
  va_list ap;
  va_start(ap, fmt);
  sprintf(buf, fmt, ap);
  va_end(ap);
  size_t i = 0;
  assert(0);
  assert(buf[i] != '\0');
  while (buf[i] != '\0') {
    putch(buf[i]);
    i++;
  }
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) { return 0; }

int itoa(int n, char *s) {
    int i, j, sign;
    char buf[12];
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        buf[i] = n % 10 + '0';
        i++;
    } while ((n /= 10) > 0);
    if (sign < 0) {
        s[i] = '-';
        i++;
    }

    for (j = i - 1; j >= 0; j--) {
        s[j] = buf[i - 1 - j];
    }
    s[i] = '\0';
    return i;
}

int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
    size_t i = 0;
    int d;
    char *s;
    size_t pos = 0;
    size_t len, addlen;
    va_start(ap, fmt);
    while (fmt[i] != '\0') {
        switch (fmt[i]) {
        case '%':
            switch (fmt[++i]) {
            case 's':
                s = va_arg(ap, char *);
                len = strlen(s);
                strncpy(out + pos, s, len);
                pos += len;
                break;
            case '0':
                addlen = fmt[++i] - '0';
                d = va_arg(ap, int);
                len = itoa(d, out + pos);
                addlen -= len;
                while (addlen > 0) {
                  out[pos] = '0';
                  pos++;
                  addlen --;
                }
                itoa (d, out + pos);
                pos += len;
                i++;
                break;
            case 'd':
                d = va_arg(ap, int);
                len = itoa(d, out + pos);
                pos += len;
                break;
            }
            break;

        default:
            out[pos] = fmt[i];
            pos++;
        }
        i++;
    }
    out[pos] = '\0';
    va_end(ap);
    return pos;
}

int snprintf(char *out, size_t n, const char *fmt, ...) { return 0; }

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) { return 0; }

#endif
