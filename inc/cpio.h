#ifndef _CPIO_H_
#define _CPIO_H_

#define CPIO_MAGIC_NUM "070701"

#define CPIO_ADDR (char*)0x8000000 // qemu loads the cpio archuve file to 0x8000000

typedef struct cpio_newc_header {
    char    c_magic[6];
    char    c_ino[8];
    char    c_mode[8];
    char    c_uid[8];
    char    c_gid[8];
    char    c_nlink[8];
    char    c_mtime[8];
    char    c_filesize[8];
    char    c_devmajor[8];
    char    c_devminor[8];
    char    c_rdevmajor[8];
    char    c_rdevminor[8];
    char    c_namesize[8];
    char    c_check[8];
}__attribute__((packed)) CPIO_HEADER; // 110 byte
// } CPIO_HEADER;


void cpio_list();
void cpio_cat();
char* _find_file(char *name);

#endif