#include "obrecovery.h"
#include "iostream"
#include "list"
#include "fstream"
#include "obrm.h"
#include "obtouch.h"
#include "obmkfs.h"
using namespace std;

static int PART_START;
static string RUTA_PARTITTION;

extern structs::DiscosMontados Discos_Montados[99];
extern structs::LOGIN USUARIO_ONLINE;

obrecovery::obrecovery()
{

}

void obrecovery::mostrardatos(obrecovery *disco){

    this->id = EliminarComillas(this->id);
    string id = this->id;
   //OBTENER ATRIBUTOS DEL DISCO MONTADO
    string Ruta_part = "";
    string Nombre = "";
    int Start_Part = 0;
    int Size_Part = 0;

    //VERIFICAMOS SI EL CODIGO PERTENECE A UNA PARTICION MONTADA
    if(!existelaparticionmontada(Discos_Montados, id, &Ruta_part, &Nombre, &Start_Part, &Size_Part)){
        //cout << "NO EXISTE NINGUNA PARTICION CON EL ID: "<< id << endl;
        return;
    }

    RUTA_PARTITTION = Ruta_part;
    PART_START = Start_Part;
}
void obrecovery::recovery(){
     cout << "\n     ----COMANDO RECOVERY----\n"<<endl;


    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, RUTA_PARTITTION, PART_START);

    if(super.s_filesystem_type == 2){
        cout << "EL SISTEMA ES EXT2, NO SE PUDO RECUPERAR LA INFORMACION"<<endl;
        return;
    }

    for(int i = 0; i < super.s_inodes_count; i++){
        FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
        fseek(bfile, super.s_bm_inode_start+i, SEEK_SET);
        if(i < super.s_firts_ino){
            fwrite("1", 1, 1, bfile);
        }
        else{
            fwrite("0", 1, 1, bfile);
        }
        fclose(bfile);
    }

    for(int i = 0; i < super.s_blocks_count; i++){
        FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
        fseek(bfile, super.s_bm_block_start+i, SEEK_SET);
        if(i < super.s_first_blo){
            fwrite("1", 1, 1, bfile);
        }
        else{
            fwrite("0", 1, 1, bfile);
        }

        fclose(bfile);
    }

    cout << "SE RECUPERO LA INFORMACION DE LA PARTICION: "<<this->id<<endl;

}
void obrecovery::simulateloss(){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, RUTA_PARTITTION, PART_START);
     cout << "\n     ----COMANDO LOSS----\n"<<endl;
    for(int i = 0; i < super.s_inodes_count; i++){
        FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
        fseek(bfile, super.s_bm_inode_start+i, SEEK_SET);
        fwrite("\0", 1, 1, bfile);
        fclose(bfile);
    }

    for(int i = 0; i < super.s_blocks_count; i++){
        FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
        fseek(bfile, super.s_bm_block_start+i, SEEK_SET);
        fwrite("\0", 1, 1, bfile);
        fclose(bfile);
    }

    cout << "SE SIMULO LA PERDIDA DE DATOS"<<endl;
}
string obrecovery::EliminarComillas(string cadena){
    string ruta;
    if(cadena[0] == '"'){
        for(int i = 1; i < cadena.size()-1; i++){ //ELIMINAR COMILLAS DE LA RUTA
            ruta += cadena[i];
        }
    }else{
        ruta = cadena;
    }
    return ruta;
}
string obrecovery::NombreArchivo(string path){
    int contador = 0;
    string cadena = "";
    for(int i = path.length()-1; i >= 0; i--){
        if(path[i] == '/'){
            break;
        }
        else{
            contador++;
        }
    }

    for(int i = path.length() - contador; i < path.length(); i++){
        cadena += path[i];
    }
    return cadena;
}
list<string> obrecovery::separar_carpetas(string path) {
    if (path[0] == '/') {
        path = path.substr(1, path.length());
    }
    list<string> lista_carpetas;
    char ruta[path.length() + 1];
    strcpy(ruta, path.c_str());
    string nombre_carpeta;
    ////cout << "NOMBRE || "<<nombre <<endl;
    for (int i = 0; i < path.length() + 1; i++) {
        if (ruta[i] == '/') {
            lista_carpetas.push_back(nombre_carpeta);
            nombre_carpeta.clear();
            continue;
        }
        nombre_carpeta += ruta[i];
    }
    return lista_carpetas;
}
bool obrecovery::existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int *size_particion){
    for(int i = 0; i < 26; i++){
        if(tmp[i].status == 1){
            for(int j = 0; j < 99; j++){
                if(tmp[i].partittion[j].status == 1){
                    string id_aux(std::to_string(tmp[i].number));
                    id_aux += tmp[i].partittion[j].letter;
                    if(id_aux == codigo){

                        *ruta = tmp[i].path;
                        *nombre = tmp[i].partittion[j].nombre;

                        structs::MBR tmp_disco;
                        if(!ExisteMBR(tmp[i].path, &tmp_disco)){
                           return false;
                        }
                        int start1;
                        int size1;
                        int Ext;

                        if(!ExisteNombre(tmp_disco, tmp[i].partittion[j].nombre, &Ext, &start1, &size1)){
                            *start_particion = start1;
                            *size_particion = size1;
                            return true;
                        }
                        else if(Ext != 0){
                            if(ExisteNombreLogica(tmp[i].path, tmp[i].partittion[j].nombre, Ext, &start1, &size1 )){
                                *start_particion = start1;
                                *size_particion = size1;
                                return true;
                            }
                            else{
                                return false;
                            }
                        }
                        else{
                            return false;
                        }

                    }
                }
            }
        }
    }
    return false;
}
bool obrecovery::ExisteMBR(string ruta, structs::MBR *mbr){
    structs::MBR tmp_disco;
    FILE *file = fopen(ruta.c_str(), "rb+");
    if (file != NULL){
        fseek(file, 0, SEEK_SET);
        fread(&tmp_disco, sizeof(tmp_disco), 1, file); //RECUPERAR LOS DATOS DEL DISCO
    }
    else{
        return false;
    }
    fclose(file);
    *mbr = tmp_disco;
    return true;
}
bool obrecovery::ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part){
    *Ext = 0;
    ////cout<<"primera busqueda"<<endl;
    for(int i = 0; i<4; i++){
        if(tmp_MBR.mbr_partitions[i].type == 'E'){
            *Ext=tmp_MBR.mbr_partitions[i].part_start;
            ////cout<<"encontre extendida"<<endl;
        }
        if(tmp_MBR.mbr_partitions[i].name == name){
            ////cout<<"encontre particion ---"<<endl;
            *start_part = tmp_MBR.mbr_partitions[i].part_start;
            *size_part = tmp_MBR.mbr_partitions[i].size;
            return false;
        }
    }
    return true;
}
bool obrecovery::ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part){
    ////cout<<"busque logica "<< part_start_ext <<endl;
    bool ExisteSiguiente = true;
    int contador = 0;
    int siguiente = part_start_ext;
    structs::EBR tmp_EBRS[50];
    structs::EBR aux_ebr;

    //RECUPERAMOS LOS EBRS DEL DISCO
    while(ExisteSiguiente){
        FILE *archivo = fopen(ruta.c_str(), "rb+");
        fseek(archivo, siguiente , SEEK_SET);
        fread(&aux_ebr, sizeof(structs::EBR), 1, archivo);
        fclose(archivo);

        tmp_EBRS[contador] = aux_ebr;
        if(siguiente <= 0 && tmp_EBRS[contador].status != '3'){
            ExisteSiguiente = false;
            contador--;
            break;
        }
        siguiente = tmp_EBRS[contador].part_next;
        contador++;
    }
    ////cout << "pase aqui" <<endl;
    //BUSCAMOS SI EXISTE EL NOMBRE
    for(int i = 0; i < 50; i++){
        ////cout << tmp_EBRS[i].status << "<<<< "<<endl;
        if((tmp_EBRS[i].status == '1') || (tmp_EBRS[i].status == '0')){
            ////cout << tmp_EBRS[i].name << " == "<< name <<endl;
            if(tmp_EBRS[i].name == name){
                ////cout << tmp_EBRS[i].part_start << " -> "<<tmp_EBRS[i].size<<endl;
                *start_part = tmp_EBRS[i].part_start;
                *size_part = tmp_EBRS[i].size;
                return true;
            }
        }
    }
    return false;
}

