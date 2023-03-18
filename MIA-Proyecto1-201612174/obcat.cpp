#include "obcat.h"
#include "iostream"
#include "list"
#include "fstream"
#include "obtouch.h"
using namespace std;

static int PART_START;
static string RUTA_PARTITTION;

extern structs::DiscosMontados Discos_Montados[99];
extern structs::LOGIN USUARIO_ONLINE;

obcat::obcat()
{

}


void obcat::mostrardatos(obcat *disco){
     cout << "\n     ----COMANDO CAT----\n"<<endl;

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
        cout << "NO EXISTE NINGUNA PARTICION CON EL ID: "<< id << endl;
        return;
    }

    //cout << "PATH -> "<<Ruta_part << endl;
    //cout << "NOMBRE -> "<<Nombre << endl;
    //cout << "START -> "<<Start_Part << endl;
    //cout << "SIZE -> "<<Size_Part << endl;
    RUTA_PARTITTION = Ruta_part;
    PART_START = Start_Part;

    structs::SuperBloque super;
    getSuperBloque(&super, RUTA_PARTITTION, PART_START);

    cout<< "\n*RECUPERAR INFORMACION DE BLOQUE DE ARCHIVO*\n"<<endl;
    //return;
    //EMPIEZA EL FOR PARA BUSCAR TODAS LAS RUTAS
    string RUTA_A = "";
    for(string e: disco->path){
        RUTA_A = e;
        string Ruta = EliminarComillas(e);
        list<string> carpetas = separar_carpetas(Ruta);
        list<string>::iterator it;
        string NombreAr = NombreArchivo(Ruta);
        carpetas.push_back(NombreAr);

        int contador_inodo = 0;
        int contador_block= 0;

        structs::TabladeInodos Inodo_actual; //RECUPERAMOS EL INO
        bool Encontrelacarpeta;

        for (it = carpetas.begin(); it != carpetas.end(); it++){

            getSuperBloque(&super, RUTA_PARTITTION, PART_START);

            getInodo(&Inodo_actual, RUTA_PARTITTION, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));
            if(Inodo_actual.i_type != 'I'){
                Encontrelacarpeta = false;

                //BUSCAMOS LAS POSICIONES QUE ESTEN DISPONIBLES
                for(int j = 0; j < 15; j++){
                    if(Inodo_actual.i_block[j] != -1){
                        contador_block =Inodo_actual.i_block[j];
                        if(j < 12){
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
                                            if(Encontrelacarpeta){
                                                break;
                                            }
                                        }
                                    }
                                    if(Encontrelacarpeta){
                                        break;
                                    }
                                }
                            }
                        }




                    }
                    if(Encontrelacarpeta){
                        break;
                    }
                }
                if(!Encontrelacarpeta){
                    cout << "LA CARPETA NO EXISTE!-> "<< Ruta << "\n"<< endl;
                    break;
                }
            }
        }
        if(Encontrelacarpeta){
            //VAMOS A RECUPERAR EL BLOQUE
            getInodo(&Inodo_actual, RUTA_PARTITTION, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));
            if(Inodo_actual.i_type == '1'){
                cout << "** FILE : "<< e << endl;
                for(int i = 0; i < 15; i++){
                    if(Inodo_actual.i_block[i] != -1){
                        structs::BloquesdeArchivos tmp_A;
                        getBlock_Files(&tmp_A, RUTA_PARTITTION, super.s_block_start + Inodo_actual.i_block[i]*sizeof(structs::BloquesdeArchivos));

                        if(i<12){
                            string n(tmp_A.b_content);
                            cout<<n;
                        }
                    }
                }
                cout << "\n--- PROCESO EXITOSAMENTE!! ---\n"<<endl;
            }
            else{
                cout << "\nEL ARCHIVO NO EXISTE! -> "<<Ruta << "\n"<< endl;
            }

        }

    }
    if(super.s_filesystem_type == 3){
        obtouch *touch = new obtouch();
        string tipo_o = "cat";
        touch->CrearJornaling(tipo_o, '1', RUTA_A, "", 0, RUTA_PARTITTION, PART_START);
    }

    return;



}


string obcat::EliminarComillas(string cadena){
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

void obcat::getSuperBloque(structs::SuperBloque *sp, string ruta, int part_Start){
    structs::SuperBloque aux;
    FILE *archivo = fopen(ruta.c_str(), "rb+");
    fseek(archivo, part_Start , SEEK_SET);
    fread(&aux, sizeof(structs::SuperBloque ), 1, archivo);
    fclose(archivo);
    *sp = aux;
}

bool obcat::getInodo(structs::TabladeInodos*sp, string ruta, int part_Start){
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
bool obcat::getBlock_Files(structs::BloquesdeArchivos *sp, string ruta, int part_Start){
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
bool obcat::getBlock_Carpets(structs::BloquesdeCarpetas *sp, string ruta, int part_Start){
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
bool obcat::getBlock_Apuntador(structs::BloquesdeApuntadores *sp, string ruta, int part_Start){
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

string obcat::NombreArchivo(string path){
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

list<string> obcat::separar_carpetas(string path) {
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

bool obcat::existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int *size_particion){
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

bool obcat::ExisteMBR(string ruta, structs::MBR *mbr){
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

bool obcat::ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part){
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

bool obcat::ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part){
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


