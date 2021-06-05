#ifndef Estructuras_H
#define Estructuras_H
#include <string>
using namespace std;

class Estructura
{
public:
    Estructura();
    typedef struct _Partition
    {
        char part_status='N';
        char part_type=' ';
        char part_fit=' ';
        int part_start=0;
        int part_size=0;
        char part_name[16]={""};
    } Partition;

    typedef struct _MBR
    {
        int mbr_tamano=0;
        char mbr_fecha_creacion[20];
        int mbr_disk_signature;
        char disk_fit;
        Partition mbr_Partition_1;
        Partition mbr_Partition_2;
        Partition mbr_Partition_3;
        Partition mbr_Partition_4;
    } MBR;

    typedef struct _EBR
    {
        char part_status='N';
        char part_fit = ' ';
        int part_start = 0;
        int part_size = 0;
        int part_next = -1;
        char part_name[16]={""};
    } EBR;

    typedef struct _Super
    {
        int s_filesystem_type;
        int s_inodes_count;
        int s_blocks_count;
        int s_free_blocks_count;
        int s_free_inodes_count;
        char s_mtime [20];
        char s_umtime [20];
        int s_mnt_count;
        int s_magic;
        int s_inode_size;
        int s_block_size;
        int s_firts_ino;
        int s_first_blo;
        int s_bm_inode_start;
        int s_bm_block_start;
        int s_inode_start;
        int s_block_start;
    } Super;

    typedef struct _JOURNAL{
        char Journal_Tipo_Operacion[10] ;
        char Journal_tipo;
        char Journal_nombre [200];
        char Journal_contenido [200];
        char Journal_fecha[20]; 
        char Journal_propietario [10];
        int Journal_permisos;
    }JOURNAL;

    typedef struct _Inodo
    {
        int i_uid;
        int I_gid;
        int i_size;
        char i_atime[20];
        char i_ctime[20];
        char i_mtime[20];
        int i_block[15];
        char i_type;
        int i_perm;

    } Inodo;

    typedef struct _content
    {
        char b_name [12];
        int b_inodo;
    } content;

    typedef struct _Carpeta
    {
        content b_content [4];
    } Carpeta;

    typedef struct _Archivo
    {
        char b_content [64];   
    } Archivo;

    typedef struct _Apuntador
    {
        int b_pointers [16];
    } Apuntador;
    
};

#endif