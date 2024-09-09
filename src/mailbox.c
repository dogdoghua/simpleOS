#include "gpio.h"

volatile unsigned int __attribute__((aligned(16))) mbox[8];

#define VIDEOCORE_MBOX (MMIO_BASE + 0x0000B880)
#define MAILBOX_READ    
