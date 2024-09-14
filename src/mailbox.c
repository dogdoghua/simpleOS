#include "gpio.h"
#include "uart.h"
#include "mailbox.h"

volatile unsigned int __attribute__((aligned(16))) mailbox[8];


/**
 * 1. Combine the message address (upper 28 bits) with channel number (lower 4 bits)
 * 2. Check if [Mailbox 0] status register’s full flag is set.
 * 3. If not, then you can `write` to [Mailbox 1] Read/Write register.
 * 4. Check if [Mailbox 0] status register’s empty flag is set.
 * 5. If not, then you can `read`  from [Mailbox 0] Read/Write register.
 * 6. Check if the value is the same as you wrote in step 1.
 */
int mailbox_call () {
    unsigned int msg = (((unsigned int) ((unsigned long) mailbox) & ~0xF) | (0x8 & 0xF));

    while (*MAILBOX_STATUS & MAILBOX_FULL) {}   
    *MAILBOX_WRITE = msg;   // register is not full => write

    while (1) {
        while (*MAILBOX_STATUS & MAILBOX_EMPTY) {}
        // register is not empty
        if (msg == *MAILBOX_READ)
            return mailbox[1] == REQUEST_SUCCEED;
    }
    return 0;
    
}

void get_board_revision () {
    mailbox[0] = 7 * 4; // buffer size in bytes;
    mailbox[1] = REQUEST_CODE;
    // tags start
    mailbox[2] = GET_BOARD_REVISTION;   // tag identifier
    mailbox[3] = 4; // maximum of request and response value buffer's length
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    // tags end
    mailbox[6] = END_TAG;
    mailbox_call();

    printf("\n[Mailbox] Raspi3's board revision number: 0x%x\n", mailbox[5]); // it should be 0xA02082 for rpi3 b

}
// 0xA02082: 1000 0010
void get_arm_mem_info () {
    mailbox[0] = 8 * 4;
    mailbox[1] = REQUEST_CODE;
    // tags start
    mailbox[2] = GET_ARM_MEM;
    mailbox[3] = 8; // maximum of request and response value buffer's length
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer: base address 
    mailbox[6] = 0; // value buffer: size in bytes
    // tags end
    mailbox[7] = END_TAG;
    mailbox_call();
    printf("\n[Mailbox] Physical memory base address: 0x%x\n", mailbox[5]);
    printf("[Mailbox] Physical memory size in bytes 0x%x\n", mailbox[6]);

}