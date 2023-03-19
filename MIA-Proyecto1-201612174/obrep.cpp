#include "obrep.h"
#include <QString>
#include "iostream"
#include "structs.h"
#include "string"
#include <list>
#include "obtouch.h"
#include <fstream>
using namespace std;

extern structs::DiscosMontados Discos_Montados[99];
obrep::obrep()
{

}


void obrep::mostrardatos(obrep *disco){
     cout << "\n     ----COMANDO REP----\n"<<endl;
    string Metodo = ToUpperString(disco->name);
    this->path = EliminarComillas(this->path);
    this->ruta_file = EliminarComillas(this->ruta_file);


    string Ruta = "";
    string Nombre = "";
    int Start_Part = 0;
    int Size_Part = 0;
    //cout << "-> "<< disco->id <<endl;
    //VERIFICAMOS SI EL CODIGO PERTENECE A UNA PARTICION MONTADA
    if(!existelaparticionmontada(Discos_Montados, disco->id, &Ruta, &Nombre, &Start_Part, &Size_Part)){
        cout << "NO EXISTE NINGUNA PARTICION CON EL ID: "<< disco->id << endl;
        return;
    }
    /*cout << "PATH -> "<<Ruta << endl;
    cout << "NOMBRE -> "<<Nombre << endl;
    cout << "START -> "<<Start_Part << endl;
    cout << "SIZE -> "<<Size_Part << endl;*/

    if(Metodo == "TREE"){
        //CrearTree(Ruta, Start_Part);
        CrearTreeRoot(Ruta, Start_Part);
    }
    else if(Metodo == "MBR"){
        CrearTablaMBR(Ruta);
    }
    else if(Metodo == "DISK"){
        CrearDisk(Ruta);
    }
    else if(Metodo == "SB"){
        CrearSB(Ruta, Start_Part);
    }
    else if(Metodo == "INODO"){
        CrearINODO(Ruta, Start_Part);
    }
    else if(Metodo == "BLOCK"){
        CrearBLOCK(Ruta, Start_Part);
    }
    else if(Metodo == "BM_INODE"){
        CrearBIPMAP(Ruta, Start_Part, true);
    }
    else if(Metodo == "BM_BLOCK"){
        CrearBIPMAP(Ruta, Start_Part, false);
    }
    else if(Metodo == "FILE"){
        CrearFILE(Ruta, Start_Part, this->ruta_file);
    }
    else if(Metodo == "JOURNALING"){
        CrearJornaling(Ruta, Start_Part);
    }
    else if(Metodo == "LS"){
        CrearLS(Ruta, Start_Part);
    }
    return;
}

void obrep::CrearTree(string ruta, int inicio){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);
    list<string> lista1;
    int numero_inodos = super.s_firts_ino;
    int numero_blocks = super.s_first_blo;

    string contenido_dot = "digraph g{\n rankdir=LR; \n";
    for(int i = 0; i < numero_inodos; i++){
        structs::TabladeInodos Inodos;
        obtouch::getInodo(&Inodos, ruta, super.s_inode_start + i*sizeof(structs::TabladeInodos));
        string bloque_dot = "";

        char tmp[60];
        strftime(tmp, 20, "%d/%m/%Y %H:%M", localtime(&Inodos.i_ctime));
        string date_c = CharToString(tmp);
        char tmp1[60];
        strftime(tmp1, 20, "%d/%m/%Y %H:%M", localtime(&Inodos.i_mtime));
        string date_m = CharToString(tmp1);

        contenido_dot += "I"+to_string(i)+"[ shape=plaintext label=<  \n" +
                            "<table border='0' cellborder='1' cellspacing='0'> \n"+
                            "<tr><td bgcolor="+'"'+"lightblue"+'"'+">INODO</td><td bgcolor="+'"'+"lightblue"+'"'+">"+to_string(i) +"</td></tr> \n"+
                            "<tr><td>UID:</td><td>"+to_string(Inodos.i_uid) +"</td></tr> \n"+
                            "<tr><td>GUID:</td><td>"+to_string(Inodos.i_gid) +"</td></tr> \n"+
                            "<tr><td>SIZE:</td><td>"+to_string(Inodos.i_size) +"</td></tr> \n"+
                            "<tr><td>CREACION:</td><td>"+date_c +"</td></tr> \n"+
                            "<tr><td>MODIFICACION:</td><td>"+date_m+"</td></tr> \n";

        for(int j = 0; j < 15; j++){
            contenido_dot +="<tr><td>BLOCK"+to_string(j)+"</td><td port='BS"+to_string(j)+"'>"+to_string(Inodos.i_block[j])+"</td></tr> \n";
            string cadena;
            list<string> aux;
            aux = CrearBloque(super, i, Inodos.i_block[j] , Inodos.i_type ,ruta, &cadena);
            for(string y : aux){
                lista1.push_back(y);
            }
            bloque_dot += cadena;

            if(Inodos.i_block[j] != -1){
                string cadena = "I"+to_string(i)+":BS"+to_string(j)+"->B"+to_string(Inodos.i_block[j])+";";
                lista1.push_back(cadena);
            }
        }


        string tipo(1, Inodos.i_type);
        contenido_dot += "<tr><td>TIPO:</td><td>"+ tipo +"</td></tr>"+
                            "</table>> ]; \n";

        contenido_dot += bloque_dot;


    }



    for(string n : lista1){
        //cout << " -> -> "<< n<<endl;
        contenido_dot += n;
    }

    contenido_dot +="}";
    archivo_dot(contenido_dot, "reporte");
    return;

}

void obrep::CrearTreeRoot(string ruta, int inicio){

    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    this->inodos.clear();
    this->bloques.clear();
    this->nodos.clear();

    int inicio_inodo = this->root;
    this->root = 0;

    CrearInodo(ruta, inicio, inicio_inodo);



    string contenido_dot = "digraph g{\n rankdir=LR; \n";

    for(string e: this->inodos){
        //cout << "" << e << endl;
        contenido_dot += "\n"+e;
    }

    for(string e: this->bloques){
        //cout << "" << e << endl;
        contenido_dot += "\n"+e;
    }

    for(string e: this->nodos){
        //cout << "" << e << endl;
        contenido_dot += "\n"+e;
    }

    contenido_dot +="}";

    char numero;
    FILE *bfile = fopen(ruta.c_str(), "rb+");
    fseek(bfile, super.s_bm_inode_start, SEEK_SET);
    fread(&numero, 1, 1, bfile);
    fclose(bfile);

    char eli;
    if(numero != '1'){
        contenido_dot = "digraph g{\n rankdir=LR; \n}";
    }
    archivo_dot(contenido_dot, "reporte");
    return;
}

void obrep::CrearInodo(string ruta, int inicio, int posicion){

    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    structs::TabladeInodos Inodos;
    obtouch::getInodo(&Inodos, ruta, super.s_inode_start + posicion*sizeof(structs::TabladeInodos));

    char tmp[60];
    strftime(tmp, 20, "%d/%m/%Y %H:%M", localtime(&Inodos.i_ctime));
    string date_c = CharToString(tmp);
    char tmp1[60];
    strftime(tmp1, 20, "%d/%m/%Y %H:%M", localtime(&Inodos.i_mtime));
    string date_m = CharToString(tmp1);

    string contenido_dot = "I"+to_string(posicion)+"[ shape=plaintext label=<  \n";
    contenido_dot += "<table border='0' cellborder='1' cellspacing='0'> \n";
    contenido_dot += "<tr><td bgcolor=\"lightblue\">INODO</td><td bgcolor=\"lightblue\">"+to_string(posicion) +"</td></tr> \n";
    contenido_dot += "<tr><td>UID:</td><td>"+to_string(Inodos.i_uid) +"</td></tr> \n";
    contenido_dot += "<tr><td>GUID:</td><td>"+to_string(Inodos.i_gid) +"</td></tr> \n";
    contenido_dot += "<tr><td>SIZE:</td><td>"+to_string(Inodos.i_size) +"</td></tr> \n";
    contenido_dot +=  "<tr><td>CREACION:</td><td>"+date_c +"</td></tr> \n";
    contenido_dot += "<tr><td>MODIFICACION:</td><td>"+date_m+"</td></tr> \n";

    for(int i = 0; i < 15; i++){

        if(Inodos.i_block[i] == -1){
            for(int j = 0; j < 15-i; j++){
                contenido_dot +="<tr><td>BLOCK"+to_string(j+i)+"</td><td port='BS"+to_string(j+i)+"'>"+to_string(Inodos.i_block[j+i])+"</td></tr> \n";
            }
            string tipo(1, Inodos.i_type);
            contenido_dot += "<tr><td>TIPO:</td><td>"+ tipo +"</td></tr>"+
                                "</table>> ]; \n";
            this->inodos.push_back(contenido_dot);
            return;
        }
        else{
            if(i < 12){
                string cadena = "I"+to_string(posicion)+":BS"+to_string(i)+"->B"+to_string(Inodos.i_block[i])+"; \n";
                this->nodos.push_back(cadena);
                if(Inodos.i_type == '0'){
                    contenido_dot +="<tr><td>BLOCK"+to_string(i)+"</td><td port='BS"+to_string(i)+"'>"+to_string(Inodos.i_block[i])+"</td></tr> \n";
                    CrearBloque(ruta, inicio, Inodos.i_block[i], true, i);
                }
                else{
                    contenido_dot +="<tr><td>BLOCK"+to_string(i)+"</td><td port='BS"+to_string(i)+"'>"+to_string(Inodos.i_block[i])+"</td></tr> \n";
                    CrearBloque(ruta, inicio, Inodos.i_block[i], false, i);
                }
            }
            else if(i == 12){
                contenido_dot +="<tr><td>BLOCK"+to_string(i)+"</td><td port='BS"+to_string(i)+"'>"+to_string(Inodos.i_block[i])+"</td></tr> \n";
                string cadena = "I"+to_string(posicion)+":BS"+to_string(i)+"->B"+to_string(Inodos.i_block[i])+"; \n";
                this->nodos.push_back(cadena);
                structs::BloquesdeApuntadores bloque_apuntador;
                obtouch::getBlock_Apuntador(&bloque_apuntador, ruta, super.s_block_start + Inodos.i_block[i]*sizeof(structs::BloquesdeApuntadores));

                string contenido = "B"+to_string(Inodos.i_block[i])+"[ shape=plaintext label=<  \n";
                contenido += "<table border='0' cellborder='1' cellspacing='0'> \n";
                contenido += "<tr><td bgcolor=\"orange\">BLOCK "+to_string(Inodos.i_block[i]) +"</td></tr> \n";

                for(int j = 0; j < 15; j++){
                    if(bloque_apuntador.b_pointers[j] == -1){
                        for(int k = 0; k < 15-j; k++){
                            contenido +="<tr><td port='BA"+to_string(bloque_apuntador.b_pointers[j+k])+"B"+to_string(j+k)+"'>"+to_string(bloque_apuntador.b_pointers[j+k])+"</td></tr> \n";
                        }
                        break;
                    }
                    else{
                        contenido +="<tr><td port='BA"+to_string(bloque_apuntador.b_pointers[j])+"B"+to_string(j)+"'>"+to_string(bloque_apuntador.b_pointers[j])+"</td></tr> \n";
                        string cadena = "B"+to_string(Inodos.i_block[i])+":BA"+to_string(bloque_apuntador.b_pointers[j])+"B"+to_string(j)+"->B"+to_string(bloque_apuntador.b_pointers[j])+";";
                        this->nodos.push_back(cadena);
                        if(Inodos.i_type == '0'){
                            CrearBloque(ruta, inicio, bloque_apuntador.b_pointers[j], true, i);
                        }
                        else{
                            CrearBloque(ruta, inicio, bloque_apuntador.b_pointers[j], false, i);
                        }

                    }
                }
                contenido += "</table>> ]; \n";
                this->bloques.push_back(contenido);

            }
            else if(i == 13){
                contenido_dot +="<tr><td>BLOCK"+to_string(i)+"</td><td port='BS"+to_string(i)+"'>"+to_string(Inodos.i_block[i])+"</td></tr> \n";
                string cadena = "I"+to_string(posicion)+":BS"+to_string(i)+"->B"+to_string(Inodos.i_block[i])+"; \n";
                this->nodos.push_back(cadena);

                structs::BloquesdeApuntadores bloque_apuntador2;
                obtouch::getBlock_Apuntador(&bloque_apuntador2, ruta, super.s_block_start + Inodos.i_block[i]*sizeof(structs::BloquesdeApuntadores));


                string contenido2 = "B"+to_string(Inodos.i_block[i])+"[ shape=plaintext label=<  \n";
                contenido2 += "<table border='0' cellborder='1' cellspacing='0'> \n";
                contenido2 += "<tr><td bgcolor=\"orange\">BLOCK "+to_string(Inodos.i_block[i]) +"</td></tr> \n";

                for(int k = 0; k < 16; k++){
                    structs::BloquesdeApuntadores bloque_apuntador;
                    obtouch::getBlock_Apuntador(&bloque_apuntador, ruta, super.s_block_start + bloque_apuntador2.b_pointers[k]*sizeof(structs::BloquesdeApuntadores));

                    if(bloque_apuntador2.b_pointers[k] != -1){
                        string cadena = "B"+to_string(Inodos.i_block[i])+":BA"+to_string(bloque_apuntador2.b_pointers[k])+"B"+to_string(k)+"->B"+to_string(bloque_apuntador2.b_pointers[k])+";";
                        this->nodos.push_back(cadena);

                        contenido2 +="<tr><td port='BA"+to_string(bloque_apuntador2.b_pointers[k])+"B"+to_string(k)+"'>"+to_string(bloque_apuntador2.b_pointers[k])+"</td></tr> \n";


                        string contenido = "B"+to_string(bloque_apuntador2.b_pointers[k])+"[ shape=plaintext label=<  \n";
                        contenido += "<table border='0' cellborder='1' cellspacing='0'> \n";
                        contenido += "<tr><td bgcolor=\"orange\">BLOCK "+to_string(bloque_apuntador2.b_pointers[k]) +"</td></tr> \n";

                        for(int j = 0; j < 16; j++){
                            if(bloque_apuntador.b_pointers[j] == -1){
                                for(int l = 0; l < 16-j; l++){
                                    contenido +="<tr><td port='BA"+to_string(bloque_apuntador.b_pointers[j+l])+"B"+to_string(j+l)+"'>"+to_string(bloque_apuntador.b_pointers[j+l])+"</td></tr> \n";
                                }
                                break;
                            }
                            else{
                                contenido +="<tr><td port='BA"+to_string(bloque_apuntador.b_pointers[j])+"B"+to_string(j)+"'>"+to_string(bloque_apuntador.b_pointers[j])+"</td></tr> \n";
                                string cadena = "B"+to_string(bloque_apuntador2.b_pointers[k])+":BA"+to_string(bloque_apuntador.b_pointers[j])+"B"+to_string(j)+"->B"+to_string(bloque_apuntador.b_pointers[j])+";";
                                this->nodos.push_back(cadena);

                                if(Inodos.i_type == '0'){
                                    CrearBloque(ruta, inicio, bloque_apuntador.b_pointers[j], true, i);
                                }
                                else{
                                    CrearBloque(ruta, inicio, bloque_apuntador.b_pointers[j], false, i);
                                }

                            }
                        }
                        contenido += "</table>> ]; \n";
                        this->bloques.push_back(contenido);

                    }
                    else{
                        //cout << "---> "<< bloque_apuntador2.b_pointers[k]<<endl;
                        if(bloque_apuntador2.b_pointers[k] == -1){
                            for(int l = 0; l < 16-k; l++){
                                contenido2 +="<tr><td port='BA"+to_string(bloque_apuntador2.b_pointers[k+l])+"B"+to_string(k+l)+"'>"+to_string(bloque_apuntador2.b_pointers[k+l])+"</td></tr> \n";
                            }
                            break;
                        }
                    }
                }
                contenido2 += "</table>> ]; \n";
                this->bloques.push_back(contenido2);
            }

            else if(i == 14){
                contenido_dot +="<tr><td>BLOCK"+to_string(i)+"</td><td port='BS"+to_string(i)+"'>"+to_string(Inodos.i_block[i])+"</td></tr> \n";
                string cadena = "I"+to_string(posicion)+":BS"+to_string(i)+"->B"+to_string(Inodos.i_block[i])+"; \n";
                this->nodos.push_back(cadena);

                structs::BloquesdeApuntadores bloque_apuntador3;
                obtouch::getBlock_Apuntador(&bloque_apuntador3, ruta, super.s_block_start + Inodos.i_block[i]*sizeof(structs::BloquesdeApuntadores));


                string contenido3 = "B"+to_string(Inodos.i_block[i])+"[ shape=plaintext label=<  \n";
                contenido3 += "<table border='0' cellborder='1' cellspacing='0'> \n";
                contenido3 += "<tr><td bgcolor=\"orange\">BLOCK "+to_string(Inodos.i_block[i]) +"</td></tr> \n";

                for(int m = 0; m < 16; m++){
                    structs::BloquesdeApuntadores bloque_apuntador2;
                    obtouch::getBlock_Apuntador(&bloque_apuntador2, ruta, super.s_block_start + bloque_apuntador3.b_pointers[m]*sizeof(structs::BloquesdeApuntadores));

                    if(bloque_apuntador3.b_pointers[m] != -1){
                        /*string cadena = "B"+to_string(Inodos.i_block[i])+":BA"+to_string(bloque_apuntador3.b_pointers[m])+"B"+to_string(m)+"->B"+to_string(bloque_apuntador3.b_pointers[m])+";";
                        this->nodos.push_back(cadena);

                        contenido3 +="<tr><td port='BA"+to_string(bloque_apuntador3.b_pointers[m])+"B"+to_string(m)+"'>"+to_string(bloque_apuntador2.b_pointers[m])+"</td></tr> \n";

                        //AQUI COMIENZAN SEGUNDO
                        string contenido2 = "B"+to_string(Inodos.i_block[i])+"[ shape=plaintext label=<  \n";
                        contenido2 += "<table border='0' cellborder='1' cellspacing='0'> \n";
                        contenido2 += "<tr><td bgcolor=\"orange\">BLOCK "+to_string(Inodos.i_block[i]) +"</td></tr> \n";

                        for(int k = 0; k < 16; k++){
                            structs::BloquesdeApuntadores bloque_apuntador;
                            obtouch::getBlock_Apuntador(&bloque_apuntador, ruta, super.s_block_start + bloque_apuntador2.b_pointers[k]*sizeof(structs::BloquesdeApuntadores));

                            if(bloque_apuntador2.b_pointers[k] != -1){
                                string cadena = "B"+to_string(Inodos.i_block[i])+":BA"+to_string(bloque_apuntador2.b_pointers[k])+"B"+to_string(k)+"->B"+to_string(bloque_apuntador2.b_pointers[k])+";";
                                this->nodos.push_back(cadena);

                                contenido2 +="<tr><td port='BA"+to_string(bloque_apuntador2.b_pointers[k])+"B"+to_string(k)+"'>"+to_string(bloque_apuntador2.b_pointers[k])+"</td></tr> \n";


                                string contenido = "B"+to_string(bloque_apuntador2.b_pointers[k])+"[ shape=plaintext label=<  \n";
                                contenido += "<table border='0' cellborder='1' cellspacing='0'> \n";
                                contenido += "<tr><td bgcolor=\"orange\">BLOCK "+to_string(bloque_apuntador2.b_pointers[k]) +"</td></tr> \n";

                                for(int j = 0; j < 16; j++){
                                    if(bloque_apuntador.b_pointers[j] == -1){
                                        for(int l = 0; l < 16-j; l++){
                                            contenido +="<tr><td port='BA"+to_string(bloque_apuntador.b_pointers[j+l])+"B"+to_string(j+l)+"'>"+to_string(bloque_apuntador.b_pointers[j+l])+"</td></tr> \n";
                                        }
                                        break;
                                    }
                                    else{
                                        contenido +="<tr><td port='BA"+to_string(bloque_apuntador.b_pointers[j])+"B"+to_string(j)+"'>"+to_string(bloque_apuntador.b_pointers[j])+"</td></tr> \n";
                                        string cadena = "B"+to_string(bloque_apuntador2.b_pointers[k])+":BA"+to_string(bloque_apuntador.b_pointers[j])+"B"+to_string(j)+"->B"+to_string(bloque_apuntador.b_pointers[j])+";";
                                        this->nodos.push_back(cadena);

                                        if(Inodos.i_type == '0'){
                                            CrearBloque(ruta, inicio, bloque_apuntador.b_pointers[j], true, i);
                                        }
                                        else{
                                            CrearBloque(ruta, inicio, bloque_apuntador.b_pointers[j], false, i);
                                        }

                                    }
                                }
                                contenido += "</table>> ]; \n";
                                this->bloques.push_back(contenido);

                            }
                            else{
                                //cout << "---> "<< bloque_apuntador2.b_pointers[k]<<endl;
                                if(bloque_apuntador2.b_pointers[k] == -1){
                                    for(int l = 0; l < 16-k; l++){
                                        contenido2 +="<tr><td port='BA"+to_string(bloque_apuntador2.b_pointers[k+l])+"B"+to_string(k+l)+"'>"+to_string(bloque_apuntador2.b_pointers[k+l])+"</td></tr> \n";
                                    }
                                    break;
                                }
                            }
                        }
                        contenido2 += "</table>> ]; \n";
                        this->bloques.push_back(contenido2);*/

                    }
                    else{
                        if(bloque_apuntador3.b_pointers[m] == -1){
                            for(int l = 0; l < 16-m; l++){
                                contenido3 +="<tr><td port='BA"+to_string(bloque_apuntador3.b_pointers[m+l])+"B"+to_string(m+l)+"'>"+to_string(bloque_apuntador3.b_pointers[m+l])+"</td></tr> \n";
                            }
                            break;
                        }
                    }
                }
                contenido3 += "</table>> ]; \n";
                this->bloques.push_back(contenido3);
            }
        }

    }
    string tipo(1, Inodos.i_type);
    //cout << "||||-> "<<tipo<<endl;
    string num;
    if(tipo == "0"){
        num = "0";
    }
    else{
        num = "1";
    }
    //contenido_dot += "<tr><td>TIPO:</td><td>"+ num +"</td></tr>";
    contenido_dot += "</table>> ]; \n";
    this->inodos.push_back(contenido_dot);
    return;
}

void obrep::CrearBloque(string ruta, int inicio, int posicion, bool tipo, int soycero){

    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    if(tipo){
        structs::BloquesdeCarpetas Bloque_carpeta;
        obtouch::getBlock_Carpets(&Bloque_carpeta, ruta, super.s_block_start + posicion*sizeof(structs::BloquesdeCarpetas));

        string contenido = "B"+to_string(posicion)+"[ shape=plaintext label=<  \n";
        contenido += "<table border='0' cellborder='1' cellspacing='0'> \n";
        contenido += "<tr><td bgcolor=\"lightgreen\">BLOCK</td><td bgcolor=\"lightgreen\">"+to_string(posicion) +"</td></tr> \n";

        if(soycero == 0){
            contenido +="<tr><td> .. </td><td port='I"+to_string(Bloque_carpeta.b_content[0].b_inodo)+"B"+to_string(posicion)+"'>"+to_string(Bloque_carpeta.b_content[0].b_inodo)+"</td></tr> \n";
            contenido +="<tr><td> . </td><td port='I"+to_string(Bloque_carpeta.b_content[1].b_inodo)+"B"+to_string(posicion)+"'>"+to_string(Bloque_carpeta.b_content[1].b_inodo)+"</td></tr> \n";
            for(int i = 2; i < 4; i++){
                if(Bloque_carpeta.b_content[i].b_inodo == -1){
                    for(int j = 0; j < 4-i; j++){
                        contenido +="<tr><td> </td><td port='I"+to_string(Bloque_carpeta.b_content[j+i].b_inodo)+"B"+to_string(posicion)+"'>"+to_string(Bloque_carpeta.b_content[j+i].b_inodo)+"</td></tr> \n";
                    }
                    contenido += "</table>> ]; \n";
                    this->bloques.push_back(contenido);
                    return;
                }
                else{
                   string nombre(Bloque_carpeta.b_content[i].b_name);
                   contenido +="<tr><td>" +nombre+"</td><td port='I"+to_string(Bloque_carpeta.b_content[i].b_inodo)+"B"+to_string(posicion)+"'>"+to_string(Bloque_carpeta.b_content[i].b_inodo)+"</td></tr> \n";
                   string cadena = "B"+to_string(posicion)+":I"+to_string(Bloque_carpeta.b_content[i].b_inodo)+"B"+to_string(posicion)+"->I"+to_string(Bloque_carpeta.b_content[i].b_inodo)+";";
                   this->nodos.push_back(cadena);
                   CrearInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo);
                }
            }
        }
        else{
            for(int i = 0; i < 4; i++){
                if(Bloque_carpeta.b_content[i].b_inodo == -1){
                    for(int j = 0; j < 4-i; j++){
                        contenido +="<tr><td> </td><td port='I"+to_string(Bloque_carpeta.b_content[j+i].b_inodo)+"B"+to_string(posicion)+"'>"+to_string(Bloque_carpeta.b_content[j+i].b_inodo)+"</td></tr> \n";
                    }
                    contenido += "</table>> ]; \n";
                    this->bloques.push_back(contenido);
                    return;
                }
                else{
                    string nombre(Bloque_carpeta.b_content[i].b_name);
                    contenido +="<tr><td>" +nombre+"</td><td port='I"+to_string(Bloque_carpeta.b_content[i].b_inodo)+"B"+to_string(posicion)+"'>"+to_string(Bloque_carpeta.b_content[i].b_inodo)+"</td></tr> \n";
                    string cadena = "B"+to_string(posicion)+":I"+to_string(Bloque_carpeta.b_content[i].b_inodo)+"B"+to_string(posicion)+"->I"+to_string(Bloque_carpeta.b_content[i].b_inodo)+";";
                    this->nodos.push_back(cadena);
                    CrearInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo);
                }
            }
        }
        contenido += "</table>> ]; \n";
        this->bloques.push_back(contenido);
        return;

    }
    else{
        structs::BloquesdeArchivos Bloque_Archivos;
        obtouch::getBlock_Files(&Bloque_Archivos, ruta, super.s_block_start + posicion*sizeof(structs::BloquesdeArchivos));

        string contenido = "B"+to_string(posicion)+"[ shape=plaintext label=<  \n" ;
        contenido +=  "<table border='0' cellborder='1' cellspacing='0'> \n";
        contenido +=  "<tr><td bgcolor=\"yellow\">BLOCK</td><td bgcolor=\"yellow\">"+to_string(posicion) +"</td></tr> \n";

        string nombre(Bloque_Archivos.b_content);
        if(nombre.length() > 64){
            nombre = nombre.substr(0, 64);
        }
        contenido +="<tr><td>" +nombre+"</td></tr> \n";
        contenido += "</table>> ]; \n";
        this->bloques.push_back(contenido);
        return;
    }

    return;
}

void obrep::CrearTablaMBR(string ruta){
    string contenido = "";
    contenido += "digraph g{\n rankdir=TB; \n";
    structs::MBR mbr;
    ExisteMBR(ruta, &mbr);
    string nombre_archivo = NombreArchivo(ruta);
    char tmp1[60];
    strftime(tmp1, 20, "%d/%m/%Y %H:%M", localtime(&mbr.mbr_date));
    string date_m(tmp1);
    string fit1(1, mbr.mbr_fit);

    contenido += "MBR"+to_string(0)+"[ shape=plaintext label=<  \n"+
                 "<table border='0' cellborder='1' cellspacing='0'> \n"+
                 "<tr><td bgcolor="+'"'+"lightblue"+'"'+">MBR</td><td bgcolor="+'"'+"lightblue"+'"'+">"+nombre_archivo+"</td></tr> \n"+
                 "<tr><td bgcolor="+'"'+"lightblue"+'"'+">ATRIBUTO</td><td bgcolor="+'"'+"lightblue"+'"'+">VALOR</td></tr> \n"+
                 "<tr><td>mbr_tamaño</td><td>"+to_string(mbr.mbr_size)+"</td></tr> \n"+
                 "<tr><td>mbr_fecha_creacion</td><td>"+date_m+"</td></tr> \n"+
                 "<tr><td>mbr_disk_signature</td><td>"+to_string(mbr.mbr_disk_signature)+"</td></tr> \n"+
                 "<tr><td>mbr_disk_fit</td><td>"+fit1+"</td></tr> \n";


    for(int i = 0; i < 4; i++){
        if(mbr.mbr_partitions[i].status == '1'){

            structs::partition tmp_partittion = mbr.mbr_partitions[i];
            int n = i+1;
            string status(1, tmp_partittion.status);
            string type(1, tmp_partittion.type);
            string fit(1, tmp_partittion.fit);
            string name(tmp_partittion.name);
            contenido += "<tr><td bgcolor=\"lightblue\">Partittion</td><td bgcolor=\"lightblue\">"+to_string(n)+"</td></tr> \n"+
                    "<tr><td>part_status_"+to_string(n)+"</td><td>"+status+"</td></tr> \n"+
                    "<tr><td>part_type_"+to_string(n)+"</td><td>"+type+"</td></tr> \n"+
                    "<tr><td>part_fit_"+to_string(n)+"</td><td>"+fit+"</td></tr> \n"+
                    "<tr><td>part_start_"+to_string(n)+"</td><td>"+to_string(tmp_partittion.part_start)+"</td></tr> \n"+
                    "<tr><td>part_size_"+to_string(n)+"</td><td>"+to_string(tmp_partittion.size)+"</td></tr> \n"+
                    "<tr><td>part_name_"+to_string(n)+"</td><td>"+name+"</td></tr> \n";
         }
    }
    contenido +="</table>> ]; \n";

    list<structs::EBR> lista_ebr;
    int extendida = ExisteExtendida(mbr);
    cout << "--> "<< extendida<<endl;

    if(extendida != 0){
        lista_ebr = getEBR(ruta, extendida);

    }

    string contenido_dot = "";
    int contar = 1;

    for(structs::EBR e:lista_ebr){
        string NombreE(e.name);
        string FitE(1, e.fit);
        string StatusE(1 ,e.status);

        contenido_dot += "EXT"+to_string(contar)+"[ shape=plaintext label=<  \n"+
                     "<table border='0' cellborder='1' cellspacing='0'> \n"+
                     "<tr><td bgcolor="+'"'+"lightgreen"+'"'+">EBR</td><td bgcolor="+'"'+"lightgreen"+'"'+">"+to_string(contar)+"</td></tr> \n"+
                     "<tr><td bgcolor="+'"'+"lightgreen"+'"'+">ATRIBUTO</td><td bgcolor="+'"'+"lightgreen"+'"'+">VALOR</td></tr> \n"+
                     "<tr><td>part_status_"+to_string(contar)+"</td><td>"+StatusE+"</td></tr> \n"+
                     "<tr><td>part_fit_"+to_string(contar)+"</td><td>"+FitE+"</td></tr> \n"+
                     "<tr><td>part_start_"+to_string(contar)+"</td><td>"+to_string(e.part_start)+"</td></tr> \n"+
                     "<tr><td>part_size_"+to_string(contar)+"</td><td>"+to_string(e.size)+"</td></tr> \n"+
                     "<tr><td>part_next_"+to_string(contar)+"</td><td>"+to_string(e.part_next)+"</td></tr> \n"+
                     "<tr><td>part_name_"+to_string(contar)+"</td><td>"+NombreE+"</td></tr> \n";
        contenido_dot +="</table>> ]; \n";
        contar++;
    }

    contenido += contenido_dot;

    contenido += "}";
    archivo_dot(contenido, "reporte");
    return;
}

void obrep::CrearDisk(string ruta){
    string contenido = "";
    contenido += "digraph g{\n rankdir=TB; \n";
    structs::MBR mbr;
    ExisteMBR(ruta, &mbr);
    int SIZE_MBR = mbr.mbr_size -sizeof(structs::MBR);
    cout<< "--- "<< SIZE_MBR << endl;


    list<structs::EBR> lista_ebr;
    int extendida = ExisteExtendida(mbr);

    if(extendida != 0){
        lista_ebr = getEBR(ruta, extendida);
    }
    list<structs::DRAW_MBR> lista_draw;
    list<structs::DRAW_MBR> lista_draw_L;

    for(int i = 0; i < 4; i++){
        structs::DRAW_MBR draw_mbr;
        if(mbr.mbr_partitions[i].status == '1'){

            if(mbr.mbr_partitions[i].type == 'E'){
                //CALCULAR EL ESPACIO CON LAS LOGICASs
                draw_mbr.porcentaje = (SIZE_MBR*100)/mbr.mbr_partitions[i].size;
                draw_mbr.nombre_tipo = getNombreTipoMBR(mbr.mbr_partitions[i].type);
                lista_draw.push_back(draw_mbr);

                cout<< "EBRS -> "<< lista_ebr.size() << endl;
                if(lista_ebr.size() == 0){
                    float g = (mbr.mbr_partitions[i].size*100);
                    draw_mbr.porcentaje = g/SIZE_MBR;
                    draw_mbr.nombre_tipo = "Libre";
                    lista_draw_L.push_back(draw_mbr);

                }else{
                    int contador = 1;
                    for(structs::EBR e: lista_ebr){
                        structs::DRAW_MBR draw_mbrL;
                        float a =(e.size*100);
                        draw_mbrL.porcentaje = a/SIZE_MBR;
                        draw_mbrL.nombre_tipo = "Logica";
                        lista_draw_L.push_back(draw_mbrL);

                        int espacio;
                        if(contador == lista_ebr.size()){
                            espacio = (mbr.mbr_partitions[i].part_start + mbr.mbr_partitions[i].size) - (e.part_start + e.size);
                        }
                        else{
                            espacio = e.part_next - (e.part_start + e.size);
                        }
                        if(espacio > 0){
                            structs::DRAW_MBR draw_mbrL2;
                            float b = (espacio)*100.0;
                            draw_mbrL2.porcentaje = b/SIZE_MBR;
                            draw_mbrL2.nombre_tipo = "Libre";
                            lista_draw_L.push_back(draw_mbrL2);
                        }
                        contador++;
                    }
                }


            }
            else{
                float a = (mbr.mbr_partitions[i].size*100);
                draw_mbr.porcentaje = a/SIZE_MBR;
                draw_mbr.nombre_tipo = getNombreTipoMBR(mbr.mbr_partitions[i].type);
                lista_draw.push_back(draw_mbr);
                if(i<3){
                    //ESPACIOS ENTRE LAS PRIMERAS 3 PARTICIONES
                    int espacio = mbr.mbr_partitions[i+1].part_start - (mbr.mbr_partitions[i].part_start + mbr.mbr_partitions[i].size);
                    if(espacio < 1){
                        //NO HAY ESPACIO LIBRE
                    }
                    else{
                        //SI ENTRA AQUI ES QUE HAY ESPACIO LIBRE
                        structs::DRAW_MBR draw_mbr1;
                        float b = (espacio*100);
                        draw_mbr1.porcentaje = b/SIZE_MBR;
                        draw_mbr1.nombre_tipo = "Libre";
                        lista_draw.push_back(draw_mbr1);
                    }
                }
                else{
                    //SI LLEGA A LA CUARTA PARTICION CALCULAR SI HAY ESPACIO CON EL FINAL
                    int espacio = SIZE_MBR- (mbr.mbr_partitions[i].part_start + mbr.mbr_partitions[i].size);
                    if(espacio < 1){
                        //NO HAY ESPACIO LIBRE

                    }
                    else{
                        //SI ENTRA AQUI ES QUE HAY ESPACIO LIBRE
                        structs::DRAW_MBR draw_mbr1;
                        float c = (espacio*100);
                        draw_mbr1.porcentaje = c/SIZE_MBR;
                        draw_mbr1.nombre_tipo = "Libre";
                        lista_draw.push_back(draw_mbr1);
                    }
                }
            }
        }
        else{
            //CALCULAR LA ULTIMA PARTICION ACTIVA CON EL FINAL
            int espacio = SIZE_MBR- (mbr.mbr_partitions[i-1].part_start + mbr.mbr_partitions[i-1].size);
            float d =(espacio*100);
            draw_mbr.porcentaje = d/SIZE_MBR;
            draw_mbr.nombre_tipo = "Libre";
            lista_draw.push_back(draw_mbr);
            break;
        }

    }
    string contenido_logicas = "";
    string contenido_logicas1 = "";

    contenido_logicas  +=  "<td><table border='0' cellborder='1' cellspacing='0' align='left'> \n";
    int contador = 1;
    for(structs::DRAW_MBR d:lista_draw_L){
        cout << "-------------------------------"<<endl;
        cout<< "NOMBRE -> " <<d.nombre_tipo << endl;
        cout<< "PORCENTAJE -> " <<to_string(d.porcentaje) << endl;
        if(d.nombre_tipo == "Logica"){
            contenido_logicas1 += "<td>EBR</td><td>"+d.nombre_tipo+"<br/>"+to_string(d.porcentaje) +"%</td> \n";
            contador += 2;
        }
        else{
            contenido_logicas1 += "<td>"+d.nombre_tipo+"<br/>"+to_string(d.porcentaje) +"%</td> \n";
            contador += 1;
        }

    }
    contenido_logicas  +=  "<tr><td colspan=\""+to_string(contador)+"\">Extendida</td></tr> \n";
    contenido_logicas += "<tr>"+contenido_logicas1+"</tr>";
    contenido_logicas  +="</table> \n </td> \n ";

    contenido  +=  "DISK0[ shape=plaintext label=< \n";
    contenido  +=  "<table border='0' cellborder='1' cellspacing='0' > \n <tr>";

    contenido += "<td><table border='0' cellborder='1' cellspacing='0'>\n";
    contenido += "<tr><td>MBR</td></tr>\n";
    contenido += "</table></td> \n";

    for(structs::DRAW_MBR d:lista_draw){
        if(d.nombre_tipo == "Extendida"){
            contenido += contenido_logicas;
        }
        else{
            contenido += "<td><table border='0' cellborder='1' cellspacing='0'>\n";
            contenido += "<tr><td>"+d.nombre_tipo+"<br/>"+to_string(d.porcentaje) +"%</td></tr>\n";
            contenido += "</table></td> \n";
        }
    }

    contenido += "</tr> \n </table>> ]; \n}";

    archivo_dot(contenido, "reporte");

}

string obrep::getNombreTipoMBR(char a){
    if(a == 'P'){
        return "Primaria";
    }
    else if(a == 'E'){
        return "Extendida";
    }
    else if(a == 'L'){
        return "Logica";
    }
}

void obrep::CrearSB(string ruta, int inicio){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);



    string contenido = "";
    contenido += "digraph g{\n rankdir=TB; \n";

    char tmp1[60];
    strftime(tmp1, 20, "%d/%m/%Y %H:%M", localtime(&super.s_mtime));
    string date_m(tmp1);

    char tmp2[60];
    strftime(tmp2, 20, "%d/%m/%Y %H:%M", localtime(&super.s_umtime));
    string date_c(tmp2);

    contenido += "SB"+to_string(0)+"[ shape=plaintext label=<  \n"+
                 "<table border='0' cellborder='1' cellspacing='0'> \n"+
                 "<tr><td bgcolor="+'"'+"lightblue"+'"'+">ATRIBUTO</td><td bgcolor="+'"'+"lightblue"+'"'+">VALOR</td></tr> \n"+
                 "<tr><td>s_filesystem_type</td><td>"+to_string(super.s_filesystem_type)+"</td></tr> \n"+
                 "<tr><td>s_inodes_count</td><td>"+to_string(super.s_inodes_count)+"</td></tr> \n"+
                 "<tr><td>s_blocks_count</td><td>"+to_string(super.s_blocks_count)+"</td></tr> \n"+
                 "<tr><td>s_free_blocks_count</td><td>"+to_string(super.s_free_blocks_count)+"</td></tr> \n"+
                 "<tr><td>s_free_inodes_count</td><td>"+to_string(super.s_free_inodes_count)+"</td></tr> \n"+
                 "<tr><td>s_mtime</td><td>"+date_m+"</td></tr> \n"+
                 "<tr><td>s_umtime</td><td>"+date_c+"</td></tr> \n"+
                 "<tr><td>s_mnt_count</td><td>"+to_string(super.s_mnt_count)+"</td></tr> \n"+
                 "<tr><td>s_magic</td><td>"+to_string(super.s_magic)+"</td></tr> \n"+
                 "<tr><td>s_inode_size</td><td>"+to_string(super.s_inode_size)+"</td></tr> \n"+
                 "<tr><td>s_block_size</td><td>"+to_string(super.s_block_size)+"</td></tr> \n"+
                 "<tr><td>s_firts_ino</td><td>"+to_string(super.s_firts_ino)+"</td></tr> \n"+
                 "<tr><td>s_first_blo</td><td>"+to_string(super.s_first_blo)+"</td></tr> \n"+
                 "<tr><td>s_bm_inode_start</td><td>"+to_string(super.s_bm_inode_start)+"</td></tr> \n"+
                 "<tr><td>s_bm_block_start</td><td>"+to_string(super.s_bm_block_start)+"</td></tr> \n"+
                 "<tr><td>s_inode_start</td><td>"+to_string(super.s_inode_start)+"</td></tr> \n"+
                 "<tr><td>s_block_start</td><td>"+to_string(super.s_block_start)+"</td></tr> \n";

    contenido +="</table>> ]; \n }";
    archivo_dot(contenido, "reporte");
    return;
}

void obrep::CrearINODO(string ruta, int inicio){
    this->inodos.clear();
    string contenido = "digraph g{\n rankdir=LR; \n";
    contenido += "B001[ shape=plaintext label=<  \n";
    contenido += "<table border='0' cellborder='1' cellspacing='0'> \n";
    contenido += "<tr><td bgcolor=\"lightblue\">INDEX</td><td bgcolor=\"lightblue\">TIPO</td><td bgcolor=\"lightblue\">NOMBRE</td></tr> \n";

    CrearINODOInodo(ruta, inicio, 0,  "/");

    for(string e: this->inodos){
        contenido += e;
    }

    contenido += "</table>> ]; \n }";

    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    char numero;
    FILE *bfile = fopen(ruta.c_str(), "rb+");
    fseek(bfile, super.s_bm_inode_start, SEEK_SET);
    fread(&numero, 1, 1, bfile);
    fclose(bfile);

    char eli;
    if(numero != '1'){
        contenido = "digraph g{\n rankdir=LR; \n}";
    }

    archivo_dot(contenido, "reporte");
    return;


}

void obrep::CrearINODOInodo(string ruta, int inicio, int posicion, string nombre){

        structs::SuperBloque super;
        obtouch::getSuperBloque(&super, ruta, inicio);

        structs::TabladeInodos Inodos;
        obtouch::getInodo(&Inodos, ruta, super.s_inode_start + posicion*sizeof(structs::TabladeInodos));
        string tipo;
        string contenido = "";
        for(int i = 0; i < 15; i++){
            if(Inodos.i_block[i] == -1){
                contenido += "<tr><td>"+to_string(posicion)+"</td><td>"+tipo+"</td><td>"+nombre+"</td></tr>";
                this->inodos.push_back(contenido);
                return;
            }
            else{
                if(Inodos.i_type == '0'){
                    tipo = "Folder";
                    CrearINODOBloque(ruta, inicio, Inodos.i_block[i], true, i, nombre);
                }
                else{
                    tipo = "Archivo";
                     CrearINODOBloque(ruta, inicio, Inodos.i_block[i], false, i, nombre);
                }
            }
        }

        contenido += "<tr><td>"+to_string(posicion)+"</td><td>"+tipo+"</td><td>"+nombre+"</td></tr>";
        this->inodos.push_back(contenido);
        return;
}

void obrep::CrearINODOBloque(string ruta, int inicio, int posicion, bool tipo, int soycero, string name){
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
                    CrearINODOInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo, name+"/"+nombre);
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
                    CrearINODOInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo, name+"/"+nombre);
                }
            }
        }
        return;

    }
    else{
        return;
    }

    return;
}

void obrep::CrearBLOCK(string ruta, int inicio){
    this->bloques.clear();
    string contenido = "digraph g{\n rankdir=LR; \n";

    CrearBLOCKInodo(ruta, inicio, 0);

    for(string e: this->bloques){
        contenido += e;
        //cout << ""<< e << endl;
    }
    contenido += "\n }";

    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    char numero;
    FILE *bfile = fopen(ruta.c_str(), "rb+");
    fseek(bfile, super.s_bm_inode_start, SEEK_SET);
    fread(&numero, 1, 1, bfile);
    fclose(bfile);

    char eli;
    if(numero != '1'){
        contenido = "digraph g{\n rankdir=LR; \n}";
    }
    archivo_dot(contenido, "reporte");


}

void obrep::CrearBLOCKInodo(string ruta, int inicio, int posicion){

        structs::SuperBloque super;
        obtouch::getSuperBloque(&super, ruta, inicio);

        structs::TabladeInodos Inodos;
        obtouch::getInodo(&Inodos, ruta, super.s_inode_start + posicion*sizeof(structs::TabladeInodos));

        for(int i = 0; i < 15; i++){
            if(Inodos.i_block[i] == -1){

                return;
            }
            else{
                if(Inodos.i_type == '0'){
                    CrearBLOCKBloque(ruta, inicio, Inodos.i_block[i], true, i);
                }
                else{
                    CrearBLOCKBloque(ruta, inicio, Inodos.i_block[i], false, i);
                }
            }
        }

        return;
}

void obrep::CrearBLOCKBloque(string ruta, int inicio, int posicion, bool tipo, int soycero){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);


    if(tipo){
        structs::BloquesdeCarpetas Bloque_carpeta;
        obtouch::getBlock_Carpets(&Bloque_carpeta, ruta, super.s_block_start + posicion*sizeof(structs::BloquesdeCarpetas));

        string contenido = "Bloque de carpeta "+to_string(posicion)+"\\l";
        contenido += "b_name   b_inodo \\l";

        if(soycero == 0){
            contenido += " ..  - "+to_string(Bloque_carpeta.b_content[0].b_inodo) +"\\l";
            contenido += " .   - "+to_string(Bloque_carpeta.b_content[1].b_inodo) +"\\l";
            for(int i = 2; i < 4; i++){
                if(Bloque_carpeta.b_content[i].b_inodo == -1){
                    for(int j = 0; j < 4-i; j++){
                        contenido += "   -  -1 \\l";
                    }
                    string con = "BLOCK"+to_string(posicion)+"[ shape=record label=\" "+contenido+"\"];\n";
                    this->bloques.push_back(con);
                    return;
                }
                else{
                    string name (Bloque_carpeta.b_content[i].b_name);
                    contenido += name+" - "+to_string(Bloque_carpeta.b_content[i].b_inodo)+"\\l";
                    CrearBLOCKInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo);
                }
            }
        }
        else{
            for(int i = 0; i < 4; i++){
                if(Bloque_carpeta.b_content[i].b_inodo == -1){
                    for(int j = 0; j < 4-i; j++){
                        contenido += "   -  -1\\l";
                    }
                    string con = "BLOCK"+to_string(posicion)+"[ shape=record label=\" "+contenido+"\"];\n";
                    this->bloques.push_back(con);
                    return;
                }
                else{
                    string name (Bloque_carpeta.b_content[i].b_name);
                    contenido += name+" - "+to_string(Bloque_carpeta.b_content[i].b_inodo)+"\\l";
                    CrearBLOCKInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo);
                }
            }
        }
        string con = "BLOCK"+to_string(posicion)+"[ shape=record label=\" "+contenido+"\"];\n";
        this->bloques.push_back(con);
        return;

    }
    else{
        structs::BloquesdeArchivos Bloque_archivos;
        obtouch::getBlock_Files(&Bloque_archivos, ruta, super.s_block_start + posicion*sizeof(structs::BloquesdeArchivos));

        string contenido = "Bloque de archivo "+to_string(posicion)+" \n";

        string name(Bloque_archivos.b_content);
        string nombre = name;
        string aux = "";
        if(nombre.length() > 0){
            nombre = nombre.substr(0, 64);
        }
        aux = nombre;
        //cout << ""<< aux << endl;
        string con = "BLOCK"+to_string(posicion)+"[ shape=record label=\" "+contenido+"\\n"+aux+" \"]; \n";
        this->bloques.push_back(con);
        return;
    }

    return;
}

void obrep::CrearBIPMAP(string ruta, int inicio, bool tipo){
    int inicio_bm = 0;
    int final = 0;
    string contenido = "BITMAP ";
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    if(tipo){
        final = super.s_inodes_count;
        inicio_bm = super.s_bm_inode_start;
        contenido += "INODES\\n \\n";
    }
    else{
        final = super.s_blocks_count;
        inicio_bm = super.s_bm_block_start;
        contenido += "BLOCKS\\n \\n";
    }
    string cadena;
    char tmp;
    for(int i = 0; i < final; i++){
        FILE *file = fopen(ruta.c_str(), "rb+");
        fseek(file, inicio_bm+i , SEEK_SET);
        fread(&tmp, 1, 1, file);
        fclose(file);

        cadena += tmp;

    }
    int contador = 0;
    for(int i = 0; i < cadena.length(); i++){
        if(contador == 20){
            contador=0;
            contenido += "\\n";
        }
        contenido += cadena[i];
        contador++;
    }




    string cdot = "digraph g{ BITMAP[ shape=record label=\""+contenido+"\"]; }";
    archivo_dot(cdot, "reporte");

    return;

}

void obrep::CrearFILE(string ruta, int inicio, string ruta_carpetas){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    list<string> carpetas = separar_carpetas(ruta_carpetas);
    list<string>::iterator it;
    string NombreAr = NombreArchivo(ruta_carpetas);
    carpetas.push_back(NombreAr);

    int contador_inodo = 0;
    int contador_block= 0;
    int contador_block_actual= 0;
    int contar = 0;
    if(this->ruta_file != "/"){
        for (it = carpetas.begin(); it != carpetas.end(); it++){
            contar++;
            obtouch::getSuperBloque(&super, ruta, inicio);
            structs::TabladeInodos Inodo_actual; //RECUPERAMOS EL INO
            obtouch::getInodo(&Inodo_actual, ruta, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));
            if(Inodo_actual.i_type != 'I'){
                bool Encontrelacarpeta = false;

                //BUSCAMOS LAS POSICIONES QUE ESTEN DISPONIBLES
                for(int j = 0; j < 15; j++){
                    if(Inodo_actual.i_block[j] != -1){
                        contador_block_actual = j;
                        //cout << "CONTADOR DE BLOQUE ACTUAL " <<contador_block_actual << endl;
                        //cout << "APUNTADOR DE BLOQUE -->"<< Inodo_actual.i_block[j] <<endl;
                        contador_block =Inodo_actual.i_block[j];
                        //BUSCAMOS BLOQUES DE CARPETAS
                        structs::BloquesdeCarpetas BloqueCarpeta_Actual;
                        obtouch::getBlock_Carpets(&BloqueCarpeta_Actual, ruta, super.s_block_start + contador_block*sizeof(structs::BloquesdeCarpetas));

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
                    if(Encontrelacarpeta){
                        break;
                    }
                }

                if(!Encontrelacarpeta){
                    cout << "\n LA CARPETA BUSCADA NO EXISTE \n"<<endl;
                    return;
                }
            }
        }

    }
    else{
        NombreAr="/";
        contador_inodo = 0;
    }

    structs::TabladeInodos Inodo_actual; //RECUPERAMOS EL INO
    obtouch::getInodo(&Inodo_actual, ruta, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));
    string contenido_tmp;
    for(int i = 0; i < 15; i++){
        if(Inodo_actual.i_block[i] == -1){
            break;
        }
        else{
            structs::BloquesdeArchivos BloqueArchivo_Actual;
            obtouch::getBlock_Files(&BloqueArchivo_Actual, ruta, super.s_block_start + Inodo_actual.i_block[i]*sizeof(structs::BloquesdeArchivos));

            string contenido(BloqueArchivo_Actual.b_content);
            contenido_tmp += contenido + "\\n";

        }
    }

    string dot = "digraph g{ \n FILE[ shape=record label= \" "+NombreAr+" \\n \\n "+contenido_tmp+"\"]; }";


    char numero;
    FILE *bfile = fopen(ruta.c_str(), "rb+");
    fseek(bfile, super.s_bm_inode_start, SEEK_SET);
    fread(&numero, 1, 1, bfile);
    fclose(bfile);

    char eli;
    if(numero != '1'){
        dot = "digraph g{\n rankdir=LR; \n}";
    }

    archivo_dot(dot, "reporte");
    return;

}

void obrep::CrearJornaling(string ruta, int inicio){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    list<structs::Jornaling> jg;
    obtouch *touch = new obtouch();
    touch->getJornaling(&jg, super, ruta, inicio);
    string contenido = "digraph g{\n rankdir=LR; \n";
    contenido += "JG"+to_string(0)+"[ shape=plaintext label=<  \n"+
                 "<table border='0' cellborder='1' cellspacing='0'> \n"+
                 "<tr><td bgcolor=\"lightblue\">Tipo_Operacion</td><td bgcolor=\"lightblue\">Tipo</td><td bgcolor=\"lightblue\">Path</td><td bgcolor=\"lightblue\">Contenido</td><td bgcolor=\"lightblue\">Fecha Creacion</td><td bgcolor=\"lightblue\">Size</td></tr> \n";
    //cout << "SIZEOF J "<<sizeof(jg)<<endl;
    for(structs::Jornaling e: jg){

        if(e.tipo == 'I'){
            break;
        }

        string tipo_op(e.tipo_operacion);
        string tipo(1, e.tipo);
        string path(e.path);
        string entrada(e.contenido);

        char tmp2[60];
        strftime(tmp2, 20, "%d/%m/%Y %H:%M", localtime(&e.log_fecha));
        string date_c(tmp2);

        contenido += "<tr><td>"+tipo_op+"</td><td>"+tipo+"</td><td>"+path+"</td><td>"+entrada+"</td><td>"+date_c+"</td><td>"+to_string(e.size)+"</td></tr> \n";
    }

    contenido +="</table>> ]; \n }";
    archivo_dot(contenido, "reporte");
    return;
}

void obrep::CrearLS(string ruta, int inicio){

    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);

    list<string> carpetas = separar_carpetas(this->ruta_file);
    list<string>::iterator it;
    string NombreAr = NombreArchivo(this->ruta_file);
    carpetas.push_back(NombreAr);

    int contador_inodo = 0;
    int contador_block= 0;
    int contador_block_actual= 0;
    int contar = 0;
    if(this->ruta_file != "/"){
        for (it = carpetas.begin(); it != carpetas.end(); it++){
            contar++;
            obtouch::getSuperBloque(&super, ruta, inicio);
            structs::TabladeInodos Inodo_actual; //RECUPERAMOS EL INO
            obtouch::getInodo(&Inodo_actual, ruta, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));
            if(Inodo_actual.i_type != 'I'){
                bool Encontrelacarpeta = false;

                //BUSCAMOS LAS POSICIONES QUE ESTEN DISPONIBLES
                for(int j = 0; j < 15; j++){
                    if(Inodo_actual.i_block[j] != -1){
                        contador_block_actual = j;
                        //cout << "CONTADOR DE BLOQUE ACTUAL " <<contador_block_actual << endl;
                        //cout << "APUNTADOR DE BLOQUE -->"<< Inodo_actual.i_block[j] <<endl;
                        contador_block =Inodo_actual.i_block[j];
                        //BUSCAMOS BLOQUES DE CARPETAS
                        structs::BloquesdeCarpetas BloqueCarpeta_Actual;
                        obtouch::getBlock_Carpets(&BloqueCarpeta_Actual, ruta, super.s_block_start + contador_block*sizeof(structs::BloquesdeCarpetas));

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
                    if(Encontrelacarpeta){
                        break;
                    }
                }

                if(!Encontrelacarpeta){
                    cout << "\n LA CARPETA BUSCADA NO EXISTE \n"<<endl;
                    return;
                }
            }
        }

    }
    else{
        NombreAr="/";
        contador_inodo = 0;
    }



    this->bloques.clear();
    string contenido = "digraph g{\n rankdir=LR; \n";

    CrearLSInodo(ruta, inicio, contador_inodo, NombreAr);
    contenido += "JG"+to_string(0)+"[ shape=plaintext label=<  \n"+
                 "<table border='0' cellborder='1' cellspacing='0'> \n"+
                 "<tr><td bgcolor=\"lightblue\">Permisos</td><td bgcolor=\"lightblue\">Owner</td><td bgcolor=\"lightblue\">Grupo</td><td bgcolor=\"lightblue\">Size (en Bytes)</td><td bgcolor=\"lightblue\">Fecha Creacion</td><td bgcolor=\"lightblue\">Tipo</td><td bgcolor=\"lightblue\">Nombre</td></tr> \n";

    for(string e: this->bloques){
        contenido += e;
        //cout << ""<< e << endl;
    }
    contenido +="</table>> ]; \n }";


    char numero;
    FILE *bfile = fopen(ruta.c_str(), "rb+");
    fseek(bfile, super.s_bm_inode_start, SEEK_SET);
    fread(&numero, 1, 1, bfile);
    fclose(bfile);

    char eli;
    if(numero != '1'){
        contenido = "digraph g{\n rankdir=LR; \n}";
    }
    archivo_dot(contenido, "reporte");
    return;


}

void obrep::CrearLSInodo(string ruta, int inicio, int posicion, string nombre){

        structs::SuperBloque super;
        obtouch::getSuperBloque(&super, ruta, inicio);

        structs::TabladeInodos Inodos;
        obtouch::getInodo(&Inodos, ruta, super.s_inode_start + posicion*sizeof(structs::TabladeInodos));
        string tipo = "";
        string contenido = "";
        string grupo_inodo = getGrupo(to_string(Inodos.i_gid), ruta, inicio);
        string user_inodo = getUsuario(to_string(Inodos.i_uid), ruta, inicio);

        char tmp1[60];
        strftime(tmp1, 20, "%d/%m/%Y %H:%M", localtime(&Inodos.i_ctime));
        string date_m(tmp1);

        for(int i = 0; i < 15; i++){
            if(Inodos.i_block[i] == -1){
                contenido += "<tr><td>-rw-rw-r-</td><td>"+user_inodo+"</td><td>"+grupo_inodo+"</td><td>"+to_string(Inodos.i_size)+"</td><td>"+tmp1+"</td><td>"+tipo+"</td><td>"+nombre+"</td></tr> \n";
                this->bloques.push_back(contenido);
                return;
            }
            else{
                if(Inodos.i_type == '0'){
                    tipo = "Carpeta";
                    CrearLSBloque(ruta, inicio, Inodos.i_block[i], true, i);
                }
                else{
                    tipo = "Archivo";
                    CrearLSBloque(ruta, inicio, Inodos.i_block[i], false, i);
                }
            }
        }
        contenido += "<tr><td>-rw-rw-r-</td><td>"+user_inodo+"</td><td>"+grupo_inodo+"</td><td>"+to_string(Inodos.i_size)+"</td><td>"+tmp1+"</td><td>"+tipo+"</td><td>"+nombre+"</td></tr> \n";
        this->bloques.push_back(contenido);

        return;
}

void obrep::CrearLSBloque(string ruta, int inicio, int posicion, bool tipo, int soycero){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, ruta, inicio);


    if(tipo){
        structs::BloquesdeCarpetas Bloque_carpeta;
        obtouch::getBlock_Carpets(&Bloque_carpeta, ruta, super.s_block_start + posicion*sizeof(structs::BloquesdeCarpetas));



        if(soycero == 0){
            for(int i = 2; i < 4; i++){
                if(Bloque_carpeta.b_content[i].b_inodo == -1){
                    return;
                }
                else{
                    string n(Bloque_carpeta.b_content[i].b_name);
                    CrearLSInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo, n);
                }
            }
        }
        else{
            for(int i = 0; i < 4; i++){
                if(Bloque_carpeta.b_content[i].b_inodo == -1){

                    return;
                }
                else{
                    string n(Bloque_carpeta.b_content[i].b_name);
                    CrearLSInodo(ruta, inicio, Bloque_carpeta.b_content[i].b_inodo, n);
                }
            }
        }
        return;

    }
    else{
        return;
    }

    return;
}

string obrep::getUsuario(string usuario, string RUTA_PARTITTION1, int PART_START1){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, RUTA_PARTITTION1, PART_START1);

    structs::TabladeInodos Inodo;
    obtouch::getInodo(&Inodo, RUTA_PARTITTION1, super.s_inode_start+ sizeof(structs::TabladeInodos));

    string contenido = "";
    //OBTENEMOS TODO EL CONTENIDO EN LOS BLOQUES
    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] == -1){
            break;
        }
        structs::BloquesdeArchivos Bloque_Archivo;
        obtouch::getBlock_Files(&Bloque_Archivo, RUTA_PARTITTION1, super.s_block_start + Inodo.i_block[i]*sizeof(structs::BloquesdeArchivos));
        string n =Bloque_Archivo.b_content;
        if(n.length() > 64){
            n = n.substr(0,64);
        }
        contenido += n;
    }

    list<string> lista_grupos;
    list<structs::LOGIN> lista_USER;

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
        if(!EsGrupo){
            structs::LOGIN user;
            user.posicion = aux[0];
            user.usuario = aux[3];
            lista_USER.push_back(user);
        }
        else{
            //LLENAR LOS GRUPOS
        }
        s.erase(0, pos + delimiter.length());
    }

    for(structs::LOGIN e: lista_USER){

        if(usuario == e.posicion){
            return e.usuario;
        }
    }

    //cout << "EL USUARIO NO EXISTE!!"<<endl;
    return "root";

}
string obrep::getGrupo(string grupo, string RUTA_PARTITTION1, int PART_START1){
    structs::SuperBloque super;
    obtouch::getSuperBloque(&super, RUTA_PARTITTION1, PART_START1);

    structs::TabladeInodos Inodo;
    obtouch::getInodo(&Inodo, RUTA_PARTITTION1, super.s_inode_start+ sizeof(structs::TabladeInodos));

    string contenido = "";
    //OBTENEMOS TODO EL CONTENIDO EN LOS BLOQUES
    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] == -1){
            break;
        }
        structs::BloquesdeArchivos Bloque_Archivo;
        obtouch::getBlock_Files(&Bloque_Archivo, RUTA_PARTITTION1, super.s_block_start + Inodo.i_block[i]*sizeof(structs::BloquesdeArchivos));
        string n =Bloque_Archivo.b_content;
        if(n.length() > 64){
            n = n.substr(0,64);
        }
        contenido += n;
    }

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

    for(structs::GROUP e: lista_GROUP){
        //cout<< grupo << " == "<< e.posicion << endl;
        if(grupo == e.posicion){
            return e.nombre;
        }
    }

    //cout << "EL GRUPO DEL USUARIO NO EXISTE!!"<<endl;
    return "root";
}
//METODOS OTROS---------------
bool obrep::existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int *size_particion){
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
                        //cout << "LLEGUE AQUI"<<endl;
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

bool obrep::ExisteMBR(string ruta, structs::MBR *mbr){
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

bool obrep::ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part){
    *Ext = 0;
    //cout<<"primera busqueda"<<endl;
    for(int i = 0; i<4; i++){
        if(tmp_MBR.mbr_partitions[i].type == 'E'){
            *Ext=tmp_MBR.mbr_partitions[i].part_start;
            //cout<<"encontre extendida"<<endl;
        }
        if(tmp_MBR.mbr_partitions[i].name == name){
            //cout<<"encontre particion ---"<<endl;
            *start_part = tmp_MBR.mbr_partitions[i].part_start;
            *size_part = tmp_MBR.mbr_partitions[i].size;
            return false;
        }
    }
    return true;
}

int obrep::ExisteExtendida(structs::MBR tmp_MBR){
    for(int i = 0; i<4; i++){
        if(tmp_MBR.mbr_partitions[i].type == 'E'){
            return tmp_MBR.mbr_partitions[i].part_start;
        }
    }
    return 0;
}

bool obrep::ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part){
    //cout<<"busque logica "<< part_start_ext <<endl;
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
    //cout << "pase aqui" <<endl;
    //BUSCAMOS SI EXISTE EL NOMBRE
    for(int i = 0; i < 50; i++){
        //cout << tmp_EBRS[i].status << "<<<< "<<endl;
        if((tmp_EBRS[i].status == '1') || (tmp_EBRS[i].status == '0')){
            //cout << tmp_EBRS[i].name << " == "<< name <<endl;
            if(tmp_EBRS[i].name == name){
                //cout << tmp_EBRS[i].part_start << " -> "<<tmp_EBRS[i].size<<endl;
                *start_part = tmp_EBRS[i].part_start;
                *size_part = tmp_EBRS[i].size;
                return true;
            }
        }
    }
    return false;
}

void obrep::archivo_dot(string content, string name) {
    string path_carpeta = this->path.substr(0, this->path.length() - 4) + ".dot";
    FILE *searchFile = fopen(path_carpeta.c_str(), "wb+");

    //cout << path_carpeta << endl;
    cin;

    if (searchFile == NULL) {
        string create_command = "mkdir -p \"" + this->path + "\"";
        system(create_command.c_str());
        create_command = "rmdir \'" + this->path + "\'";
        system(create_command.c_str());
    }

    ofstream graphFile;
    graphFile.open(path_carpeta);
    graphFile << content;
    graphFile.close();

    string filePath;
    string extension = (this->path.substr(this->path.length() - 3, this->path.length()));
    string salida = this->path.substr(0, this->path.length() - 3) + extension;
    if (extension == "png") {
        filePath = "dot -Tpng \"" + path_carpeta + "\" -o \"" + salida + "\"";
    } else if (extension == "jpg") {
        filePath = "dot -Tjpg \"" + path_carpeta + "\" -o \"" + salida + "\"";
    } else if (extension == "pdf") {
        filePath = "dot -Tpdf \"" + path_carpeta + "\" -o \"" + salida + "\"";
    }
    system(filePath.c_str());
    cout << "Reporte Generado con éxito." << endl;
}

list<string> obrep::CrearBloque(structs::SuperBloque sp, int inodo, int bloque, char tipo, string ruta,  string *contenido1){
    list<string> lista2;
    string contenido = "";

    //CREANDO BLOQUES DE CARPETAS
    if(tipo == '0'){
        if(bloque != -1){
            structs::BloquesdeCarpetas BC;
            obtouch::getBlock_Carpets(&BC, ruta, sp.s_block_start + bloque*sizeof(structs::BloquesdeCarpetas));

            contenido += "B"+to_string(bloque)+"[ shape=plaintext label=<  \n" +
                                "<table border='0' cellborder='1' cellspacing='0'> \n"+
                                "<tr><td bgcolor="+'"'+"lightgreen"+'"'+">BLOCK</td><td bgcolor="+'"'+"lightgreen"+'"'+">"+to_string(bloque) +"</td></tr> \n";

            for(int i = 0; i < 4; i++){

                string nombre(BC.b_content[i].b_name);
                //cout << BC.b_content[i].b_name << " <- "<< BC.b_content[i].b_inodo <<endl;
                if(BC.b_content[i].b_inodo == -1){
                    //cout << "entre en el -1 "<<endl;
                    contenido +="<tr><td> </td><td port='I"+to_string(inodo)+"B"+to_string(i)+"'>"+to_string(BC.b_content[i].b_inodo)+"</td></tr> \n";
                }
                else {
                    //cout << ".. =="<< nombre <<endl;
                    if( nombre == "." || nombre == ".."){
                        //B0:IS2 -> I1;

                    }
                    else{
                        //cout<<"entre aqui"<<endl;
                        string cadena = "B"+to_string(bloque)+":I"+to_string(inodo)+"B"+to_string(i)+"->I"+to_string(BC.b_content[i].b_inodo)+";";
                        lista2.push_back(cadena);
                    }
                    contenido +="<tr><td>"+nombre+"</td><td port='I"+to_string(inodo)+"B"+to_string(i)+"'>"+to_string(BC.b_content[i].b_inodo)+"</td></tr> \n";
                }
            }

            contenido += "</table>> ]; \n";
        }

    }
    else if(tipo == '1'){
        structs::TabladeInodos Ino;
        obtouch::getInodo(&Ino, ruta, sp.s_inode_start + inodo*sizeof(structs::TabladeInodos));

        structs::BloquesdeArchivos BA;
        obtouch::getBlock_Files(&BA, ruta, sp.s_block_start + bloque*sizeof(structs::BloquesdeArchivos));



        for(int i = 0; i < 12; i++){
            if(Ino.i_block[i] == -1){
                //FALTA ARREGLAR ESTO
            }
            else{
                contenido += "B"+to_string(bloque)+"[ shape=plaintext label=<  \n" +
                                    "<table border='0' cellborder='1' cellspacing='0'> \n"+
                                    "<tr><td bgcolor="+'"'+"yellow"+'"'+">BLOCK</td><td bgcolor="+'"'+"yellow"+'"'+">"+to_string(bloque) +"</td></tr> \n";
                string n(BA.b_content);
                contenido += "<tr><td>"+n+"</td></tr> \n";
                contenido += "</table>> ]; \n";
            }
        }

    }

    *contenido1 = contenido;
    return lista2;
}

string obrep::CharToString(char tmp[]){
    string cadena = "";
    char a;
    for(int i = 0; i < sizeof(tmp); i++){
        if(tmp[i] == a){
            cadena += " ";
            break;
        }
        else{
            cadena += tmp[i];
        }

    }
    return cadena;
}

string obrep::NombreArchivo(string path){
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

string obrep::ToUpperString(string cadena){
    string tmp = "";
    for(int i = 0; i < cadena.size(); i++){
        tmp += toupper(cadena[i]);
    }
    return tmp;
}

list<structs::EBR> obrep::getEBR(string ruta, int extendida){
    bool ExisteSiguiente = true;
    int contador = 0;
    int siguiente = extendida;
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
    list<structs::EBR> lista_ebr;
    for(int j = 0; j < 50; j++){
        //cout << tmp_EBRS[j].status << " == "<< tmp_EBRS[j].name <<endl;
        if(tmp_EBRS[j].status == '1'){
            lista_ebr.push_back(tmp_EBRS[j]);
        }
    }
    return lista_ebr;
}

list<string> obrep::separar_carpetas(string path) {
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

string obrep::EliminarComillas(string cadena){
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
