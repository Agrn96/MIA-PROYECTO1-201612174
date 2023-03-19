#ifndef OBRECOVERY_H
#define OBRECOVERY_H
#include <QString>
#include "string"
#include "structs.h"
#include <list>
using namespace std;

class obrecovery
{
public:
    obrecovery();

    string id;

    void mostrardatos(obrecovery *disco);
    void recovery();
    void simulateloss();
    string EliminarComillas(string cadena);
    list<string> separar_carpetas(string path);
    string NombreArchivo(string path);
    bool existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int * size_particion);
    bool ExisteMBR(string ruta, structs::MBR *mbr);
    bool ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part);
    bool ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part);
};

#endif // OBRECOVERY_H
