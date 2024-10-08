#ifndef _UART_H_
#define _UART_H_
void uart_init();
void uart_send(unsigned int c);
char uart_getc();
void uart_puts(char *s);
void uart_flush();
void printf(char *fmt, ...);
#endif