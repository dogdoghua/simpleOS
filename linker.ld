SECTIONS
{
  . = 0x80000;
  .text : { KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) }
  .rodata : { *(.rodata .rodata.* .gun.linkonce.r*) } /*read only data*/
  PROVIDE(_data = .);
  .data : { *(.data .data.* .gnu.linkonce.d*) }
  .bss (NOLOAD) : {
    . = ALIGN(16); /*align by 16 bytes*/
    __bss_start = .; 
    *(.bss .bss.*)
    *(COMMON)
    __bss_end = .;
  }
  _end = .;
  /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>3;