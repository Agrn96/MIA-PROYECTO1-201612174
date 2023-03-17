#include "obmkfs.h"
#include "obmount.cpp"
#include "iostream"
#include "structs.h"
#include "math.h"
using namespace std;

obmkfs::obmkfs()
{

}

void obmkfs::mostrardatos(obmkfs *disco){
    cout << "\n     ----COMANDO MKFS---\n"<<endl;

    if(disco->id == ""){
        cout << "NO SE INGRESO EL ID"<<endl;
        return;
    }
    string FS = ToUpperString(disco->fs);
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



    int n = 0;
    int n_superblock = 0;
    int n_jornaling = 0;
    int n_bitmapinodos = 0;
    int n_bitmapbloques = 0;
    int n_inodos = 0;
    int n_bloques = 0;

    int ext = 0;

    if(FS == "2FS"){
        n = (Size_Part - sizeof(structs::SuperBloque))/(4+sizeof(structs::TabladeInodos)+(3*sizeof(structs::BloquesdeCarpetas)));
        n = floor(n);
        ext = 2;
        n_jornaling = 0;
    }
    else if(FS == "3FS"){
        n = (Size_Part - sizeof(structs::SuperBloque)-(100*sizeof(structs::BloquesdeCarpetas)))/(4+sizeof(structs::TabladeInodos)+(3*sizeof(structs::BloquesdeCarpetas)));
        n = floor(n);
        ext = 3;
        n_jornaling = 100 * sizeof(structs::BloquesdeArchivos);
        Create_Jornaling(Start_Part, Ruta, Start_Part + sizeof(structs::SuperBloque) + n_jornaling);
    }
    else{
        cout << "EL SISTEMA DE ARCHIVOS A FORMATEAR ES INVALIDO!"<<endl;
    }

    n_superblock = sizeof(structs::SuperBloque);
    n_bitmapinodos = n;
    n_bitmapbloques = 3*n;
    n_inodos = n*sizeof(structs::TabladeInodos);
    n_bloques = 3*n*sizeof(structs::BloquesdeArchivos);

    int inicio_bitmap_inodos = Start_Part + sizeof(structs::SuperBloque)+ n_jornaling;
    int inicio_bitmap_bloques = inicio_bitmap_inodos + n_bitmapinodos;

    int inicio_inodo = inicio_bitmap_bloques  + n_bitmapbloques;
    int inicio_bloque = inicio_inodo + n_inodos;

    this->type = ToUpperString(this->type);
    if(this->type == "FULL"){
        for(int i = inicio_bitmap_inodos ; i < Start_Part + Size_Part; i ++){
            FILE *bfile = fopen(Ruta.c_str(), "rb+");
            fseek(bfile, i, SEEK_SET);
            fwrite("\0", 1, 1, bfile);
            fclose(bfile);
        }
    }


    //----------- SUPER BLOQUE ---------------
    structs::SuperBloque super;

    super.s_filesystem_type = ext;
    super.s_inodes_count = n_bitmapinodos;
    super.s_blocks_count = n_bitmapbloques;
    super.s_free_blocks_count = n_bitmapbloques -2;
    super.s_free_inodes_count = n_bitmapinodos - 2;
    super.s_mtime = time(0);
    super.s_umtime = time(0);
    //super.s_mnt_cout++;
    super.s_magic = 0xEF53;
    super.s_inode_size = sizeof(structs::TabladeInodos);
    super.s_block_size = sizeof(structs::BloquesdeCarpetas);
    super.s_firts_ino = 2;
    super.s_first_blo= 2;
    super.s_bm_inode_start = inicio_bitmap_inodos;
    super.s_bm_block_start = inicio_bitmap_bloques;
    super.s_inode_start = inicio_inodo;
    super.s_block_start = inicio_bloque;

    cout << "\n     ---DATOS--\n"<<endl;
    cout << "n: "<< n<<endl;
    cout << "size super: "<< sizeof(super)<<endl;
    cout << "size inodos: "<< sizeof(structs::TabladeInodos) <<endl;
    cout << "size blocks: "<< sizeof(structs::BloquesdeArchivos) <<endl;
    cout << "bm inode start: "<< inicio_bitmap_inodos<<endl;
    cout << "bm block start: "<< inicio_bitmap_bloques<<endl;
    cout << "inode start: "<< inicio_inodo<<endl;
    cout << "block start: "<<inicio_bloque<<endl;


    FILE *bfile = fopen(Ruta.c_str(), "rb+");
    fseek(bfile, Start_Part, SEEK_SET);
    fwrite(&super, sizeof(structs::SuperBloque), 1, bfile);
    fclose(bfile);

    //-------- TABLA DE INODOS 0 ---------
    structs::TabladeInodos Inodos;
    Inodos.i_type = '0';
    Inodos.i_gid = 1;
    Inodos.i_uid = 1;
    Inodos.i_size = 0;
    Inodos.i_ctime = time(0);
    Inodos.i_mtime = time(0);
    Inodos.i_block[0] = 0;
    for(int i = 1 ; i < 15; i++){
        Inodos.i_block[i] = -1;
    }


    //--------- BLOQUES CARPETAS 0 ----------------
    structs::BloquesdeCarpetas Bloque_carpeta;
    Bloque_carpeta.b_content[0].b_inodo = 0; //PADRE
    Bloque_carpeta.b_content[1].b_inodo = 0; //ACTUAL
    Bloque_carpeta.b_content[2].b_inodo = 1; //USUARIOS
    strcpy(Bloque_carpeta.b_content[0].b_name ,"..");
    strcpy(Bloque_carpeta.b_content[1].b_name , ".");
    strcpy(Bloque_carpeta.b_content[2].b_name , "users.txt");


    //-------- TABLA DE INODOS 1 ---------
    structs::TabladeInodos Inodos1;
    Inodos1.i_type = '1';
    Inodos1.i_gid = 1;
    Inodos1.i_uid = 1;
    Inodos1.i_size = sizeof("1,G,root\n1,U,root,root,123\n");
    Inodos1.i_ctime = time(0);
    Inodos1.i_mtime = time(0);
    Inodos1.i_block[0] = 1;
    for(int i = 1 ; i < 15; i++){
        Inodos1.i_block[i] = -1;
    }

    //--------- BLOQUES ARCHIVOS 1 ----------------
    structs::BloquesdeArchivos Bloque_archivo;
    strcpy(Bloque_archivo.b_content, "1,G,root\n1,U,root,root,123\n");


    char numero = '1';
    bfile = fopen(Ruta.c_str(), "rb+");
    fseek(bfile, inicio_bitmap_inodos, SEEK_SET);
    fwrite(&numero, 1, 1, bfile);
    fseek(bfile, inicio_bitmap_inodos+1, SEEK_SET);
    fwrite(&numero, 1, 1, bfile);

    fseek(bfile, inicio_bitmap_bloques, SEEK_SET);
    fwrite(&numero, 1, 1, bfile);

    fseek(bfile, inicio_bitmap_bloques+1, SEEK_SET);
        fwrite(&numero, 1, 1, bfile);

    numero = '0';
    for(int i = inicio_bitmap_inodos + 2; i < inicio_bitmap_bloques; i++){
        fseek(bfile, i, SEEK_SET);
        fwrite(&numero, 1, 1, bfile);
    }

    for(int i = inicio_bitmap_bloques + 2; i < inicio_inodo; i++){
        fseek(bfile, i, SEEK_SET);
        fwrite(&numero, 1, 1, bfile);
    }

    //------ ESCRIBO INODO 0 -----------
    fseek(bfile, inicio_inodo, SEEK_SET);
    fwrite(&Inodos, sizeof(structs::TabladeInodos), 1, bfile);

    //------- ESCRIBO BLOQUE CARPETA 0 ------------
    fseek(bfile, inicio_bloque, SEEK_SET);
    fwrite(&Bloque_carpeta, sizeof(structs::BloquesdeCarpetas), 1, bfile);

    //------ ESCRIBO INODO 1 -----------
    fseek(bfile, inicio_inodo+ sizeof(structs::TabladeInodos), SEEK_SET);
    fwrite(&Inodos1, sizeof(structs::TabladeInodos), 1, bfile);

    //------- ESCRIBO BLOQUE ARCHIVO 1 ------------
    fseek(bfile, inicio_bloque + sizeof(structs::BloquesdeCarpetas), SEEK_SET);
    fwrite(&Bloque_archivo, sizeof(structs::BloquesdeArchivos), 1, bfile);

    fclose(bfile);


    cout << "PROCESO EXITOSAMENTE!!"<<endl;

}


bool obmkfs::existelaparticionmontada(structs::DiscosMontados tmp[], string codigo, string *ruta, string *nombre, int *start_particion, int *size_particion){
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


bool obmkfs::ExisteMBR(string ruta, structs::MBR *mbr){
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



bool obmkfs::ExisteNombre(structs::MBR tmp_MBR, string name, int *Ext, int *start_part, int *size_part){
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


bool obmkfs::ExisteNombreLogica(string ruta, string name, int part_start_ext, int *start_part, int *size_part){
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

string obmkfs::ToUpperString(string cadena){
    string tmp = "";
    for(int i = 0; i < cadena.size(); i++){
        tmp += toupper(cadena[i]);
    }
    return tmp;
}

void obmkfs::Create_Jornaling(int part_Start, string ruta, int final){
    structs::Jornaling jg;
    string tipo = "mkfs";
    strcpy(jg.tipo_operacion, tipo.c_str());
    jg.tipo = '0';
    string Ruta = "/";
    strcpy(jg.path, Ruta.c_str());
    jg.log_fecha = time(0);
    jg.size = 0;

    FILE *archivo = fopen(ruta.c_str(), "rb+");
    fseek(archivo, part_Start +  sizeof(structs::SuperBloque), SEEK_SET);
    fwrite(&jg, sizeof(structs::Jornaling ), 1, archivo);
    fclose(archivo);




    int inicio = part_Start;
    structs::Jornaling jorna;
    for(int i = inicio+ sizeof(structs::SuperBloque)+sizeof(structs::Jornaling) ; i < final; i=i+sizeof(structs::Jornaling)){
        if((i + sizeof(structs::Jornaling)) < final){
            //cout << "CONTADOR -> "<<to_string(i) << endl;
            FILE *file = fopen(ruta.c_str(), "rb+");
            fseek(file, i, SEEK_SET);
            fwrite(&jorna, sizeof(structs::Jornaling), 1, file);
            fclose(file);
        }

    }
}





