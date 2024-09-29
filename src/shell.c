#include "uart.h"
#include "string.h"
#include "reboot.h"
#include "utils.h"
#include "mailbox.h"
#include "cpio.h"

void shell_input(char *cmd) {
    uart_puts("# ");
    char c;
    int idx = 0, end = 0;
    cmd[0] = '\0';
    while((c = uart_getc()) != '\n') {
        if(c == 8 || c == 127) {
            if (idx > 0) {
                idx--;
                for (int i = idx; i < end; i++) {
                    cmd[i] = cmd[i + 1];
                }
                cmd[--end] = '\0';
            }
        }
        else if (c == 3) {
            cmd[0] = '\0';
			break;
		}
		else if(c == 27) {
			c = uart_getc();
			c = uart_getc();
			switch (c) {
				case 'C' :
					if (idx < end) idx++;
					break;
				case 'D' :
					if (idx > 0) idx--;
					break;
				default : uart_flush();
			}
		}
		else {
            if (idx < end) {
                for (int i = end; i > idx; i--) {
                    cmd[i] = cmd[i - 1];
                }
            }
            cmd[idx++] = c;
            cmd[++end] = '\0';
        }
        printf("\r# %s \r\e[%dC", cmd, idx + 2);
    }
	printf("\r");
}
void shell()
{
	char cmd[100];

	while(1)
	{
		shell_input(cmd);

		if (strcmp(cmd, "hello") == 0)
		{
			uart_puts("\nHello World!\n");
		}
		else if (strcmp(cmd, "help") == 0)
		{
			uart_puts("\nhelp\t\t: print this help menu\n");
            uart_puts("hello\t\t: print Hello World!\n");
            uart_puts("reboot\t\t: reboot the device\n");
            uart_puts("meminfo\t\t: get the base address and size of memory\n");
            uart_puts("boardinfo\t: get the board revision number of raspi 3b\n");
		}
		else if (strcmp(cmd, "reboot") == 0)
		{
			uart_puts("\nGoing to reboot! See you later~~\n");
            reset(50);
		}
		else if (strcmp(cmd, "meminfo") == 0) {
			get_arm_mem_info();
		}
		else if (strcmp(cmd, "boardinfo") == 0) {
			get_board_revision();
		}
		else if (strcmp(cmd, "ls") == 0) {
			cpio_list();
		}
		else if (strcmp(cmd, "cat") == 0) {
			cpio_cat();
		}
		else {
			uart_puts("\nCommand not found \n");
		}
	}
}