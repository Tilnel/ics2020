#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
    size_t i = 0;
    while (s[i] != '\0') {
        i++;  
    }
    return i;
}

char *strcpy(char *dst, const char *src) {
    return strncpy(dst, src, strlen(src));
}

char *strncpy(char *dst, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }
    for (; i < n; i++)
        dst[i] = '\0';
    return dst;
}

char *strcat(char *dst, const char *src) {
    size_t dest_len = strlen(dst);
    size_t src_len = strlen(src);
    size_t i;
    for (i = 0; i < src_len && src[i] != '\0'; i++) {
        dst[dest_len + i] = src[i];
    }
    dst[dest_len + i] = '\0';
    return dst;
}

int strcmp(const char *s1, const char *s2) { 
  size_t i;
  for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) {
    if (s1[i] == s2[i]) continue;
    else return s1[i] > s2[i] ? 1 : -1;
  }
  if (s1[i] == '\0' && s2[i] == '\0') return 0;
  else return s1[i] == '\0' ? -1 : 1;
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) { 
  size_t i;
  for (i = 0; s1[i] != '\0' && s2[i] != '\0' && i < n; i++) {
    if (s1[i] == s2[i]) continue;
    else return s1[i] > s2[i] ? 1 : -1;
  }
  if (i == n) return 0;
  if (s1[i] == '\0' && s2[i] == '\0') return 0;
  else return s1[i] == '\0' ? -1 : 1;
  return 0; 
}

void *memset(void *v, int c, size_t n) { 
  size_t i;
  char *tmp = (char *)v;
  for (i = 0; i < n; i++) {
    *(tmp + i) = c;
  }
  return v;
}

void *memmove(void *dst, const void *src, size_t n) { 
  size_t i; 
  char *tmp = (char *)dst;
  char *tmps = (char *)src;
  for (i = 0; i < n; i++) {
    *(tmp + i) = *(tmps + i);
  }
  return dst; 
}

void *memcpy(void *out, const void *in, size_t n) { 
  size_t i; 
  char *tmp = (char *)out;
  char *tmps = (char *)in;
  for (i = 0; i < n; i++) {
    *(tmp + i) = *(tmps + i);
  }
  return out; 
}

int memcmp(const void *s1, const void *s2, size_t n) { 
  size_t i;
  char *tmp1 = (char *)s1, *tmp2 = (char *)s2;
  for (i = 0; i < n; i++) {
    if (tmp1[i] == tmp2[i]) continue;
    else return tmp1[i] > tmp2[i] ? 1 : -1;
  }
  if (i == n) return 0;
  return 0;
}

#endif
