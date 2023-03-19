#ifndef OBMOUNT_H
#define OBMOUNT_H
#include <QString>
#include "string"
#include "structs.h"
using namespace std;


class obmount
{
public:
    obmount();
    string name = "";
    string path = "";
    string id = "";

    void mostrardatos(obmount *disco);
    structs::MBR RecuperarMBR(string ruta);
    bool ExisteMBR(string ruta);
    string EliminarComillas(string cadena);
    bool ExisteNombre(structs::MBR tmp_MBR, string name);
    int ExisteExtendida(structs::MBR tmp_MBR);
    bool ExisteNombreLogica(string ruta, string name, int part_start_ext);

    structs::partitions RecuperarParticion(structs::MBR tmp_MBR, string name);
    structs::EBR RecuperarParticionLogica(string ruta, string name, int part_start_ext);


    void DesmontarMount(obmount *disco);


};

#endif // OBMOUNT_H
