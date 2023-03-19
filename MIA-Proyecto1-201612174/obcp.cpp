#include "obcp.h"
#include "iostream"
#include "list"
#include "fstream"
#include "obrm.h"
#include "obtouch.h"
using namespace std;

static int PART_START;
static string RUTA_PARTITTION;

extern structs::DiscosMontados Discos_Montados[99];
extern structs::LOGIN USUARIO_ONLINE;

obcp::obcp()
{

}

void obcp::mostrardatos(obcp *disco){
     cout << "\n     ----COMANDO CP----\n"<<endl;
    string Ruta = EliminarComillas(disco->path);
    string Destino = EliminarComillas(this->destino);
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


    list<string> carpetas = separar_carpetas(Ruta);
    list<string>::iterator it;
    string NombreAr = NombreArchivo(Ruta);
    carpetas.push_back(NombreAr);

    int contador_inodo = 0;
    int contador_block= 0;

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
                    //BUSCAMOS BLOQUES DE CARPETAS
                    if(j < 12){
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
            //SI NO EXISTE LA CARPETA
            if(!Encontrelacarpeta){
                cout << "LA CARPETA NO EXISTE!"<< endl;
                return;
            }
        }
    }





    //EMPEZAMOS A BUSCAR LAS RUTAS SI EXISTEN DESPUES DE LA CARPETA
    getSuperBloque(&super, RUTA_PARTITTION, PART_START);
    structs::TabladeInodos Inodo_tmp;
    getInodo(&Inodo_tmp, RUTA_PARTITTION, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));


    this->inodos.clear();
    BuscarInodo(RUTA_PARTITTION, PART_START, contador_inodo, NombreAr);

    //COPIAMOS CREANDO NUEVOS INODOS Y BLOQUES;
    int contador = 0;

    for(structs::COPY e: this->inodos){
        contador++;
        if(contador != sizeof (this->inodos)){
            list<string> carpetas = separar_carpetas(Destino+"/"+e.ruta);
            list<string>::iterator it;
            string NombreAr = NombreArchivo(Destino+"/"+e.ruta);
            carpetas.push_back(NombreAr);
            //cout << "-> "<< Destino+"/"+e.ruta <<  endl;
            int contador_inodo = 0;
            int contador_block= 0;
            int contar = 0;
            for (it = carpetas.begin(); it != carpetas.end(); it++){
                contar++;
                getSuperBloque(&super, RUTA_PARTITTION, PART_START);
                structs::TabladeInodos Inodo_actual; //RECUPERAMOS EL INO
                getInodo(&Inodo_actual, RUTA_PARTITTION, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));
                if(Inodo_actual.i_type != 'I'){
                    bool Encontrelacarpeta = false;

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
                                        if((carpetas.size()) == contar){
                                            continue;
                                        }
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
                                                if((carpetas.size()) == contar){
                                                    continue;
                                                }
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
                                                        if((carpetas.size()) == contar){
                                                            continue;
                                                        }
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
                    string nombre_aux = *it;
                    //SI NO EXISTE LA CARPETA
                    if(!Encontrelacarpeta){
                        string nombre = *it;
                        int recuperar;
                        obtouch *touch = new obtouch();
                        touch->path=RUTA_PARTITTION;
                        touch->BuscarEspacio(super, contador_inodo, contador_block, Inodo_actual, nombre, &recuperar);
                        contador_inodo = recuperar;
                    }
                }
            }

            //CREAMOS EL ARCHIVO SI ES
            if(e.tipo == '1' ){
               string con = "";
               for(int k = 0; k < sizeof(e.contenido); k++){
                   if(e.contenido[k] == '"'){

                   }
                   else{
                       con += e.contenido[k];
                   }
               }
               obtouch *touch = new obtouch();
               touch->path=RUTA_PARTITTION;
               touch->CrearArchivos(super, contador_inodo, con);
            }
        }
    }

    if(super.s_filesystem_type == 3){
        obtouch *touch = new obtouch();
        string tipo_o = "cp";
        touch->CrearJornaling(tipo_o, '0', Ruta, " ", 0, RUTA_PARTITTION, PART_START);
    }
    cout << "PROCESO EXITOSAMENTE"<<endl;

}

void obcp::BuscarInodo(string ruta, int inicio, int posicion, string nombre){

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
                    for(int j = 0; j < 15; j++){
                        if(Inodos.i_block[j] == -1){
                            break;
                        }
                        structs::BloquesdeArchivos Bloque_Archivo;
                        obtouch::getBlock_Files(&Bloque_Archivo, ruta, super.s_block_start + Inodos.i_block[j]*sizeof(structs::BloquesdeArchivos));
                        string ca(Bloque_Archivo.b_content);
                        contenido+= ca;
                    }
                    structs::COPY copiar;
                    copiar.ruta = nombre;
                    copiar.tipo = '1';
                    copiar.contenido = contenido;
                    this->inodos.push_back(copiar);
                }
            }
        }

        return;
}

void obcp::BuscarBloque(string ruta, int inicio, int posicion, bool tipo, int soycero, string name){
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
                    structs::COPY copiar;
                    copiar.ruta = name;
                    copiar.tipo = '0';
                    this->inodos.push_back(copiar);
                    return;
                }
                else{
                    string nombre(Bloque_carpeta.b_content[i].b_name);
                    BuscarInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo, name+"/"+nombre);
                }
            }
        }
        else{
            for(int i = 0; i < 4; i++){
                if(Bloque_carpeta.b_content[i].b_inodo == -1){
                    structs::COPY copiar;
                    copiar.ruta = name;
                    copiar.tipo = '0';
                    this->inodos.push_back(copiar);
                    return;
                }
                else{
                    string nombre(Bloque_carpeta.b_content[i].b_name);
                    BuscarInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo, name+"/"+nombre);
                }
            }
        }
        structs::COPY copiar;
        copiar.ruta = name;
        copiar.tipo = '0';
        this->inodos.push_back(copiar);
        return;

    }
    else{

        //this->inodos.push_back(name);
        return;
    }

    return;
}


string obcp::EliminarComillas(string cadena){
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
void obcp::getSuperBloque(structs::SuperBloque *sp, string ruta, int part_Start){
    structs::SuperBloque aux;
    FILE *archivo = fopen(ruta.c_str(), "rb+");
    fseek(archivo, part_Start , SEEK_SET);
    fread(&aux, sizeof(structs::SuperBloque ), 1, archivo);
    fclose(archivo);
    *sp = aux;
}
bool obcp::getInodo(structs::TabladeInodos*sp, string ruta, int part_Start){
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
bool obcp::getBlock_Files(structs::BloquesdeArchivos *sp, string ruta, int part_Start){
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
bool obcp::getBlock_Carpets(structs::BloquesdeCarpetas *sp, string ruta, int part_Start){
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
bool obcp::getBlock_Apuntador(structs::BloquesdeApuntadores *sp, string ruta, int part_Start){
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
string obcp::NombreArchivo(string path){
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
list<string> obcp::separar_carpetas(string path) {
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
bool obcp::existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int *size_particion){
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
bool obcp::ExisteMBR(string ruta, structs::MBR *mbr){
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
bool obcp::ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part){
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
bool obcp::ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part){
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


