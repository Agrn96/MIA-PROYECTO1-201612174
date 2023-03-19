#include "obmkdir.h"
#include "iostream"
#include "list"
#include "fstream"
#include "obedit.h"
#include "obtouch.h"
using namespace std;

static int PART_START;
static string RUTA_PARTITTION;

extern structs::DiscosMontados Discos_Montados[99];
extern structs::LOGIN USUARIO_ONLINE;

obmkdir::obmkdir()
{

}

void obmkdir::mostrardatos(obmkdir *disco){
     cout << "\n     ----COMANDO MKDIR----\n"<<endl;
    string Ruta = EliminarComillas(disco->path);
    bool R = disco->r;

    if(disco->p_entrada == "r" || disco->p_entrada == "R"){

    }
    else{
        cout << "ERROR DE SINTAXIS"<<endl;
        return;
    }

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

    cout << "bm inode start: "<< super.s_bm_inode_start<<endl;
    cout << "bm block start: "<< super.s_bm_block_start<<endl;
    cout << "inode start: "<< super.s_inode_start<<endl;
    cout << "block start: "<<super.s_block_start<<endl;
    cout << "parte final inodos: "<< super.s_inode_start + super.s_inodes_count*sizeof(structs::TabladeInodos) << " count inodos: "<< super.s_inodes_count <<endl;


    list<string> carpetas = separar_carpetas(Ruta);
    list<string>::iterator it;
    string NombreAr = NombreArchivo(Ruta);
    carpetas.push_back(NombreAr);

    int contador_inodo = 0;
    int contador_block= 0;
    int contador_block_actual= 0;
    int contar = 0;
    for (it = carpetas.begin(); it != carpetas.end(); it++){
        contar++;
        getSuperBloque(&super, RUTA_PARTITTION, PART_START);
        //cout << "CARPETA: "<< *it << endl;
        //cout << "-CONTADOR- : "<< contador_inodo << endl;
        structs::TabladeInodos Inodo_actual; //RECUPERAMOS EL INO
        //cout << "Voy a buscar inodo en: "<< super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos)<<endl;
        getInodo(&Inodo_actual, RUTA_PARTITTION, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));
         //cout << "-BLOQUE APUNTA 0 - : "<< Inodo_actual.i_block[0] << endl;
        if(Inodo_actual.i_type != 'I'){
            bool Encontrelacarpeta = false;

            //BUSCAMOS LAS POSICIONES QUE ESTEN DISPONIBLES
            for(int j = 0; j < 15; j++){
                if(Inodo_actual.i_block[j] != -1){
                    contador_block_actual = j;
                    //cout << "CONTADOR DE BLOQUE ACTUAL " <<contador_block_actual << endl;
                    //cout << "APUNTADOR DE BLOQUE -->"<< Inodo_actual.i_block[j] <<endl;
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
                                    cout << "\n     LA CARPETA YA EXISTE !!\n "<< endl;

                                    return;
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
                                            cout << "\n     LA CARPETA YA EXISTE !!\n "<< endl;

                                            return;
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
                                                    cout << "\n     LA CARPETA YA EXISTE !!\n "<< endl;

                                                    return;
                                                }
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
            //SI NO EXISTE LA CARPETA DENTRO DE LOS BLOQUES
            string nombre_aux = *it;
            if(!Encontrelacarpeta || nombre_aux == NombreAr){
                if(disco->r || nombre_aux == NombreAr){
                    string nombre = *it;
                    int recuperar;
                    BuscarEspacio(super, contador_inodo, contador_block, Inodo_actual, nombre, &recuperar);
                    contador_inodo = recuperar;
                }
                else{
                    cout << "LA CARPETA NO EXISTE!"<< endl;
                    return;
                }
                ////cout << "CONTADOR DEL INODO SIGUIENTE -> "<< contador_inodo << endl;
                //break;
            }
        }
    }

    if(super.s_filesystem_type == 3){
        obtouch *touch = new obtouch();
        string tipo_o = "mkdir";
        touch->CrearJornaling(tipo_o, '0', Ruta, " ", 0, RUTA_PARTITTION, PART_START);
    }

    cout << "PROCESO EXITOSAMENTE"<<endl;
    return;



}


string obmkdir::EliminarComillas(string cadena){
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

void obmkdir::getSuperBloque(structs::SuperBloque *sp, string ruta, int part_Start){
    structs::SuperBloque aux;
    FILE *archivo = fopen(ruta.c_str(), "rb+");
    fseek(archivo, part_Start , SEEK_SET);
    fread(&aux, sizeof(structs::SuperBloque ), 1, archivo);
    fclose(archivo);
    *sp = aux;
}

bool obmkdir::getInodo(structs::TabladeInodos*sp, string ruta, int part_Start){
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
bool obmkdir::getBlock_Files(structs::BloquesdeArchivos *sp, string ruta, int part_Start){
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
bool obmkdir::getBlock_Carpets(structs::BloquesdeCarpetas *sp, string ruta, int part_Start){
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
bool obmkdir::getBlock_Apuntador(structs::BloquesdeApuntadores *sp, string ruta, int part_Start){
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

void obmkdir::BuscarEspacio(structs::SuperBloque sp, int inodo, int block, structs::TabladeInodos ino, string name, int *recuperar_inodo){


           bool ExisteEspacioEnElBloque = false;
           //cout << "*INODO ENTRANTE: "<< inodo <<"  *BLOQUE ENTRANTE: "<< block<<endl;
           structs::BloquesdeCarpetas BloqueCarpeta_Actual;
           getBlock_Carpets(&BloqueCarpeta_Actual, RUTA_PARTITTION, sp.s_block_start + block*sizeof(structs::BloquesdeCarpetas));

           //NAVEGANDO ENTRE LOS ESPACIOS SI EXISTE UNO DISPONIBLE
           for(int j = 0; j < 4; j++){
               if(BloqueCarpeta_Actual.b_content[j].b_inodo == -1){
                   ExisteEspacioEnElBloque = true;
                   //cout << "Encontre espacio para crearme en el bloque de archivo!!!"<<endl;
                   //cout << "Nombre -> "<< name << endl;
                   strcpy(BloqueCarpeta_Actual.b_content[j].b_name , name.c_str());
                   BloqueCarpeta_Actual.b_content[j].b_inodo = sp.s_firts_ino;
                   int recuperar;
                   CreandoEnBloqueArchivos(sp, inodo, block, BloqueCarpeta_Actual, &recuperar);
                   *recuperar_inodo = recuperar;
                   return;
               }
           }

           //cout << "VOLVER AL INODO ANTERIOR Y BUSCAR ESPACIO Y CREARLO"<< endl;
           //VOLVER AL INODO ANTERIOR Y BUSCAR ESPACIO Y CREARLO
           for(int i = 0; i < 12; i++){
               //cout <<"iteracion -> "<< to_string(i)<<" - " <<ino.i_block[i] <<" == "<< -1<<endl;
               if(ino.i_block[i] == -1){
                   //cout << "ENCONTRE ESPACIO EN EL INODO "<< inodo << endl;
                   ino.i_block[i] = sp.s_first_blo ;

                   structs::BloquesdeCarpetas Bloque_carpeta;
                   Bloque_carpeta.b_content[0].b_inodo = sp.s_firts_ino ; //ACTUAL
                   strcpy(Bloque_carpeta.b_content[0].b_name ,name.c_str());

                   //SUPERBLOQUE
                   sp.s_free_blocks_count -=1;
                   sp.s_umtime = time(0);
                   sp.s_first_blo += 1;


                   FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
                   //cout << "ME ESCIBO SUPER "<<PART_START<< endl;
                   fseek(bfile, PART_START, SEEK_SET);
                   fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

                   //ESCRIBIMOS EN LOS INODOS
                   //cout << "ME ESCIBO INODOS- "<<((inodo))<< endl;
                   fseek(bfile, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)), SEEK_SET);
                   fwrite(&ino, sizeof(structs::TabladeInodos), 1, bfile);

                   //ESCRIBIMOS EN LOS BLOQUES
                   //cout << "ME ESCIBO BLOQUE "<< sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas))<< endl;
                   fseek(bfile, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
                   fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

                   fseek(bfile, sp.s_bm_block_start+sp.s_first_blo-1, SEEK_SET);
                   fwrite("1", 1, 1, bfile);

                   fclose(bfile);

                   getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
                   getBlock_Carpets(&Bloque_carpeta, RUTA_PARTITTION, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)));
                   getInodo(&ino, RUTA_PARTITTION, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)));
                   block = ino.i_block[i];

                   int recuperar;
                   CreandoEnBloqueArchivos(sp, inodo, block, Bloque_carpeta, &recuperar);
                   *recuperar_inodo = recuperar;
                   return;
               }
           }

           //------------INICIAN BLOQUES INDIRECTOS----------------------
           int recuperar;
           CrearNivel1( sp, inodo, block, name, &recuperar);
           if(recuperar == -1){
               //cout << "ACCEDO AL INDIRECTO DOBLE"<< endl;
               CrearNivel2( sp, inodo, block, name, &recuperar);
               if(recuperar == -1){
                   //cout << "ACCEDO AL INDIRECTO TRIPLE"<< endl;
               }
           }
           *recuperar_inodo = recuperar;

           return;




}

void obmkdir::CrearNivel1(structs::SuperBloque sp, int inodo, int block, string name,  int *recuperar_inodo){
    structs::BloquesdeApuntadores B_A1;

    structs::TabladeInodos Inodos;
    getInodo(&Inodos, RUTA_PARTITTION, sp.s_inode_start + inodo*sizeof(structs::TabladeInodos));
    int numero_bloque_actual = Inodos.i_block[12];

    if(numero_bloque_actual == -1){
        //LLENAMOS EL INODO CON EL BLOQUE QUE CORRESPONDE
        Inodos.i_block[12] = sp.s_first_blo;
        //cout << "INODO -> BLOQUE APUNTADOR: "<< sp.s_first_blo << endl;

        sp.s_free_blocks_count -=1;
        sp.s_umtime = time(0);
        sp.s_first_blo += 1;

        //LLENAMOS EL BLOQUE APUNTADOR 1
        B_A1.b_pointers[0] = sp.s_first_blo;
        for(int k = 1; k<16; k++){
            B_A1.b_pointers[k] = -1;
        }

        //cout << "BLOQUE APUNTADOR -> BLOQUE CARPETA: "<<sp.s_first_blo<<endl;

        if(Inodos.i_type == '0'){
            //CREAR BLOQUE CARPETA

            structs::BloquesdeCarpetas Bloque_carpeta;
            Bloque_carpeta.b_content[0].b_inodo = sp.s_firts_ino ; //ACTUAL
            strcpy(Bloque_carpeta.b_content[0].b_name ,name.c_str());

            //SUPERBLOQUE
            sp.s_free_blocks_count -=1;
            sp.s_umtime = time(0);
            sp.s_first_blo += 1;


            FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
            fseek(bfile, PART_START, SEEK_SET);
            fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 1, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 1)*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
            fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

            block = sp.s_first_blo - 1;

            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 2, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            //cout << "EL APUNTADOR SE GUARDARA EN EL BLOQUE -> "<< sp.s_first_blo - 2 << endl;
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 2)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
            fwrite(&B_A1 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

            //ESCRIBIMOS EN LOS INODOS
            fseek(bfile, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)), SEEK_SET);
            fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

            fclose(bfile);

            getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
            getBlock_Carpets(&Bloque_carpeta, RUTA_PARTITTION, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)));
            getInodo(&Inodos, RUTA_PARTITTION, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)));


            int recuperar;
            CreandoEnBloqueArchivos(sp, inodo, block, Bloque_carpeta, &recuperar);
            *recuperar_inodo = recuperar;

            return;
        }
        else{
            //CREAR BLOQUE ARCHIVO
            structs::BloquesdeArchivos bloque_archivo;
            strcpy(bloque_archivo.b_content, name.c_str());

            sp.s_free_blocks_count -=1;
            sp.s_umtime = time(0);
            sp.s_first_blo += 1;


            FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
            fseek(bfile, PART_START, SEEK_SET);
            fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 1, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 1)*sizeof(structs::BloquesdeArchivos)), SEEK_SET);
            fwrite(&bloque_archivo, sizeof(structs::BloquesdeArchivos), 1, bfile);

            block = sp.s_first_blo - 1;

            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 2, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 2)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
            fwrite(&B_A1 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

            //ESCRIBIMOS EN LOS INODOS
            fseek(bfile, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)), SEEK_SET);
            fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

            fclose(bfile);
            return;
        }


    }
    else{

        getBlock_Apuntador(&B_A1, RUTA_PARTITTION, sp.s_block_start + numero_bloque_actual*sizeof(structs::BloquesdeApuntadores));
        //cout << "INODO -> BLOQUE APUNTADOR: "<< numero_bloque_actual << endl;
        //cout << "|||BLOQUE APUNTADOR -> BLOQUE CARPETA: "<< B_A1.b_pointers[0] <<endl;
        //cout << "BIT MAP -> "<< sp.s_first_blo << endl;
        for(int i = 0; i < 16; i++){
            if(B_A1.b_pointers[i] == -1){
                if(Inodos.i_type == '0'){
                    B_A1.b_pointers[i]  = sp.s_first_blo ;

                    structs::BloquesdeCarpetas Bloque_carpeta;
                    Bloque_carpeta.b_content[0].b_inodo = sp.s_firts_ino ; //ACTUAL
                    strcpy(Bloque_carpeta.b_content[0].b_name ,name.c_str());

                    //SUPERBLOQUE
                    sp.s_free_blocks_count -=1;
                    sp.s_umtime = time(0);
                    sp.s_first_blo += 1;


                    FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
                    fseek(bfile, PART_START, SEEK_SET);
                    fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

                    fseek(bfile, sp.s_block_start + (numero_bloque_actual*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&B_A1 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    fseek(bfile, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
                    fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

                    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo-1, SEEK_SET);
                    fwrite("1", 1, 1, bfile);

                    fclose(bfile);

                    getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
                    getBlock_Carpets(&Bloque_carpeta, RUTA_PARTITTION, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)));
                    getInodo(&Inodos, RUTA_PARTITTION, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)));
                    block = B_A1.b_pointers[i];

                    int recuperar;
                    CreandoEnBloqueArchivos(sp, inodo, block, Bloque_carpeta, &recuperar);
                    *recuperar_inodo = recuperar;
                    return;
                }
                else{
                    B_A1.b_pointers[i]  = sp.s_first_blo ;

                    structs::BloquesdeArchivos Bloque_archivos;
                    strcpy(Bloque_archivos.b_content , name.c_str());

                    //SUPERBLOQUE
                    sp.s_free_blocks_count -=1;
                    sp.s_umtime = time(0);
                    sp.s_first_blo += 1;


                    FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
                    fseek(bfile, PART_START, SEEK_SET);
                    fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

                    fseek(bfile, sp.s_block_start + (numero_bloque_actual*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&B_A1 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    fseek(bfile, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeArchivos)), SEEK_SET);
                    fwrite(&Bloque_archivos, sizeof(structs::BloquesdeArchivos), 1, bfile);

                    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo-1, SEEK_SET);
                    fwrite("1", 1, 1, bfile);

                    fclose(bfile);

                    return;

                }
            }
        }
        *recuperar_inodo = -1;
        cout << "-EL PRIMER INDIRECTO YA ESTA LLENO!!"<<endl;
        return;
    }


}

void obmkdir::CrearNivel2(structs::SuperBloque sp, int inodo, int block, string name,  int *recuperar_inodo){
    structs::BloquesdeApuntadores B_A1;
    structs::BloquesdeApuntadores B_A2;

    structs::TabladeInodos Inodos;
    getInodo(&Inodos, RUTA_PARTITTION, sp.s_inode_start + inodo*sizeof(structs::TabladeInodos));
    int numero_bloque_actual = Inodos.i_block[13];

    //cout << "inodo -> "<< numero_bloque_actual<< endl;
    if(numero_bloque_actual == -1){
        //LLENAMOS EL INODO CON EL BLOQUE QUE CORRESPONDE
        Inodos.i_block[13] = sp.s_first_blo;

        //------ SEGUNDO INDIRECTO------
        sp.s_free_blocks_count -=1;
        sp.s_umtime = time(0);
        sp.s_first_blo += 1;

        //LLENAMOS EL BLOQUE APUNTADOR 1
        B_A2.b_pointers[0] = sp.s_first_blo;
        for(int k = 1; k<16; k++){
            B_A2.b_pointers[k] = -1;
        }


        //------ PRIMER INDIRECTO----------
        sp.s_free_blocks_count -=1;
        sp.s_umtime = time(0);
        sp.s_first_blo += 1;

        //LLENAMOS EL BLOQUE APUNTADOR 1
        B_A1.b_pointers[0] = sp.s_first_blo;
        for(int k = 1; k<16; k++){
            B_A1.b_pointers[k] = -1;
        }



        if(Inodos.i_type == '0'){
            //CREAR BLOQUE CARPETA

            structs::BloquesdeCarpetas Bloque_carpeta;
            Bloque_carpeta.b_content[0].b_inodo = sp.s_firts_ino ; //ACTUAL
            strcpy(Bloque_carpeta.b_content[0].b_name ,name.c_str());

            //SUPERBLOQUE
            sp.s_free_blocks_count -=1;
            sp.s_umtime = time(0);
            sp.s_first_blo += 1;


            FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
            fseek(bfile, PART_START, SEEK_SET);
            fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 1, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 1)*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
            fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

            block = sp.s_first_blo - 1;

            //---- AGREGANDO PRIMER INDIRECTO ----------
            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 2, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 2)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
            fwrite(&B_A1 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

            //---- AGREGANDO SEGUNDO INDIRECTO ----------
            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 3, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 3)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
            fwrite(&B_A2 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

            //ESCRIBIMOS EN LOS INODOS
            fseek(bfile, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)), SEEK_SET);
            fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

            fclose(bfile);

            getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
            getBlock_Carpets(&Bloque_carpeta, RUTA_PARTITTION, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)));
            getInodo(&Inodos, RUTA_PARTITTION, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)));


            int recuperar;
            CreandoEnBloqueArchivos(sp, inodo, block, Bloque_carpeta, &recuperar);
            *recuperar_inodo = recuperar;

            return;
        }
        else{
            //CREAR BLOQUE ARCHIVO
            structs::BloquesdeArchivos bloque_archivo;
            //cout << "size -> "<< sizeof(name) << endl;
            strcpy(bloque_archivo.b_content, name.c_str());

            sp.s_free_blocks_count -=1;
            sp.s_umtime = time(0);
            sp.s_first_blo += 1;


            FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
            fseek(bfile, PART_START, SEEK_SET);
            fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 1, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 1)*sizeof(structs::BloquesdeArchivos)), SEEK_SET);
            fwrite(&bloque_archivo, sizeof(structs::BloquesdeArchivos), 1, bfile);

            block = sp.s_first_blo - 1;

            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 2, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 2)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
            fwrite(&B_A1 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

            //---- AGREGANDO SEGUNDO INDIRECTO ----------
            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 3, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 3)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
            fwrite(&B_A2 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

            //ESCRIBIMOS EN LOS INODOS
            fseek(bfile, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)), SEEK_SET);
            fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

            fclose(bfile);
            return;
        }


    }
    else{
        getBlock_Apuntador(&B_A2, RUTA_PARTITTION, sp.s_block_start + numero_bloque_actual*sizeof(structs::BloquesdeApuntadores));
        //cout << "--------------"<< endl;
        //cout << "BLOCK AP 2 -> "<< numero_bloque_actual << endl;

        for(int j = 0; j < 16; j++){
            //cout << "Ap 2 TOCA  aP1 -> "<< B_A2.b_pointers[j] << endl;
            getBlock_Apuntador(&B_A1, RUTA_PARTITTION, sp.s_block_start + B_A2.b_pointers[j]*sizeof(structs::BloquesdeApuntadores));
            if(B_A2.b_pointers[j] != -1){
                for(int i = 0; i < 16; i++){
                    if(B_A1.b_pointers[i] == -1){
                        if(Inodos.i_type == '0'){
                            B_A1.b_pointers[i]  = sp.s_first_blo ;

                            structs::BloquesdeCarpetas Bloque_carpeta;
                            Bloque_carpeta.b_content[0].b_inodo = sp.s_firts_ino ; //ACTUAL
                            strcpy(Bloque_carpeta.b_content[0].b_name ,name.c_str());

                            //SUPERBLOQUE
                            sp.s_free_blocks_count -=1;
                            sp.s_umtime = time(0);
                            sp.s_first_blo += 1;


                            FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
                            fseek(bfile, PART_START, SEEK_SET);
                            fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

                            fseek(bfile, sp.s_block_start + (B_A2.b_pointers[0]*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                            fwrite(&B_A1 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

                            //ESCRIBIMOS EN LOS BLOQUES
                            fseek(bfile, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
                            fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

                            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo-1, SEEK_SET);
                            fwrite("1", 1, 1, bfile);

                            fclose(bfile);

                            getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
                            getBlock_Carpets(&Bloque_carpeta, RUTA_PARTITTION, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)));
                            getInodo(&Inodos, RUTA_PARTITTION, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)));
                            block = B_A1.b_pointers[i];

                            int recuperar;
                            CreandoEnBloqueArchivos(sp, inodo, block, Bloque_carpeta, &recuperar);
                            *recuperar_inodo = recuperar;
                            return;
                        }
                        else{
                            B_A1.b_pointers[i]  = sp.s_first_blo ;
                            //cout << "B_A1 -> "<< sp.s_first_blo <<endl;

                            structs::BloquesdeArchivos Bloque_archivos;
                            strcpy(Bloque_archivos.b_content , name.c_str());

                            //SUPERBLOQUE
                            sp.s_free_blocks_count -=1;
                            sp.s_umtime = time(0);
                            sp.s_first_blo += 1;


                            FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
                            fseek(bfile, PART_START, SEEK_SET);
                            fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

                            //cout << "me toca el bloque -> "<< B_A2.b_pointers[j] <<endl;
                            fseek(bfile, sp.s_block_start + (B_A2.b_pointers[j]*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                            fwrite(&B_A1 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

                            //ESCRIBIMOS EN LOS BLOQUES
                            fseek(bfile, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeArchivos)), SEEK_SET);
                            fwrite(&Bloque_archivos, sizeof(structs::BloquesdeArchivos), 1, bfile);

                            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo-1, SEEK_SET);
                            fwrite("1", 1, 1, bfile);

                            fclose(bfile);

                            return;

                        }
                    }
                }
                //cout << "********************** YA ESTOY LLENO PRIMER INDIRECTO ********************* "<< endl;
            }
            else{
                //cout << "VOY A CREAR UN NUEVO INDIRECTO "<<sp.s_first_blo << endl;
                B_A2.b_pointers[j] = sp.s_first_blo;

                //------ PRIMER INDIRECTO----------
                sp.s_free_blocks_count -=1;
                sp.s_umtime = time(0);
                sp.s_first_blo += 1;

                structs::BloquesdeApuntadores tmp_apuntador;

                //LLENAMOS EL BLOQUE APUNTADOR 1
                tmp_apuntador.b_pointers[0] = sp.s_first_blo;
                for(int k = 1; k<16; k++){
                    tmp_apuntador.b_pointers[k] = -1;
                }


                if(Inodos.i_type == '0'){
                    //CREAR BLOQUE CARPETA

                    structs::BloquesdeCarpetas Bloque_carpeta;
                    Bloque_carpeta.b_content[0].b_inodo = sp.s_firts_ino ; //ACTUAL
                    strcpy(Bloque_carpeta.b_content[0].b_name ,name.c_str());

                    //SUPERBLOQUE
                    sp.s_free_blocks_count -=1;
                    sp.s_umtime = time(0);
                    sp.s_first_blo += 1;


                    FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
                    fseek(bfile, PART_START, SEEK_SET);
                    fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

                    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 1, SEEK_SET);
                    fwrite("1", 1, 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 1)*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
                    fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

                    block = sp.s_first_blo - 1;

                    //---- AGREGANDO PRIMER INDIRECTO ----------
                    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 2, SEEK_SET);
                    fwrite("1", 1, 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 2)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&tmp_apuntador , sizeof(structs::BloquesdeApuntadores), 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    //cout << "me guardo -> "<< Inodos.i_block[13] << endl;
                    fseek(bfile, sp.s_block_start + ((Inodos.i_block[13])*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&B_A2 , sizeof(structs::BloquesdeApuntadores), 1, bfile);


                    //ESCRIBIMOS EN LOS INODOS
                    fseek(bfile, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)), SEEK_SET);
                    fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

                    fclose(bfile);

                    getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
                    getBlock_Carpets(&Bloque_carpeta, RUTA_PARTITTION, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)));
                    getInodo(&Inodos, RUTA_PARTITTION, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)));


                    int recuperar;
                    CreandoEnBloqueArchivos(sp, inodo, block, Bloque_carpeta, &recuperar);
                    *recuperar_inodo = recuperar;

                    return;
                }
                else{
                    //CREAR BLOQUE ARCHIVO
                    structs::BloquesdeArchivos bloque_archivo;
                    //cout << "size -> "<< sizeof(name) << endl;
                    strcpy(bloque_archivo.b_content, name.c_str());

                    sp.s_free_blocks_count -=1;
                    sp.s_umtime = time(0);
                    sp.s_first_blo += 1;


                    FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
                    fseek(bfile, PART_START, SEEK_SET);
                    fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

                    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 1, SEEK_SET);
                    fwrite("1", 1, 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 1)*sizeof(structs::BloquesdeArchivos)), SEEK_SET);
                    fwrite(&bloque_archivo, sizeof(structs::BloquesdeArchivos), 1, bfile);

                    block = sp.s_first_blo - 1;

                    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 2, SEEK_SET);
                    fwrite("1", 1, 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 2)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&tmp_apuntador , sizeof(structs::BloquesdeApuntadores), 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    //cout << "me guardo -> "<< Inodos.i_block[13] << endl;
                    fseek(bfile, sp.s_block_start + ((Inodos.i_block[13])*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&B_A2 , sizeof(structs::BloquesdeApuntadores), 1, bfile);


                    //ESCRIBIMOS EN LOS INODOS
                    fseek(bfile, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)), SEEK_SET);
                    fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

                    fclose(bfile);
                    return;
                }

            }
        }


        *recuperar_inodo = -1;
        //cout << "-EL SEGUNDO INDIRECTO YA ESTA LLENO!!"<<endl;
        return;
    }


}

void obmkdir::CreandoEnBloqueArchivos(structs::SuperBloque sp, int inodo, int block, structs::BloquesdeCarpetas bc, int *recuperar_inodo){
    //SUPERBLOQUE
    sp.s_free_blocks_count -=1;
    sp.s_free_inodes_count -=1;
    sp.s_umtime = time(0);
    sp.s_firts_ino += 1;
    sp.s_first_blo += 1;

    *recuperar_inodo = sp.s_firts_ino -1;
    //cout << " inodo----- "<< sp.s_firts_ino << " - block "<< sp.s_first_blo -1 <<endl;
    //-------- TABLA DE INODOS 0 ---------
    structs::TabladeInodos Inodos;
    Inodos.i_type = '0';
    Inodos.i_gid = stoi(USUARIO_ONLINE.no_grupo);
    Inodos.i_uid = stoi(USUARIO_ONLINE.posicion);
    Inodos.i_size = 0;
    Inodos.i_ctime = time(0);
    Inodos.i_mtime = time(0);
    Inodos.i_block[0] = sp.s_first_blo- 1;
    for(int i = 1 ; i < 15; i++){
        Inodos.i_block[i] = -1;
    }


    //--------- BLOQUES CARPETAS 0 ----------------
    structs::BloquesdeCarpetas Bloque_carpeta;
    Bloque_carpeta.b_content[0].b_inodo = inodo; //PADRE
    Bloque_carpeta.b_content[1].b_inodo = sp.s_firts_ino -1; //ACTUAL
    strcpy(Bloque_carpeta.b_content[0].b_name ,"..");
    strcpy(Bloque_carpeta.b_content[1].b_name , ".");
    strcpy(Bloque_carpeta.b_content[2].b_name ,"");
    strcpy(Bloque_carpeta.b_content[3].b_name , "");


    FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
    //ESCRIBIMOS EN EL SUPERBLOQUE
    //cout << "ME ESCIBO SUPER "<<PART_START<< endl;
    fseek(bfile, PART_START, SEEK_SET);
    fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

    //ESCRIBIMOS EN EL BITMAP DE INODOS
    fseek(bfile, sp.s_bm_inode_start+sp.s_firts_ino-1, SEEK_SET);
    fwrite("1", 1, 1, bfile);

    //ESCRIBIMOS EN EL BITMAP DE BLOQUES
    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo-1, SEEK_SET);
    fwrite("1", 1, 1, bfile);

    //ESCRIBIMOS EN LOS INODOS
    cout << "MEEEE GUARDO "<< sp.s_first_blo -1 << endl;
    fseek(bfile, sp.s_inode_start + ((sp.s_firts_ino-1)*sizeof(structs::TabladeInodos)), SEEK_SET);
    fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

    //ESCRIBIMOS EN LOS BLOQUES
    fseek(bfile, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
    fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

    cout << "Bloque carpeta mod -> "<< block << endl;
    fseek(bfile, sp.s_block_start + (block*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
    fwrite(&bc, sizeof(structs::BloquesdeCarpetas), 1, bfile);

    fclose(bfile);

}

string obmkdir::NombreArchivo(string path){
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

list<string> obmkdir::separar_carpetas(string path) {
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

bool obmkdir::existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int *size_particion){
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

bool obmkdir::ExisteMBR(string ruta, structs::MBR *mbr){
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

bool obmkdir::ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part){
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

bool obmkdir::ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part){
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



