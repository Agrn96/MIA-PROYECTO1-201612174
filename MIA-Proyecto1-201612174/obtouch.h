#ifndef OBTOUCH_H
#define OBTOUCH_H
#include <QString>
#include "string"
#include "structs.h"
#include <list>
using namespace std;

class obtouch
{
public:
    obtouch();

    string path = "";
    int size = 0;
    string cont = "";
    bool r = false;
    bool stdin = false;

    void mostrardatos(obtouch *disco);
    string EliminarComillas(string cadena);
    static void getSuperBloque(structs::SuperBloque *sp, string ruta, int part_Start);
    static bool getInodo(structs::TabladeInodos*sp, string ruta, int part_Start);
    static bool getBlock_Files(structs::BloquesdeArchivos *sp, string ruta, int part_Start);
    static bool getBlock_Carpets(structs::BloquesdeCarpetas *sp, string ruta, int part_Start);
    static bool getBlock_Apuntador(structs::BloquesdeApuntadores *sp, string ruta, int part_Start);
    list<string> separar_carpetas(string path);

    void BuscarEspacio(structs::SuperBloque sp, int inodo, int block, structs::TabladeInodos ino, string name, int *recuperar_inodo);
    void CreandoEnBloqueArchivos(structs::SuperBloque sp, int inodo, int block, structs::BloquesdeCarpetas bc, int *recuperar_inodo);
    void CrearArchivos(structs::SuperBloque sp, int inodo, string entrada);
    string NombreArchivo(string path);
    void CrearNivel1(structs::SuperBloque sp, int inodo, int block, string name, int *recuperar_inodo);
    void CrearNivel2(structs::SuperBloque sp, int inodo, int block, string name, int *recuperar_inodo);
    void CrearNivel3(structs::SuperBloque sp, int inodo, int block, string name, int *recuperar_inodo);

    bool existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int * size_particion);
    bool ExisteMBR(string ruta, structs::MBR *mbr);
    bool ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part);
    bool ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part);

    void CrearJornaling(string operacion, char tipo, string path, string contenido, int size, string ruta, int inicio);
    void getJornaling(list<structs::Jornaling> *jg, structs::SuperBloque super, string ruta, int inicio);
    void setJornaling(structs::Jornaling jg, structs::SuperBloque super, string ruta, int inicio);

};

#endif // OBTOUCH_H
