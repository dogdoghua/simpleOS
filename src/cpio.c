#include "cpio.h"
#include "utils.h"
#include "uart.h"
#include "string.h"


/* list all of the file */
void cpio_list () {
    char *addr = CPIO_ADDR;

    while (strcmp((char *)(addr + sizeof(CPIO_HEADER)), "TRAILER!!!") != 0) {
        CPIO_HEADER *header = (CPIO_HEADER*) addr;
        // if (strcmp(header->c_magic, CPIO_MAGIC_NUM) == 0) break;
        
        unsigned long pathname_size = hex2dec(header->c_namesize);
        unsigned long file_size = hex2dec(header->c_filesize);
        unsigned long header_pathname_size = sizeof(CPIO_HEADER) + pathname_size;

        // The pathname is followed by NUL bytes so that the total size	of the fixed header + pathname is a multiple of four.
        align4(&header_pathname_size);
        align4(&file_size);
        
        // print file name on the screen
        printf("\n%s", addr + sizeof(CPIO_HEADER));
        // move to next CPIO header
        addr += (header_pathname_size + file_size);
    }
    printf("\r\n");
}

char* _find_file (char *filename) {
    char *addr = CPIO_ADDR;
    CPIO_HEADER *header = (CPIO_HEADER*) addr;
    
    while (strcmp((char *)(addr + sizeof(CPIO_HEADER)), "TRAILER!!!") != 0) {
        if (strcmp(filename, (char*)(addr + sizeof(CPIO_HEADER))) == 0) {
            return addr;
        }

        unsigned long pathname_size = hex2dec(header->c_namesize);
        unsigned long file_size = hex2dec(header->c_filesize);
        unsigned long header_pathname_size = sizeof(CPIO_HEADER) + pathname_size;

        align4(&header_pathname_size);
        align4(&file_size);
        addr += (header_pathname_size + file_size);
    }
    return 0;
}

void cpio_cat () {
    // get input
    char c, filename[100];
    int idx = 0;
    printf("\nFilename: ");
    while ((c = uart_getc()) != '\n') {
        uart_send(c);
        filename[idx++] = c;
    }
    char *target = _find_file(filename);

    if (target) {
        CPIO_HEADER *header = (CPIO_HEADER*) target;
        unsigned long pathname_size = hex2dec(header->c_namesize);
        unsigned long file_size = hex2dec(header->c_filesize);
        unsigned long header_pathname_size = sizeof(CPIO_HEADER) + pathname_size;

        align4(&header_pathname_size);
        align4(&file_size);

        printf("\n%s\n", target + header_pathname_size);
    }
    else {
        printf("\nFile not found!\n");
    }
}
