#ifndef _UTILS_H_
#define _UTILS_H_

unsigned int sprintf(char *dst, char* fmt, ...);
unsigned int vsprintf(char *dst,char* fmt, __builtin_va_list args);
unsigned long hex2dec(char *s);
void align4 (void *size);
#endif