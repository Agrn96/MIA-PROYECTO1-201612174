#ifndef OBMKFS_H
#define OBMKFS_H
#include "structs.h"
#include <QString>
#include "string"
using namespace std;

class obmkfs
{
public:
    obmkfs();

    string id= "";
    string type = "FAST";
    string fs = "2FS";

    void mostrardatos(obmkfs *disco);
    bool existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int * size_particion);
    bool ExisteMBR(string ruta, structs::MBR *mbr);
    bool ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part);
    bool ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part);
    string ToUpperString(string cadena);
    void Create_Jornaling(int part_Start, string ruta, int final);


};

#endif // OBMKFS_H
