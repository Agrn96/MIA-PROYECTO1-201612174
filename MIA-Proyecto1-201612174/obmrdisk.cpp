#include "obmrdisk.h"
#include "iostream"
using namespace std;

obmrdisk::obmrdisk()
{

}

void obmrdisk::mostrardatos(string path){
     cout << "\n     ----COMANDO RMDISK----\n"<<endl;
    string ruta;

    for(int i = 1; i < path.size()-1; i++){ //ELIMINAR COMILLAS DE LA RUTA
        ruta += path[i];
    }

    if (remove(ruta.c_str()) == 0){
        cout << "Disco eliminado" << endl;
    }
    else{
        cout << "Disco no existente" << endl;
    }
}
