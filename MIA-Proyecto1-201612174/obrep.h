#ifndef OBREP_H
#define OBREP_H
#include <QString>
#include "string"
#include "structs.h"
#include <list>
using namespace std;

class obrep
{
public:
    obrep();

    string path;
    string name;
    string id;
    string ruta_file;

    list<string> inodos;
    list<string> nodos;

    int root = 0;

    void mostrardatos(obrep *disco);
    bool existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int * size_particion);
    bool ExisteMBR(string ruta, structs::MBR *mbr);
    bool ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part);
    int ExisteExtendida(structs::MBR tmp_MBR);
    bool ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part);

    void CrearTablaMBR(string ruta);
    void CrearDisk(string ruta);

    void CrearBIPMAP(string ruta, int inicio, bool tipo);

    void CrearFILE(string ruta, int inicio, string ruta_carpetas);

    string getUsuario(string usuario, string RUTA_PARTITTION, int PART_START);
    string getGrupo(string grupo, string RUTA_PARTITTION, int PART_START);

    void archivo_dot(string content, string name);
    string CharToString(char tmp[]);
    string NombreArchivo(string path);
    string ToUpperString(string cadena);
    list<structs::EBR> getEBR(string ruta, int extendida);
    list<string> separar_carpetas(string path);
    string getNombreTipoMBR(char a);
    string EliminarComillas(string cadena);


};

#endif // OBREP_H
