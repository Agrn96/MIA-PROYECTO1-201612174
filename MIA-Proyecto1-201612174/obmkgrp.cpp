#include "obmkgrp.h"
#include "iostream"
#include "list"
#include "fstream"
#include "obrm.h"
#include "obtouch.h"
#include "obedit.h"
using namespace std;

static int PART_START;
static string RUTA_PARTITTION;

extern structs::LOGIN USUARIO_ONLINE;
extern structs::DiscosMontados Discos_Montados[99];

obmkgrp::obmkgrp()
{

}

void obmkgrp::mostrardatos(obmkgrp *disco){

    cout << "\n     ----COMANDO MKGRP----\n"<<endl;

    this->name = EliminarComillas(this->name);
    if(this->name.length() > 10){
        cout << "EL GRUPO NO PUEDE TENER MAS DE 10 CARACTERES"<<endl;
        return;
    }

    string id = USUARIO_ONLINE.id_particion;
    if(id == "-1"){
        cout << "NO HAY NINGUN USUARIO REGISTRADO"<<endl;
        return;
    }


    if(USUARIO_ONLINE.usuario != "root"){
        cout << "SOLO EL USUARIO root PUEDE AGREGAR GRUPOS"<< endl;
        return;
    }

    //OBTENER ATRIBUTOS DEL DISCO MONTADO
    string Ruta_part = "";
    string Nombre = "";
    int Start_Part = 0;
    int Size_Part = 0;

    //VERIFICAMOS SI EL CODIGO PERTENECE A UNA PARTICION MONTADA
    if(!existelaparticionmontada(Discos_Montados, id, &Ruta_part, &Nombre, &Start_Part, &Size_Part)){
        cout << "NO EXISTE NINGUNA PARTICION CON EL ID: "<< id << endl;
        return;
    }
    RUTA_PARTITTION = Ruta_part;
    PART_START = Start_Part;


    structs::SuperBloque super;
    getSuperBloque(&super, RUTA_PARTITTION, PART_START);

    structs::TabladeInodos Inodo;
    getInodo(&Inodo, RUTA_PARTITTION, super.s_inode_start+ sizeof(structs::TabladeInodos));

    string contenido = "";
    //OBTENEMOS TODO EL CONTENIDO EN LOS BLOQUES
    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] == -1){
            break;
        }
        structs::BloquesdeArchivos Bloque_Archivo;
        getBlock_Files(&Bloque_Archivo, RUTA_PARTITTION, super.s_block_start + Inodo.i_block[i]*sizeof(structs::BloquesdeArchivos));
        string n =Bloque_Archivo.b_content;
        if(n.length() > 64){
           // cout << "MAS 64 -> "<< n<<"-----" <<endl;
            n = n.substr(0,64);
        }
        contenido += n;
        //cout << "NORMAL -> "<< n<<"-----" <<endl;
    }


    //cout<< "CONTENIDO \n" << contenido << " termine" <<endl;

    //-------- SEPARAMOS POR GRUPOS Y USUARIOS LOS DATOS RECOLECTADOR
    list<string> lista_grupos;
    list<structs::GROUP> lista_GROUP;

    string s = contenido;
    string delimiter = "\n";

    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);

        //cout << "-> "<< token << endl;
        int contador = -1;
        string entrada2 = token;
        string delimiter2 = ",";
        size_t pos2 = 0;
        bool EsGrupo = true;
        string token2;
        string aux[10];


        //-----------SEPARAMOS POR COMAS Y GUARDAMOS LOS DATOS
        while ((pos2 = entrada2.find(delimiter2)) != string::npos) {
            contador++;
            token2 = entrada2.substr(0, pos2);
            //cout << "=> "<< token2[0] << " - "<<sizeof(token2[0]) << " -con "<< contador << endl;
            if(token2[0] == 'U' && contador == 1){
                EsGrupo = false;
                //cout << "esta linea es de usuarios"<< endl;
            }
            aux[contador] = token2;
            entrada2.erase(0, pos2 + delimiter2.length());
        }
        //SI ES UN GRUPO REGISTRAMOS
        if(EsGrupo){
            structs::GROUP grupo;
            grupo.posicion = aux[0];
            grupo.nombre = entrada2;
            lista_GROUP.push_back(grupo);
            lista_grupos.push_back(entrada2);
        }
        else{
            //LLENAR LOS GRUPOS
        }
        s.erase(0, pos + delimiter.length());
    }

    int no = 1;
    for(structs::GROUP e: lista_GROUP){
        //cout << "GRUPO: "<<e.nombre <<" posicion" << e.posicion<<endl;
        if(e.nombre == this->name && e.posicion != "0"){
            cout << "EL GRUPO INGRESADO YA EXISTE!!"<<endl;
            return;
        }
        if(e.posicion != "0"){
            no++;
        }
    }



    contenido += to_string(no)+",G,"+this->name+"\n";
    obedit *edit = new obedit();
    edit->RUTA = RUTA_PARTITTION;
    edit->EMPIEZA = PART_START;
    edit->Modificar(super, 1, contenido);

    cout << "PROCESO EXITOSAMENTE!!" <<endl;

}
string obmkgrp::EliminarComillas(string cadena){
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
void obmkgrp::getSuperBloque(structs::SuperBloque *sp, string ruta, int part_Start){
    structs::SuperBloque aux;
    FILE *archivo = fopen(ruta.c_str(), "rb+");
    fseek(archivo, part_Start , SEEK_SET);
    fread(&aux, sizeof(structs::SuperBloque ), 1, archivo);
    fclose(archivo);
    *sp = aux;
}
bool obmkgrp::getInodo(structs::TabladeInodos*sp, string ruta, int part_Start){
    structs::TabladeInodos aux;
    FILE *file = fopen(ruta.c_str(), "rb+");
    if (file != NULL){
        fseek(file, part_Start, SEEK_SET);
        fread(&aux, sizeof(structs::TabladeInodos), 1, file);
        *sp = aux;
    }
    else{
        return false;
    }
    fclose(file);
    return true;
}
bool obmkgrp::getBlock_Files(structs::BloquesdeArchivos *sp, string ruta, int part_Start){
    structs::BloquesdeArchivos aux;
    FILE *file = fopen(ruta.c_str(), "rb+");
    if (file != NULL){
        fseek(file, part_Start, SEEK_SET);
        fread(&aux, sizeof(structs::BloquesdeArchivos), 1, file);
        *sp = aux;
    }
    else{
        return false;
    }
    fclose(file);
    return true;
}
bool obmkgrp::getBlock_Carpets(structs::BloquesdeCarpetas *sp, string ruta, int part_Start){
    structs::BloquesdeCarpetas aux;
    FILE *file = fopen(ruta.c_str(), "rb+");
    if (file != NULL){
        fseek(file, part_Start, SEEK_SET);
        fread(&aux, sizeof(structs::BloquesdeCarpetas), 1, file);
        *sp = aux;
    }
    else{
        return false;
    }
    fclose(file);
    return true;
}
bool obmkgrp::getBlock_Apuntador(structs::BloquesdeApuntadores *sp, string ruta, int part_Start){
    structs::BloquesdeApuntadores aux;
    FILE *file = fopen(ruta.c_str(), "rb+");
    if (file != NULL){
        fseek(file, part_Start, SEEK_SET);
        fread(&aux, sizeof(structs::BloquesdeApuntadores), 1, file);
        *sp = aux;
    }
    else{
        return false;
    }
    fclose(file);
    return true;
}
string obmkgrp::NombreArchivo(string path){
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
list<string> obmkgrp::separar_carpetas(string path) {
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
bool obmkgrp::existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int *size_particion){
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
bool obmkgrp::ExisteMBR(string ruta, structs::MBR *mbr){
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
bool obmkgrp::ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part){
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
bool obmkgrp::ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part){
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


