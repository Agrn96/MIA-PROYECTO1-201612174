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
    list<string> bloques;
    list<string> nodos;

    int root = 0;

    void mostrardatos(obrep *disco);
    bool existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int * size_particion);
    bool ExisteMBR(string ruta, structs::MBR *mbr);
    bool ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part);
    int ExisteExtendida(structs::MBR tmp_MBR);
    bool ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part);

    void CrearTree(string ruta, int inicio);
    void CrearTreeRoot(string ruta, int inicio);
    list<string> CrearBloque(structs::SuperBloque sp, int inodo, int bloque, char tipo, string ruta,  string *contenido1);
    void CrearTablaMBR(string ruta);
    void CrearDisk(string ruta);
    void CrearSB(string ruta, int inicio);

    void CrearInodo(string ruta, int inicio, int posicion);
    void CrearBloque(string ruta, int inicio, int posicion, bool tipo, int soycero);

    void CrearINODO(string ruta, int inicio);
    void CrearINODOInodo(string ruta, int inicio, int posicion, string nombre);
    void CrearINODOBloque(string ruta, int inicio, int posicion, bool tipo, int soycero, string name);

    void CrearBLOCK(string ruta, int inicio);
    void CrearBLOCKInodo(string ruta, int inicio, int posicion);
    void CrearBLOCKBloque(string ruta, int inicio, int posicion, bool tipo, int soycero);

    void CrearBIPMAP(string ruta, int inicio, bool tipo);

    void CrearFILE(string ruta, int inicio, string ruta_carpetas);

    void CrearJornaling(string ruta, int inicio);

    void CrearLS(string ruta, int inicio);
    void CrearLSInodo(string ruta, int inicio, int posicion, string nombre);
    void CrearLSBloque(string ruta, int inicio, int posicion, bool tipo, int soycero);
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
