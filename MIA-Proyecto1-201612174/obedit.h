#ifndef OBEDIT_H
#define OBEDIT_H
#include <QString>
#include "string"
#include "structs.h"
#include <list>
using namespace std;

class obedit
{
public:
    obedit();

    int EMPIEZA;
    string RUTA;

    string path = "";
    string cont = "";
    bool stdin = false;
    void mostrardatos(obedit *disco);
    string EliminarComillas(string cadena);
    static void getSuperBloque(structs::SuperBloque *sp, string ruta, int part_Start);
    static bool getInodo(structs::TabladeInodos*sp, string ruta, int part_Start);
    static bool getBlock_Files(structs::BloquesdeArchivos *sp, string ruta, int part_Start);
    static bool getBlock_Carpets(structs::BloquesdeCarpetas *sp, string ruta, int part_Start);
    static bool getBlock_Apuntador(structs::BloquesdeApuntadores *sp, string ruta, int part_Start);
    list<string> separar_carpetas(string path);
    string NombreArchivo(string path);
    bool existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int * size_particion);
    bool ExisteMBR(string ruta, structs::MBR *mbr);
    bool ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part);
    bool ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part);

    void Modificar(structs::SuperBloque sp, int inodo, string contenido);

};

#endif
