#include "obtouch.h"
#include "iostream"
#include "list"
#include "fstream"
#include "obedit.h"
using namespace std;

static int PART_START;
static string RUTA_PARTITTION;

extern structs::DiscosMontados Discos_Montados[99];
extern structs::LOGIN USUARIO_ONLINE;

obtouch::obtouch()
{

}

void obtouch::mostrardatos(obtouch *disco){
     cout << "\n     ----COMANDO MKFILE----\n"<<endl;
    string Ruta = EliminarComillas(disco->path);
    string CONT = "";
    int SIZE = 0;
    bool stdin = disco->stdin;
    bool R = disco->r;
    if(disco->cont != ""){
        CONT = EliminarComillas(disco->cont);
    }
    else if(disco->size != 0){
        SIZE = disco->size;
    }
    else{
        cout << "NO SE INGRESADO STDIN, CONT o SIZE!!"<<endl;
        return;
    }
    //cout << "RUTAAAAA -> "<<Ruta<< disco->path<<endl;
    disco->size = 0;
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
    //cout << "PATH -> "<<Ruta_part << endl;
    //cout << "NOMBRE -> "<<Nombre << endl;
    //cout << "START -> "<<Start_Part << endl;
    //cout << "SIZE -> "<<Size_Part << endl;
    RUTA_PARTITTION = Ruta_part;
    PART_START = Start_Part;

    structs::SuperBloque super;
    getSuperBloque(&super, RUTA_PARTITTION, PART_START);

    //cout << "bm inode start: "<< super.s_bm_inode_start<<endl;
    //cout << "bm block start: "<< super.s_bm_block_start<<endl;
    //cout << "inode start: "<< super.s_inode_start<<endl;
    //cout << "block start: "<<super.s_block_start<<endl;
    //cout << "parte final inodos: "<< super.s_inode_start + super.s_inodes_count*sizeof(structs::TabladeInodos) << " count inodos: "<< super.s_inodes_count <<endl;

    string contenido = "";
    if(CONT != ""){
        ifstream in(this->cont);
        string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        contenido = contents;

    }
    else if(stdin){
        //cout << "INGRESAR CONTENIDO DE LA CARPETA: ";
        cin >> contenido;
    }
    else if(SIZE != 0){
        int con = 0;
        for(int i = 0; i < SIZE; i++){
            if(con == 10){
                con = 0;
            }
            contenido += to_string(con);
            con++;
        }
    }

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
        structs::TabladeInodos Inodo_actual; //RECUPERAMOS EL INO
        getInodo(&Inodo_actual, RUTA_PARTITTION, super.s_inode_start + contador_inodo*sizeof(structs::TabladeInodos));
        if(Inodo_actual.i_type != 'I'){
            bool Encontrelacarpeta = false;

            //BUSCAMOS LAS POSICIONES QUE ESTEN DISPONIBLES
            for(int j = 0; j < 15; j++){
                if(Inodo_actual.i_block[j] != -1){
                    contador_block_actual = j;
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
                                    getSuperBloque(&super, RUTA_PARTITTION, PART_START);
                                    obedit *editar = new obedit();
                                    editar->RUTA = RUTA_PARTITTION;
                                    editar->EMPIEZA = PART_START;
                                    editar->Modificar(super, BloqueCarpeta_Actual.b_content[k].b_inodo, contenido);
                                    cout << "EL ARCHIVO YA EXISTE, FUE MODIFICADO "<< endl;
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
                                            getSuperBloque(&super, RUTA_PARTITTION, PART_START);
                                            obedit *editar = new obedit();
                                            editar->RUTA = RUTA_PARTITTION;
                                            editar->EMPIEZA = PART_START;
                                            editar->Modificar(super, BloqueCarpeta_Actual.b_content[k].b_inodo, contenido);
                                            cout << "EL ARCHIVO YA EXISTE, FUE MODIFICADO "<< endl;
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
                                                    getSuperBloque(&super, RUTA_PARTITTION, PART_START);
                                                    obedit *editar = new obedit();
                                                    editar->RUTA = RUTA_PARTITTION;
                                                    editar->EMPIEZA = PART_START;
                                                    editar->Modificar(super, BloqueCarpeta_Actual.b_content[k].b_inodo, contenido);
                                                    cout << "EL ARCHIVO YA EXISTE, FUE MODIFICADO "<< endl;
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
            string nombre_aux = *it;
            //SI NO EXISTE LA CARPETA DENTRO DE LOS BLOQUES
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
    //return;
    getSuperBloque(&super, RUTA_PARTITTION, PART_START);

    CrearArchivos(super, contador_inodo, contenido);

    if(super.s_filesystem_type == 3){
        string tipo_o = "touch";
        CrearJornaling(tipo_o, '1', Ruta, contenido, contenido.length(), RUTA_PARTITTION, PART_START);
    }
    cout << "PROCESO EXITOSAMENTE"<<endl;


}

string obtouch::EliminarComillas(string cadena){
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

void obtouch::getSuperBloque(structs::SuperBloque *sp, string ruta, int part_Start){
    structs::SuperBloque aux;
    FILE *archivo = fopen(ruta.c_str(), "rb+");
    fseek(archivo, part_Start , SEEK_SET);
    fread(&aux, sizeof(structs::SuperBloque ), 1, archivo);
    fclose(archivo);
    *sp = aux;
}
bool obtouch::getInodo(structs::TabladeInodos*sp, string ruta, int part_Start){
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
bool obtouch::getBlock_Files(structs::BloquesdeArchivos *sp, string ruta, int part_Start){
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
bool obtouch::getBlock_Carpets(structs::BloquesdeCarpetas *sp, string ruta, int part_Start){
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
bool obtouch::getBlock_Apuntador(structs::BloquesdeApuntadores *sp, string ruta, int part_Start){
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

void obtouch::BuscarEspacio(structs::SuperBloque sp, int inodo, int block, structs::TabladeInodos ino, string name, int *recuperar_inodo){

           bool ExisteEspacioEnElBloque = false;
           //cout << "*INODO ENTRANTE: "<< inodo <<"  *BLOQUE ENTRANTE: "<< block<<endl;
           structs::BloquesdeCarpetas BloqueCarpeta_Actual;
           getBlock_Carpets(&BloqueCarpeta_Actual, RUTA_PARTITTION, sp.s_block_start + block*sizeof(structs::BloquesdeCarpetas));

           //NAVEGANDO ENTRE LOS ESPACIOS SI EXISTE UNO DISPONIBLE
           for(int j = 0; j < 4; j++){
               if(BloqueCarpeta_Actual.b_content[j].b_inodo == -1){
                   ExisteEspacioEnElBloque = true;
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
                   fseek(bfile, PART_START, SEEK_SET);
                   fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

                   fseek(bfile, sp.s_bm_block_start+sp.s_first_blo-1, SEEK_SET);
                   fwrite("1", 1, 1, bfile);

                   //ESCRIBIMOS EN LOS INODOS
                   fseek(bfile, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)), SEEK_SET);
                   fwrite(&ino, sizeof(structs::TabladeInodos), 1, bfile);

                   //ESCRIBIMOS EN LOS BLOQUES
                   fseek(bfile, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
                   fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

                   fclose(bfile);

                   getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
                   getBlock_Carpets(&Bloque_carpeta, RUTA_PARTITTION, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)));
                   getInodo(&ino, RUTA_PARTITTION, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)));
                   block = ino.i_block[i];

                   int recuperar;
                   CreandoEnBloqueArchivos(sp, inodo, block, Bloque_carpeta, &recuperar);
                   *recuperar_inodo = recuperar;

                   getInodo(&ino, RUTA_PARTITTION, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)));
                   //cout << "INODO "<<inodo << endl;
                   for(int k = 0; k < 15; k++){
                       //cout << "BLOCK["<<k<<"] = "<<ino.i_block[k] << endl;
                   }

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
                   //CrearNivel3( sp, inodo, block, name, &recuperar);
               }
           }
           *recuperar_inodo = recuperar;

        return;



}

void obtouch::CrearNivel1(structs::SuperBloque sp, int inodo, int block, string name,  int *recuperar_inodo){
    structs::BloquesdeApuntadores B_A1;

    structs::TabladeInodos Inodos;
    getInodo(&Inodos, RUTA_PARTITTION, sp.s_inode_start + inodo*sizeof(structs::TabladeInodos));
    int numero_bloque_actual = Inodos.i_block[12];
    //cout << " INDICE -> "<< numero_bloque_actual << endl;
    //cout << " tipo -> "<< Inodos.i_type << endl;
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

void obtouch::CrearNivel2(structs::SuperBloque sp, int inodo, int block, string name,  int *recuperar_inodo){
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

void obtouch::CrearNivel3(structs::SuperBloque sp, int inodo, int block, string name,  int *recuperar_inodo){
    structs::BloquesdeApuntadores B_A1;
    structs::BloquesdeApuntadores B_A2;
    structs::BloquesdeApuntadores B_A3;

    structs::TabladeInodos Inodos;
    getInodo(&Inodos, RUTA_PARTITTION, sp.s_inode_start + inodo*sizeof(structs::TabladeInodos));
    int numero_bloque_actual = Inodos.i_block[14];

    //cout << "inodo -> "<< numero_bloque_actual<< endl;
    if(numero_bloque_actual == -1){
        //LLENAMOS EL INODO CON EL BLOQUE QUE CORRESPONDE
        Inodos.i_block[14] = sp.s_first_blo;

        //------ TERCER INDIRECTO------
        sp.s_free_blocks_count -=1;
        sp.s_umtime = time(0);
        sp.s_first_blo += 1;

        //LLENAMOS EL BLOQUE APUNTADOR 1
        B_A3.b_pointers[0] = sp.s_first_blo;
        for(int k = 1; k<16; k++){
            B_A3.b_pointers[k] = -1;
        }

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

            //---- AGREGANDO TERCER INDIRECTO ----------
            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 4, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo -4)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
            fwrite(&B_A3 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

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

            //---- AGREGANDO TERCER INDIRECTO ----------
            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 4, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            //ESCRIBIMOS EN LOS BLOQUES
            fseek(bfile, sp.s_block_start + ((sp.s_first_blo -4)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
            fwrite(&B_A3 , sizeof(structs::BloquesdeApuntadores), 1, bfile);


            //ESCRIBIMOS EN LOS INODOS
            fseek(bfile, sp.s_inode_start + ((inodo)*sizeof(structs::TabladeInodos)), SEEK_SET);
            fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

            fclose(bfile);
            return;
        }


    }
    else{
        getBlock_Apuntador(&B_A3, RUTA_PARTITTION, sp.s_block_start + numero_bloque_actual*sizeof(structs::BloquesdeApuntadores));
        for(int m = 0; m < 16; m++){
            getBlock_Apuntador(&B_A2, RUTA_PARTITTION, sp.s_block_start + B_A3.b_pointers[m]*sizeof(structs::BloquesdeApuntadores));
            if(B_A3.b_pointers[m] != -1){
                for(int j = 0; j < 16; j++){
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


            }
            else{
                B_A3.b_pointers[m] = sp.s_first_blo;

                //------ SEGUNDO INDIRECTO----------
                sp.s_free_blocks_count -=1;
                sp.s_umtime = time(0);
                sp.s_first_blo += 1;

                structs::BloquesdeApuntadores tmp_apuntador2;

                //LLENAMOS EL BLOQUE APUNTADOR 1
                tmp_apuntador2.b_pointers[0] = sp.s_first_blo;
                for(int k = 1; k<16; k++){
                    tmp_apuntador2.b_pointers[k] = -1;
                }

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

                    //---- AGREGANDO PRIMER INDIRECTO ----------
                    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 3, SEEK_SET);
                    fwrite("1", 1, 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 3)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&tmp_apuntador2 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    //cout << "me guardo -> "<< Inodos.i_block[13] << endl;
                    fseek(bfile, sp.s_block_start + ((Inodos.i_block[14])*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&B_A3 , sizeof(structs::BloquesdeApuntadores), 1, bfile);


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

                    //---- AGREGANDO PRIMER INDIRECTO ----------
                    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo- 3, SEEK_SET);
                    fwrite("1", 1, 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    fseek(bfile, sp.s_block_start + ((sp.s_first_blo - 3)*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&tmp_apuntador2 , sizeof(structs::BloquesdeApuntadores), 1, bfile);

                    //ESCRIBIMOS EN LOS BLOQUES
                    //cout << "me guardo -> "<< Inodos.i_block[13] << endl;
                    fseek(bfile, sp.s_block_start + ((Inodos.i_block[14])*sizeof(structs::BloquesdeApuntadores)), SEEK_SET);
                    fwrite(&B_A3 , sizeof(structs::BloquesdeApuntadores), 1, bfile);


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

void obtouch::CreandoEnBloqueArchivos(structs::SuperBloque sp, int inodo, int block, structs::BloquesdeCarpetas bc, int *recuperar_inodo){
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

    //cout<< "--SIZE SUPER"<< sizeof(structs::SuperBloque)<<endl;
    //cout<< "--SIZE INODO "<< sizeof(structs::TabladeInodos)<<endl;
    //cout<< "--SIZE BLOCK "<< sizeof(structs::BloquesdeArchivos)<<endl;

    FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
    //ESCRIBIMOS EN EL SUPERBLOQUE
    //cout << "ME ESCIBO SUPER "<<PART_START<< endl;
    fseek(bfile, PART_START, SEEK_SET);
    fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

    //ESCRIBIMOS EN EL BITMAP DE INODOS
    //cout << "ME ESCIBO BM INODOS "<<sp.s_bm_inode_start+sp.s_firts_ino << endl;
    fseek(bfile, sp.s_bm_inode_start+sp.s_firts_ino-1, SEEK_SET);
    fwrite("1", 1, 1, bfile);

    //ESCRIBIMOS EN EL BITMAP DE BLOQUES
    //cout << "ME ESCIBO BM BLOQUES "<<sp.s_bm_block_start+sp.s_first_blo << endl;
    fseek(bfile, sp.s_bm_block_start+sp.s_first_blo-1, SEEK_SET);
    fwrite("1", 1, 1, bfile);

    //ESCRIBIMOS EN LOS INODOS
    //cout << "ME ESCIBO INODOS "<< sp.s_inode_start + ((sp.s_firts_ino-1)*sizeof(structs::TabladeInodos))<< endl;
    fseek(bfile, sp.s_inode_start + ((sp.s_firts_ino-1)*sizeof(structs::TabladeInodos)), SEEK_SET);
    fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

    //ESCRIBIMOS EN LOS BLOQUES
    //cout << "ME ESCIBO BLOQUE "<< sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas))<< endl;
    fseek(bfile, sp.s_block_start + ((sp.s_first_blo -1)*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
    fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

    //cout << "Bloque carpeta mod -> "<< block << endl;
    //cout << "ME ESCIBO BLOQUE ANTERIOR"<< sp.s_block_start + (block*sizeof(structs::BloquesdeCarpetas))<< endl;
    fseek(bfile, sp.s_block_start + (block*sizeof(structs::BloquesdeCarpetas)), SEEK_SET);
    fwrite(&bc, sizeof(structs::BloquesdeCarpetas), 1, bfile);

    /*for(int i = 0; i<15; i++){
        //cout << " i-> "<<Inodos.i_block[i]<< endl;
    }
    for(int i = 0; i<4; i++){
        //cout << "B-> "<<Bloque_carpeta.b_content[i].b_name << " - "<< Bloque_carpeta.b_content[i].b_inodo << endl;
    }*/




    fclose(bfile);
    //cout<< "NO INODO -> "<< sp.s_firts_ino-1 << " NO BLOQUE CARPT -> "<< sp.s_first_blo-1 <<endl;
    //cout << "SE AGREGO EL INODO Y BLOQUE DE CARPETA EXITOSAMENTE"<<endl;

}

void obtouch::CrearArchivos(structs::SuperBloque sp, int inodo, string entrada){

    structs::TabladeInodos Inodo_actual; //RECUPERAMOS EL INO
    //cout << "Voy a buscar inodo en: "<< sp.s_inode_start + inodo*sizeof(structs::TabladeInodos)<<endl;
    getInodo(&Inodo_actual, RUTA_PARTITTION, sp.s_inode_start + inodo*sizeof(structs::TabladeInodos));
    Inodo_actual.i_mtime = time(0);
    Inodo_actual.i_type = '1';
    Inodo_actual.i_size = sizeof(entrada);

    FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
    fseek(bfile, sp.s_inode_start + (inodo*sizeof(structs::TabladeInodos)), SEEK_SET);
    fwrite(&Inodo_actual, sizeof(structs::TabladeInodos), 1, bfile);
    fclose(bfile);

    string contenido = entrada;
    int contador_contenido = contenido.length();



    bool YaNoHayContenido = false;
    string aux = "";
    structs::BloquesdeArchivos Bloque_Archivo;

    int contador_block = Inodo_actual.i_block[0];
    for(int i = 0; i < 15; i++){
        aux = "";
        getInodo(&Inodo_actual, RUTA_PARTITTION, sp.s_inode_start + inodo*sizeof(structs::TabladeInodos));

        for(int j = 0; j < 64; j++){
            if(contador_contenido  == 0){
                YaNoHayContenido = true;
                //cout << "TERMINE DE LLENAR "<< endl;
                break;
            }
            else{
                aux += contenido[j];
                contador_contenido--;
            }
        }

        if(i < 12){
            //SUPERBLOQUE
            sp.s_free_blocks_count -=1;
            sp.s_umtime = time(0);
            sp.s_first_blo += 1;

            Inodo_actual.i_block[i] = contador_block;

            strcpy(Bloque_Archivo.b_content, aux.c_str());

            FILE *bfile = fopen(RUTA_PARTITTION.c_str(), "rb+");
            fseek(bfile, PART_START, SEEK_SET);
            fwrite(&sp, sizeof(structs::SuperBloque), 1, bfile);

            fseek(bfile, sp.s_bm_block_start+sp.s_first_blo-1, SEEK_SET);
            fwrite("1", 1, 1, bfile);

            fseek(bfile, sp.s_inode_start + (inodo*sizeof(structs::TabladeInodos)), SEEK_SET);
            fwrite(&Inodo_actual, sizeof(structs::TabladeInodos), 1, bfile);

            fseek(bfile, sp.s_block_start + (contador_block*sizeof(structs::BloquesdeArchivos)), SEEK_SET);
            fwrite(&Bloque_Archivo, sizeof(structs::BloquesdeArchivos), 1, bfile);

            fclose(bfile);

            getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
            contador_block  = sp.s_first_blo -1;
        }
        else if(i == 12){
            //cout << "ESTOY LLENANDO INDIRECTO 1"<<endl;
            getSuperBloque(&sp, RUTA_PARTITTION, PART_START);


            int recuperar;
            CrearNivel1(sp, inodo, 0, aux, &recuperar);
            if(recuperar != -1){
                i--;
            }
        }
        else if(i == 13){
            //cout << "ESTOY LLENANDO INDIRECTO 2"<<endl;
            getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
            //cout << "contenido "<< aux << " -- "<< contenido.length() << endl;
            int recuperar = 0;
            CrearNivel2(sp, inodo, 0, aux, &recuperar);
            if(recuperar != -1){
                i--;
            }
        }
        else if(i == 14){
            //cout << "ESTOY LLENANDO INDIRECTO 2"<<endl;
            getSuperBloque(&sp, RUTA_PARTITTION, PART_START);
            //cout << "contenido "<< aux << " -- "<< contenido.length() << endl;
            int recuperar = 0;
            CrearNivel3(sp, inodo, 0, aux, &recuperar);
            if(recuperar != -1){
                i--;
            }
        }


        if(YaNoHayContenido){
            break;
        }
        else{
            contenido = contenido.substr(64, contenido.length());
        }
    }
}

string obtouch::NombreArchivo(string path){
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

void obtouch::CrearJornaling(string operacion, char tipo, string path, string contenido, int size, string ruta, int inicio){
    structs::SuperBloque super;
    getSuperBloque(&super, ruta, inicio);

    list<structs::Jornaling> jg;
    getJornaling(&jg, super, ruta, inicio);

    int contador = 0;
    for(structs::Jornaling j: jg){
        if(j.size == -1){
            break;
        }
        contador++;

    }
    contador = inicio+sizeof(structs::SuperBloque) + (contador*sizeof(structs::Jornaling));

    //cout << contenido.length() << " >  99"<< endl;
    string aux_contenido = "";
    if(contenido.length() > 99){
        //cout << "Entre aqui"<< endl;
        for(int i = 0; i < 99; i++){
            aux_contenido += contenido[i];
        }

    }
    else{
        aux_contenido = contenido;
    }

    structs::Jornaling new_jg;

    //cout << ". -> "<<new_jg.tipo_operacion<<endl;
    new_jg.tipo = tipo;
    strcpy(new_jg.path, path.c_str());
    strcpy(new_jg.contenido, aux_contenido.c_str());
    new_jg.log_fecha = time(0);
    new_jg.size = size;
    strcpy(new_jg.tipo_operacion, operacion.c_str());
    setJornaling(new_jg, super, ruta, contador);


}
void obtouch::getJornaling(list<structs::Jornaling> *jg, structs::SuperBloque super, string ruta, int inicio){
    list<structs::Jornaling> jg_aux;
    structs::Jornaling jorna;
    for(int i = inicio+ sizeof(structs::SuperBloque) ; i < super.s_bm_inode_start-sizeof(structs::Jornaling); i = i + sizeof(structs::Jornaling)){
        //cout << "CONTADOR -> "<<to_string(i) << endl;
        FILE *file = fopen(ruta.c_str(), "rb+");
        fseek(file, i, SEEK_SET);
        fread(&jorna, sizeof(structs::Jornaling), 1, file);
        fclose(file);
        jg_aux.push_back(jorna);
    }
    *jg = jg_aux;
}
void obtouch::setJornaling(structs::Jornaling jg, structs::SuperBloque super, string ruta, int inicio){
    //cout<< inicio<< " < "<< super.s_bm_inode_start << endl;
    if(inicio+sizeof(structs::Jornaling) < super.s_bm_inode_start){
        //cout << "ENTREE!! "<<endl;
        FILE *file = fopen(ruta.c_str(), "rb+");
        fseek(file, inicio, SEEK_SET);
        fwrite(&jg, sizeof(structs::Jornaling), 1, file);
        fclose(file);
    }

}

list<string> obtouch::separar_carpetas(string path) {
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

bool obtouch::existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int *size_particion){
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

bool obtouch::ExisteMBR(string ruta, structs::MBR *mbr){
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

bool obtouch::ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part){
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

bool obtouch::ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part){
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





