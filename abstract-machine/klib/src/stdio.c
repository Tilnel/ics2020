#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char buf[4096];
int itoa(int n, char *s, int base);

int sscanf(const char *src, const char *str, ...) {
    va_list vl;
    int i = 0, j = 0, ret = 0;
    char buff[100] = {0};
    strcpy(buff, src);
    va_start(vl, str);
    i = 0;
    while (str && str[i]) {
        if (str[i] == '%') {
            i++;
            switch (str[i]) {
            case 'c': {
                *(char *)va_arg(vl, char *) = buff[j];
                j++;
                ret++;
                break;
            }
            case 'd': {
                j += atoi_b(&buff[j], (int *)va_arg(vl, int *)) - &buff[j];
                ret++;
                break;
            }
            }
        } else {
            buff[j] = str[i];
            j++;
        }
        i++;
    }
    va_end(vl);
    return ret;
}

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(buf, fmt, ap);
    va_end(ap);
    size_t i = 0;
    while (buf[i] != '\0') {
        putch(buf[i]);
        i++;
    }
    return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    size_t i = 0;
    int d;
    char *s;
    char c;
    size_t pos = 0;
    size_t len, addlen;
    while (fmt[i] != '\0' && i < 256) {
        switch (fmt[i]) {
        case '%':
            switch (fmt[++i]) {
            case 'c':
                c = va_arg(ap, int);
                strncpy(out + pos, &c, 1);
                pos++;
                break;
            case 's':
                s = va_arg(ap, char *);
                len = strlen(s);
                strncpy(out + pos, s, len);
                pos += len;
                break;
            case '0':
                addlen = fmt[i + 1] - '0';
                d = va_arg(ap, int);
                len = itoa(d, out + pos, 10);
                if (len < addlen) {
                    addlen -= len;
                    while (addlen > 0) {
                        out[pos] = '0';
                        pos++;
                        addlen--;
                    }
                    itoa(d, out + pos, 10);
                }
                pos += len;
                i += 2;
                break;
            case 'd':
                d = va_arg(ap, int);
                len = itoa(d, out + pos, 10);
                pos += len;
                break;
            case 'x':
                d = va_arg(ap, int);
                len = itoa(d, out + pos, 16);
                pos += len;
                break;
            case 'p':
                d = va_arg(ap, int);
                len = itoa(d, out + pos, 16);
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

int itoa(int n, char *s, int base) {
    int i, j, sign;
    char buf[12];
    if (base == 16) {
        i = 0;
        uint32_t tmp = (uint32_t)n;
        do {
            buf[i] = tmp % base + '0';
            i++;
        } while ((tmp = tmp / base));

        for (j = i - 1; j >= 0; j--) {
            s[j] = (buf[i - 1 - j] <= '9')
                       ? buf[i - 1 - j]
                       : buf[i - 1 - j] - '9' + 'a' - 1;
        }
        s[i] = '\0';
        return i;
    }
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        buf[i] = n % base + '0';
        i++;
    } while ((n /= base) > 0);
    if (sign < 0) {
        s[i] = '-';
        i++;
    }

    for (j = i - 1; j >= 0; j--) {
        s[j] = (buf[i - 1 - j] <= '9' || buf[i - 1 - j] == '-')
                   ? buf[i - 1 - j]
                   : buf[i - 1 - j] - '9' + 'a' - 1;
    }
    s[i] = '\0';
    return i;
}

int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(out, fmt, ap);
    va_end(ap);
    return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
    assert(0);
    return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
    assert(0);
    return 0;
}

#endif
