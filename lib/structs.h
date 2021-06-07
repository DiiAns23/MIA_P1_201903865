//AQUI SE DECLARAN TODAS LAS ESTRUCTURAS A UTILIZAR EN EL PROGRAMA,
/*
*/
#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

using namespace std;

class Structs {
public:
    Structs();

    typedef struct _Partition {
        char part_status = '0';
        char part_type;
        char part_fit;
        int part_start = -1;
        int part_size = 0;
        char part_name[16];
    } Partition;

    typedef struct _MBR
    {
        int mbr_tamano;
        char mbr_fecha_creacion[20];
        int mbr_disk_signature;
        char disk_fit;
        Partition mbr_Partition_1;
        Partition mbr_Partition_2;
        Partition mbr_Partition_3;
        Partition mbr_Partition_4;
    } MBR;

    typedef struct _EBR {
        char part_status = '0';
        char part_fit;
        int part_start;
        int part_size = 0;
        int part_next = -1;
        char part_name[16];
    } EBR;

    typedef struct _Inodes {
        int i_uid = -1;
        int i_gid = -1;
        int i_size = -1;
        char i_atime[20];
        char i_ctime[20];
        char i_mtime[20];
        int i_block[15] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        char i_type = -1;
        int i_perm = -1;
    } Inodes;

    typedef struct _Superblock {
        int s_filesystem_type;
        int s_inodes_count;
        int s_blocks_count;
        int s_free_blocks_count;
        int s_free_inodes_count;
        char s_mtime[20];
        char s_umtime[20];
        int s_mnt_count;
        int s_magic = 0xEF53;
        int s_inode_size = sizeof(Inodes);
        int s_block_size= sizeof(Folderblock);
        int s_fist_ino = 0;
        int s_first_blo = 0;
        int s_bm_inode_start;
        int s_bm_block_start;
        int s_inode_start;
        int s_block_start;
    } Superblock;

    typedef struct _Content {
        char b_name[12];
        int b_inodo = -1;
    } Content;

    typedef struct _Folderblock //64
    {
        Content b_content[4];
    } Folderblock;

    typedef struct _Fileblock //64
    {
        char b_content[64];
    } Fileblock;

    typedef struct _Pointerblock //64
    {
        int b_pointers[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    } Pointerblock;

    typedef struct _Journaling {
        char operation[10] = "";
        char type = -1;
        char path[100] = "";
        char content[60] = "";
        char date[20] = "";
        int size = 0;
    } Journaling;
};

#endif // END OF DECLARATION
