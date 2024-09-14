.section ".text.boot"
.global _start

_start:
    // read cpu id
    mrs     x1, mpidr_el1 // Multiprocessor Affinity Register 
    and     x1, x1, #3 // get the 2 lowest bits in x1
    cbz     x1, 2f // compare and branch on 0 (2f: label 2 forward reference)
    
    // when cpu id > 0 (slave cores)
1:  wfe // wait for event
    b       1b // label 1 branch

2:  // cpu id == 0 (main core)
    // set top of stack 
    ldr     x1, =_start // start address load to x1
    mov     sp, x1 // set x1 value to stack pointer

    // clear bss
    ldr     x1, =__bss_start // bss start address load to x1
    ldr     w2, =__bss_size // bss size load to w2

3:  cbz     w2, 4f  //  check if bss size is equal 0
    str     xzr, [x1], #8 // write xzr value into x1 => x1 + 8 and write into x1
    sub     w2, w2, #1
    cbnz    w2, 3b

4:  bl      main
    b       1b