#ifndef OBMKDIR_H
#define OBMKDIR_H
#include <QString>
#include "string"
#include "structs.h"
#include <list>
using namespace std;

class obmkdir
{
public:
    obmkdir();

    string path;
    bool r = false;
    string p_entrada="r";

    void mostrardatos(obmkdir *disco);
    string EliminarComillas(string cadena);
    static void getSuperBloque(structs::SuperBloque *sp, string ruta, int part_Start);
    static bool getInodo(structs::TabladeInodos*sp, string ruta, int part_Start);
    static bool getBlock_Files(structs::BloquesdeArchivos *sp, string ruta, int part_Start);
    static bool getBlock_Carpets(structs::BloquesdeCarpetas *sp, string ruta, int part_Start);
    static bool getBlock_Apuntador(structs::BloquesdeApuntadores *sp, string ruta, int part_Start);
    list<string> separar_carpetas(string path);

    void BuscarEspacio(structs::SuperBloque sp, int inodo, int block, structs::TabladeInodos ino, string name, int *recuperar_inodo);
    void CreandoEnBloqueArchivos(structs::SuperBloque sp, int inodo, int block, structs::BloquesdeCarpetas bc, int *recuperar_inodo);

    string NombreArchivo(string path);
    void CrearNivel1(structs::SuperBloque sp, int inodo, int block, string name,  int *recuperar_inodo);
    void CrearNivel2(structs::SuperBloque sp, int inodo, int block, string name,  int *recuperar_inodo);
    bool existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int * size_particion);
    bool ExisteMBR(string ruta, structs::MBR *mbr);
    bool ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part);
    bool ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part);
};

#endif // OBMKDIR_H
