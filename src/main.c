#include "uart.h"
#include "shell.h"

int main() {
    uart_init();
    uart_puts("Hello World!\n");
    
    while (1) {
        shell();
    }
    
    
    return 0;
}