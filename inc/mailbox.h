#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#define MAILBOX_BASE    (MMIO_BASE + 0x0000B880)

#define MAILBOX_READ    ((volatile unsigned int*)(MAILBOX_BASE))
#define MAILBOX_STATUS  ((volatile unsigned int*)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE   ((volatile unsigned int*)(MAILBOX_BASE + 0x20))

#define MAILBOX_EMPTY   0x40000000
#define MAILBOX_FULL    0x80000000

#define GET_BOARD_REVISTION     0x00010002
#define GET_ARM_MEM             0x00010005

#define REQUEST_CODE            0x00000000
#define REQUEST_SUCCEED         0x80000000
#define TAG_REQUEST_CODE        0x00000000
#define END_TAG                 0x00000000


int mbox_request(unsigned int *mailbox);
void get_board_revision();
void get_arm_mem_info();
#endif