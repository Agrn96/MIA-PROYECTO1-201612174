#include "obfind.h"
#include "iostream"
#include "list"
#include "fstream"
#include "obrm.h"
#include "obtouch.h"
#include "regex"
#include "regex.h"
using namespace std;

static int PART_START;
static string RUTA_PARTITTION;

extern structs::DiscosMontados Discos_Montados[99];
extern structs::LOGIN USUARIO_ONLINE;

obfind::obfind()
{

}

void obfind::mostrardatos(obfind *disco){
     cout << "\n     ----COMANDO FIND----\n"<<endl;
     this->name = EliminarComillas(this->name);
    string Ruta = EliminarComillas(disco->path);
    disco->path = "";

    string id = USUARIO_ONLINE.id_particion;
    if(id == "-1"){
        cout << "NO HAY NINGUN USUARIO REGISTRADO"<<endl;
        return;
    }
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
    structs::SuperBloque super;
    getSuperBloque(&super, RUTA_PARTITTION, PART_START);




    int contador_inodo = 0;
    int contador_block= 0;

    if(Ruta != "/"){
        list<string> carpetas = separar_carpetas(Ruta);
        list<string>::iterator it;
        string NombreAr = NombreArchivo(Ruta);
        carpetas.push_back(NombreAr);

        for (it = carpetas.begin(); it != carpetas.end(); it++){
            getSuperBloque(&super, RUTA_PARTITTION, PART_START);
            structs::TabladeInodos Inodo_actual; //RECUPERAMOS EL INO
            getInodo(&Inodo_actual, RUTA_PARTITTION, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));
            if(Inodo_actual.i_type != 'I'){
                bool Encontrelacarpeta = false;

                //BUSCAMOS LAS POSICIONES QUE ESTEN DISPONIBLES
                for(int j = 0; j < 15; j++){
                    if(Inodo_actual.i_block[j] != -1){

                        contador_block =Inodo_actual.i_block[j];
                        if( j < 12 ){
                            //BUSCAMOS BLOQUES DE CARPETAS
                            structs::BloquesdeCarpetas BloqueCarpeta_Actual;
                            getBlock_Carpets(&BloqueCarpeta_Actual, RUTA_PARTITTION, super.s_block_start + contador_block*sizeof(structs::BloquesdeCarpetas));

                            //NAVEGANDO ENTRE LOS ESPACIOS
                            for(int k = 0; k < 4; k++){
                               if(BloqueCarpeta_Actual.b_content[k].b_name == *it){
                                    contador_inodo = BloqueCarpeta_Actual.b_content[k].b_inodo;
                                    contador_block = Inodo_actual.i_block[j];
                                    Encontrelacarpeta = true;
                                    break;
                                }
                            }
                        }
                        else if(j == 12){
                            //BUSCAMOS EN PRIMER INDIRECTO
                            structs::BloquesdeApuntadores B_A1;
                            getBlock_Apuntador(&B_A1, RUTA_PARTITTION, super.s_block_start + Inodo_actual.i_block[j]*sizeof(structs::BloquesdeApuntadores));
                            for(int k = 0; k < 16; k++){
                                if(B_A1.b_pointers[k] != -1){
                                    structs::BloquesdeCarpetas BloqueCarpeta_Actual;
                                    getBlock_Carpets(&BloqueCarpeta_Actual, RUTA_PARTITTION, super.s_block_start + B_A1.b_pointers[k] *sizeof(structs::BloquesdeCarpetas));
                                    //cout << "BLOQUE APUNTADOR111 -> "<<B_A1.b_pointers[k]<<endl;
                                    contador_block = B_A1.b_pointers[k];
                                    for(int l = 0; l < 4; l++){

                                        if(BloqueCarpeta_Actual.b_content[l].b_name == *it){
                                            //cout << "ENCONTRE LA CARPETA CON EL NOMBRE"<< endl;
                                            contador_inodo = BloqueCarpeta_Actual.b_content[l].b_inodo;
                                            contador_block = B_A1.b_pointers[k];
                                            Encontrelacarpeta = true;
                                            break;
                                        }
                                    }
                                    if(Encontrelacarpeta){
                                        break;
                                    }
                                }
                            }
                        }
                        else if(j == 13){
                            structs::BloquesdeApuntadores B_A1;
                            getBlock_Apuntador(&B_A1, RUTA_PARTITTION, super.s_block_start + Inodo_actual.i_block[j]*sizeof(structs::BloquesdeApuntadores));
                            for(int k = 0; k < 16; k++){
                                if(B_A1.b_pointers[k] != -1){
                                    structs::BloquesdeApuntadores B_A2;
                                    getBlock_Apuntador(&B_A2, RUTA_PARTITTION, super.s_block_start + B_A1.b_pointers[k]*sizeof(structs::BloquesdeApuntadores));
                                    //contador_block = B_A1.b_pointers[k];
                                    for(int l = 0; l < 16; l++){
                                        if(B_A2.b_pointers[l] != -1){
                                            structs::BloquesdeCarpetas BloqueCarpeta_Actual;
                                            getBlock_Carpets(&BloqueCarpeta_Actual, RUTA_PARTITTION, super.s_block_start + B_A2.b_pointers[l]*sizeof(structs::BloquesdeCarpetas));
                                            contador_block = B_A2.b_pointers[l];
                                            for(int m = 0; m < 4; m++){
                                                if(BloqueCarpeta_Actual.b_content[m].b_name == *it){
                                                    contador_inodo = BloqueCarpeta_Actual.b_content[m].b_inodo;
                                                    contador_block = B_A2.b_pointers[l];
                                                    Encontrelacarpeta = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    if(Encontrelacarpeta){
                                        break;
                                    }
                                }
                                if(Encontrelacarpeta){
                                    break;
                                }
                            }
                        }


                    }
                    if(Encontrelacarpeta){
                        break;
                    }
                }
                //SI NO EXISTE LA CARPETA
                if(!Encontrelacarpeta){
                    cout << "LA CARPETA NO EXISTE!"<< endl;
                    return;
                }
            }
        }
    }


    //EMPEZAMOS LA RECURSIVIDAD
    cout << "INODO -> "<< contador_inodo << endl;
    cout <<  "BLOCK -> "<< contador_block << endl;
    cout <<  "CARACTER -> "<< this->caracter << endl;

    BuscarInodo(RUTA_PARTITTION, PART_START, contador_inodo, Ruta);


    cout << "PROCESO EXITOSAMENTE"<<endl;

}


string obfind::estrucutrar_regex(string name){
    string regex = "^(";
    for (char i : name) {
        if(i == '*'){
            regex += "([a-zA-Z0-9]+)";
        } else if (i == '?'){
            regex += "([a-zA-Z0-9]{1})";
        } else {
            regex += "[" ;
            regex += i;
            regex += "]";
        }
    }
    regex += ")$";

    return regex;
}

bool obfind::Existe_Union(string nombre, string regex){

    std::regex word_regex(regex);
    auto words_begin = std::sregex_iterator(nombre.begin(), nombre.end(), word_regex);
    auto words_end = std::sregex_iterator();

    if(distance(words_begin, words_end) == 1){
        return true;
    }
    return false;
}

void obfind::BuscarInodo(string ruta, int inicio, int posicion, string nombre){

        structs::SuperBloque super;
        obtouch::getSuperBloque(&super, ruta, inicio);

        structs::TabladeInodos Inodos;
        obtouch::getInodo(&Inodos, ruta, super.s_inode_start + posicion*sizeof(structs::TabladeInodos));
        string tipo;
        string contenido = "";

        for(int i = 0; i < 15; i++){
            if(Inodos.i_block[i] == -1){
                return;
            }
            else{
                if(Inodos.i_type == '0'){
                    BuscarBloque(ruta, inicio, Inodos.i_block[i], true, i, nombre);
                }
                else{
                    //SI EL INODO ES TIPO ARCHIVO
                    for(int j = 0; j < 15; j++){
                        if(Inodos.i_block[j] == -1){
                            break;
                        }
                    }
                }
            }
        }

        return;
}

void obfind::BuscarBloque(string ruta, int inicio, int posicion, bool tipo, int soycero, string name){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    if(name == "/"){
        name="";
    }

    if(tipo){
        structs::BloquesdeCarpetas Bloque_carpeta;
        obtouch::getBlock_Carpets(&Bloque_carpeta, ruta, super.s_block_start + posicion*sizeof(structs::BloquesdeCarpetas));



        if(soycero == 0){

            for(int i = 2; i < 4; i++){
                if(Bloque_carpeta.b_content[i].b_inodo == -1){
                    return;
                }
                else{
                    string nombre(Bloque_carpeta.b_content[i].b_name);
                    //cout << "name -> " << nombre << endl;
                    if(Existe_Union(nombre, estrucutrar_regex(this->name))){
                        structs::TabladeInodos Inodos;
                        obtouch::getInodo(&Inodos, ruta, super.s_inode_start + Bloque_carpeta.b_content[i].b_inodo*sizeof(structs::TabladeInodos));
                        string Tipo;
                        if(Inodos.i_type == '0'){
                            Tipo = "Carpeta";
                        }
                        else{
                            Tipo = "Archivo";
                        }
                        cout << name+"/"+nombre << "|"<< to_string(Bloque_carpeta.b_content[i].b_inodo) << "|" << Tipo << endl;
                    }
                    BuscarInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo, name+"/"+nombre);
                }
            }
        }
        else{
            for(int i = 0; i < 4; i++){
                if(Bloque_carpeta.b_content[i].b_inodo == -1){
                    return;
                }
                else{
                    string nombre(Bloque_carpeta.b_content[i].b_name);
                    if(Existe_Union(nombre, estrucutrar_regex(this->name))){
                        structs::TabladeInodos Inodos;
                        obtouch::getInodo(&Inodos, ruta, super.s_inode_start + Bloque_carpeta.b_content[i].b_inodo*sizeof(structs::TabladeInodos));
                        string Tipo;
                        if(Inodos.i_type == '0'){
                            Tipo = "Carpeta";
                        }
                        else{
                            Tipo = "Archivo";
                        }
                        cout << name+"/"+nombre << "|"<< to_string(Bloque_carpeta.b_content[i].b_inodo) << "|" << Tipo << endl;
                    }
                    //cout << "name -> " << nombre << endl;
                    BuscarInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo, name+"/"+nombre);
                }
            }
        }
        return;

    }
    else{

        //this->inodos.push_back(name);
        return;
    }

    return;
}


string obfind::EliminarComillas(string cadena){
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
void obfind::getSuperBloque(structs::SuperBloque *sp, string ruta, int part_Start){
    structs::SuperBloque aux;
    FILE *archivo = fopen(ruta.c_str(), "rb+");
    fseek(archivo, part_Start , SEEK_SET);
    fread(&aux, sizeof(structs::SuperBloque ), 1, archivo);
    fclose(archivo);
    *sp = aux;
}
bool obfind::getInodo(structs::TabladeInodos*sp, string ruta, int part_Start){
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
bool obfind::getBlock_Files(structs::BloquesdeArchivos *sp, string ruta, int part_Start){
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
bool obfind::getBlock_Carpets(structs::BloquesdeCarpetas *sp, string ruta, int part_Start){
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
bool obfind::getBlock_Apuntador(structs::BloquesdeApuntadores *sp, string ruta, int part_Start){
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
string obfind::NombreArchivo(string path){
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
list<string> obfind::separar_carpetas(string path) {
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
bool obfind::existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int *size_particion){
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
bool obfind::ExisteMBR(string ruta, structs::MBR *mbr){
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
bool obfind::ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part){
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
bool obfind::ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part){
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


