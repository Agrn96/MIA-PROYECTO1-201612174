#ifndef STRUCTS_H
#define STRUCTS_H
#include <iostream>
#include <time.h>
using namespace std;

class structs
{
public:
    structs();
    typedef struct partitions
    {
        char status = '0';
        char type = ' ';
        char fit = ' ';
        int part_start = 0;
        int size = 0;
        char name[16] = " ";
    } partition;

    typedef struct MBR_
    {
        int mbr_size = 0;
        time_t mbr_date;
        int mbr_disk_signature;
        char mbr_fit;
        partition mbr_partitions[4];
    } MBR;

    typedef struct EBR_
    {
        char status = '3';
        char fit = ' ';
        int part_start = 0;
        int size = 0;
        int part_next = -1;
        char name[16] = " ";
    } EBR;

    typedef struct ParticionMontada_
    {

        string letter; // Change to string to store the dsk name
        int status;
        int type = -1;
        string nombre;
    }ParticionMontada;

    typedef struct DiscosMontados_
    {
        string path;
        int number;
        int status;
        ParticionMontada partittion[99];
    }DiscosMontados;

    typedef struct SuperBloque_
    {
        int s_filesystem_type;
        int s_inodes_count;
        int s_blocks_count;
        int s_free_blocks_count;
        int s_free_inodes_count;
        time_t s_mtime;
        time_t s_umtime;
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
    }SuperBloque;

    typedef struct TabladeInodos_
    {
        int i_uid;
        int i_gid;
        int i_size;
        time_t i_atime;
        time_t i_ctime;
        time_t i_mtime;
        int i_block[15];
        char i_type = 'I';
        int i_perm;
    }TabladeInodos;


    typedef struct Content_
    {
        char b_name[12];
        int b_inodo = -1;
    }Content;

    typedef struct BloquesdeCarpetas_
    {
        Content b_content[4];
    }BloquesdeCarpetas;

    typedef struct BloquesdeArchivos_
    {
        char b_content[64];
    }BloquesdeArchivos;

    typedef struct BloquesdeApuntadores_
    {
        int b_pointers[16];
    }BloquesdeApuntadores;

    typedef struct Jornaling_
    {
        int size = -1;

        char tipo = 'I';
        char path[40] = "";
        char contenido[100] = ""; // Validar cuando espacio necesitan.
        time_t log_fecha;
        char tipo_operacion[10] = "";

    }Jornaling; //typedef

    typedef struct
    {
        float porcentaje;
        string nombre_tipo;
    }DRAW_MBR;

    typedef struct
    {
        string ruta;
        string contenido;
        char tipo;
    }COPY;

    typedef struct
    {
        string usuario = "-1";
        string password = "-1";
        string id_particion = "-1";
        string grupo = "-1";
        string posicion ="-1";
        string tipo = "-1";
        string no_grupo = "-1";
    }LOGIN;

    typedef struct
    {
        string posicion = "-1";
        string nombre = "-1";
    }GROUP;
};

#endif // STRUCTS_H
