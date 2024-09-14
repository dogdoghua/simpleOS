#include "uart.h"
#include "shell.h"
#include "mailbox.h"

int main() {
    uart_init();
    uart_puts("Hello World!\n");
    shell();
    return 0;
}