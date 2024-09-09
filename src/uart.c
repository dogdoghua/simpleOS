#include "gpio.h"
#include "sprintf.h"
/* Auxilary mini UART registers */
#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))

extern volatile unsigned char _end;


void uart_init () {

    register unsigned int r;

    /*intialize UART*/
    *AUX_ENABLE |= 1;       // enable UART
    *AUX_MU_CNTL = 0;       // disable transmitter and receiver
    *AUX_MU_MCR = 0;        // dont need flow control
    *AUX_MU_LCR = 3;        // set the data size to 8 bit
    *AUX_MU_IER = 0;        // disable interrupt
    *AUX_MU_IIR = 0xc6;     // show the interrupt status (c: FIFO, 6: disable interrupt)
    *AUX_MU_BAUD = 270;     // set baud rate 115200

    /*map UART1 to GPIO pins*/
    r = *GPFSEL1;
    r &= ~((7<<12) | (7<<15));  // gpio14, gpio15
    r |= ((2<<12) | (2<<15));   // alt5
    *GPFSEL1 = r;
    /*enable gpio pins*/
    *GPPUD = 0;                 // enable pull up / down on GPIO pins 14 15
    r = 150; while (r--) {asm volatile("nop");} // 150ms to allow pull to take effect
    *GPPUDCLK0 = (1<<14)|(1<<15); // set the clock for gpio14 15
    r = 150; while (r--) {asm volatile("nop");}
    *GPPUDCLK0 = 0;             // flush GPIO setup
    *AUX_MU_CNTL = 3;           // enable Tx, Rx
}

/*Send a character*/
void uart_send(unsigned int c) {
    /*wait until we can send*/
    do {asm volatile("nop");} while (!(*AUX_MU_LSR & 0x20));
    /* write the character to the buffer */
    *AUX_MU_IO=c;
}

/*Receive a character*/
char uart_getc() {
    char r;
    /* wait until something is in the buffer */
    do {asm volatile("nop");} while (!(*AUX_MU_LSR & 0x01));
    /*read and return*/
    r = (char)(*AUX_MU_IO);
    /*convert carriage return to newline*/
    return r == '\r' ? '\n' : r;
}

/*Display a string*/
void uart_puts (char *s) {
    while (*s) {
        /* convert newline to carriage return + newline */
        if(*s == '\n')
            uart_send('\r');
        uart_send(*s++);
    }
}

void uart_flush()
{
	char tmp;
	do{tmp = (char)(*AUX_MU_IO);}while(*AUX_MU_LSR&0x01);
}

void uart_hex(unsigned int d) {
    unsigned int n;
    int c;
    for(c=28;c>=0;c-=4) {
        // get highest tetrad
        n=(d>>c)&0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n+=n>9?0x37:0x30;
        uart_send(n);
    }
}

void printf(char *fmt, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    // we don't have memory allocation yet, so we
    // simply place our string after our code
    char *s = (char*)&_end;
    // use sprintf to format our string
    vsprintf(s,fmt,args);
    // print out as usual
    while(*s) {
        /* convert newline to carrige return + newline */
        if(*s=='\n')
            uart_send('\r');
        uart_send(*s++);
    }
}
