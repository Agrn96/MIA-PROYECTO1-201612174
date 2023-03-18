#ifndef OBFIND_H
#define OBFIND_H
#include <QString>
#include "string"
#include "structs.h"
#include <list>
using namespace std;


class obfind
{
public:
    obfind();

    string path;
    string name;
    string caracter;

    void mostrardatos(obfind *disco);
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
    void BuscarInodo(string ruta, int inicio, int posicion, string name);
    void BuscarBloque(string ruta, int inicio, int posicion, bool tipo, int soycero, string name);
    string estrucutrar_regex(string nombre);
    bool Existe_Union(string nombre, string regex);
};

#endif // OBFIND_H
