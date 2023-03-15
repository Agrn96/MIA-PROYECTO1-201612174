#include "obfdisk.h"
#include "iostream"
#include "structs.h"
#include "vector"
using namespace std;

obfdisk::obfdisk()
{

}

void obfdisk::mostrardatos(obfdisk *disco){
     cout << "\n     ----COMANDO FDISK----\n"<<endl;
    if(disco->path == ""){
        cout << "ERROR, NO SE AGREGO PATH, COMANDO OBLIGATORIO"<<endl;
        return;
    }

    if(disco->name == ""){
        cout << "ERROR, NO SE AGREGO NAME, COMANDO OBLIGATORIO"<<endl;
        return;
    }

    /*printf("\n---------DATOS----------\n");
    cout<<"-SIZE = "<< disco->size <<endl;
    cout<<"-UNIT = "<< disco->unit <<endl;
    cout<<"-PATH = "<< disco->path <<endl;
    cout<<"-FIT = "<< disco->fit <<endl;
    cout<<"-TYPE = "<< disco->type <<endl;
    cout<<"-NAME = "<< disco->name <<endl;
    cout<<"-DELETE = "<< disco->delet <<endl;
    cout<<"-ADD_SIZE = "<< disco->add_size <<endl;
    cout<<"-ADD_TYPE = "<< disco->add_type <<endl;*/

    //cout << "add_size -> "<< disco->add_size<<endl;
    if(disco->add_size != 0){
        disco->add(disco);
    }
    else if(disco->delet != ""){
        disco->deleter(disco);
    }
    else{
        disco->add_size = 0;
        disco->create(disco);
    }
}

void obfdisk::create(obfdisk *disco){

    if(disco->size == 0){
        cout << "ERROR, NO SE AGREGO SIZE, COMANDO OBLIGATORIO"<<endl;
        return;
    }

    structs::partition new_partition;
    int size = 0;
    string  fit,type;

    if((disco->unit == "M") || (disco->unit == "m")){
        size = disco->size * 1024 * 1024;
    }
    else if((disco->unit == "K") || (disco->unit == "k")){
        size = disco->size * 1024;
    }
    else if((disco->unit == "B") || (disco->unit == "b")){
        size = disco->size;
    }
    else{
        size = disco->size * 1024;
        cout << "LA UNIDAD INGRESADA ES ERRONEA, SE TOMARA LA UNIDAD DE KILOBYTES"<<endl;
    }

    if((disco->type == "P") || (disco->type == "p")){
        type = "P";
        disco->type = 'P';
    }
    else if((disco->type == "E") || (disco->type == "e")){
        type = "E";
        disco->type = 'E';
    }
    else if((disco->type == "L") || (disco->type == "l")){
        type = "L";
        disco->type = 'L';
    }
    else{
        cout << "EL TIPO INGRESADO ES ERRONEO, SE TOMARA COMO PARTICION PRIMARIA"<<endl;
    }



    if((disco->fit == "BF") || (disco->fit == "Bf") || (disco->fit == "bF") || (disco->fit == "bf")){
        fit = "B";
        disco->fit = fit;
    }
    else if((disco->fit == "FF") || (disco->fit == "Ff") || (disco->fit == "fF") || (disco->fit == "ff")){
        fit = "F";
        disco->fit = fit;
    }
    else if((disco->fit == "WF") || (disco->fit == "Wf") || (disco->fit == "wF") || (disco->fit == "wf")){
        fit = "W";
        disco->fit = fit;
    }
    else{
        cout << "EL FIT INGRESADO ES ERRONEO, SE TOMARA COMO PEOR AJUSTE (WF)"<<endl;
    }
    //cout << "-FIT -> "<< fit << endl;

    string Rename;
    if(disco->name[0] == '"'){
        for(int i = 1; i < disco->name.size()-1; i++){ //ELIMINAR COMILLAS
            Rename+= disco->name[i];
        }
    }else{
        Rename = disco->name;
    }

    new_partition.size = size;
    new_partition.type = toupper(type[0]);
    new_partition.status = '1';
    new_partition.fit = toupper(fit[0]);
    strcpy(new_partition.name , Rename.c_str());

    //------------- QUITAR LAS COMILLAS --------------------
    string cadenas = disco->path;
    string ruta;
    if(cadenas[0] == '"'){
        for(int i = 1; i < cadenas.size()-1; i++){ //ELIMINAR COMILLAS DE LA RUTA
            ruta += cadenas[i];
        }
    }else{
        ruta = cadenas;
    }
    disco->path = ruta;

    //-----------RECUPERAMOS DATOS DEL DISCO-------------------------
    structs::MBR tmp_disco;
    FILE *file = fopen(ruta.c_str(), "rb+");
    if (file != NULL){
        fseek(file, 0, SEEK_SET);
        fread(&tmp_disco, sizeof(tmp_disco), 1, file); //RECUPERAR LOS DATOS DEL DISCO
    }
    else{
        cout << "Error. Path no existente." << endl;
        return;
    }
    fclose(file);


    //----------EMPIEZAN LAS VERIFICACIONES-------------------
    bool EstaLleno = true;
    int NumPartittion;
    int Size_Disk = tmp_disco.mbr_size;



    //VERIFICAR SI HAY PARTICIONES DISPONIBLES
    for(int i = 0; i < 4; i++){

        if(tmp_disco.mbr_partitions[i].status == '0'){
            EstaLleno = false;
            NumPartittion = i;
            cout<<"PARTICION DISPONIBLE" <<endl;
            break;
        }
    }

    //SI NO HAY PARTICIONES DISPONIBLES MOSTRAR MENSAJE
    if(EstaLleno && (type != "L")){
        cout<<"EL DISCO ESTA LLENO-" <<endl;
        return;
    }



    //VALIDAR SI YA EXISTE UNA PARTICION EXTENDIDA
    if(new_partition.type == 'E'){
        for(int i = 0; i < 4; i++){
            if(tmp_disco.mbr_partitions[i].type == 'E'){
                cout<<"¡YA EXISTE UNA PARTICION EXTENDIDA!" <<endl;
                return;
            }
        }
    }


    //VERIFICAR SI EL NOMBRE ES REPETIDO
    for(int i = 0; i < 4; i++){
        if(tmp_disco.mbr_partitions[i].name == name){
            cout<<"¡YA EXISTE UNA PARTICION CON EL NOMBRE "<< name <<endl;
            return;
        }
    }


    /*for(int i = 0; i < 4; i++){
        cout << "\n---- name " << tmp_disco.mbr_partitions[i].name << endl;
        cout << "-status " << tmp_disco.mbr_partitions[i].status << endl;
        cout << "-size " << tmp_disco.mbr_partitions[i].size << endl;
        cout << "-start " << tmp_disco.mbr_partitions[i].part_start<< endl;
        cout << "-fit " << tmp_disco.mbr_partitions[i].fit<< endl;
        cout << "-type " << tmp_disco.mbr_partitions[i].type<< endl;
        cout << "-------------------------------" << endl;
    }*/



    //-------------------------------------------------------------------------------------------------------
    // -------------------------------- EMPIEZA LA PARTICION -------------------
    int contador_espacios = 0;
    int sizes_fits[4];
    int starts_fits[4];

    //LIMPIAR LOS VECTORES
    //cout<<"LIMPIOS--- \n" << endl;
    for(int i = 0; i < 4; i++){
        starts_fits[i] = -1;
        sizes_fits[i] = -1;
        //cout<<"INICIO DE ESPACIO -> "<< starts_fits[i] << "|| TAMAÑO DE ESPACIO -> "<< sizes_fits[i] << endl;
    }

    if(type != "L"){
        //----------------------------------------------------------------------------
        //------------------------- PARTICION PRIMARIA Y EXTENDIDA--------------------
        //----------------------------------------------------------------------------
        //OBTENIENDO ESPACOS DE TODO EL DISCO
        for(int i = 0; i < 4; i++){
            if(tmp_disco.mbr_partitions[i].status == '1'){
                //cout << "STATUS 1 PARTICION "<< i << endl;
                //VERIFICAR SI AL PRINCIPIO DESPUES DEL MBR HAY UN ESPACIO
                if((tmp_disco.mbr_partitions[i].status == '1') && (tmp_disco.mbr_partitions[i].part_start != sizeof(tmp_disco)) && ( i == 0 )){

                    starts_fits[contador_espacios] = sizeof(tmp_disco);
                    //cout << "STATUS 1 PARTICION -> tiene que ser 0 ->  "<< i << endl;
                    //VERIFICAR SI EXISTE UNA SIGUIENTE PARTICION
                    if(tmp_disco.mbr_partitions[i+1].status == '1'){
                        //cout << tmp_disco.mbr_partitions[i].part_start<< " == " << sizeof(tmp_disco) << endl;
                        if(tmp_disco.mbr_partitions[i].part_start != sizeof(tmp_disco)){
                            sizes_fits[contador_espacios] = tmp_disco.mbr_partitions[i].part_start - sizeof(tmp_disco);
                            contador_espacios++;

                            starts_fits[contador_espacios] = tmp_disco.mbr_partitions[i].part_start + tmp_disco.mbr_partitions[i].size;
                            sizes_fits[contador_espacios]  = tmp_disco.mbr_partitions[i+1].part_start - (tmp_disco.mbr_partitions[i].part_start + tmp_disco.mbr_partitions[i].size);

                        }else{
                            //cout << "VERIFICAR SI EXISTE UNA SIGUIENTE PARTICION ->  "<< i << endl;
                            sizes_fits[contador_espacios] = tmp_disco.mbr_partitions[i+1].part_start - tmp_disco.mbr_partitions[i].size - sizeof(tmp_disco);
                            //cout<< tmp_disco.mbr_partitions[i+1].part_start << " - " << sizeof(tmp_disco) << " " << sizes_fits[contador_espacios] << endl;
                        }
                    }
                    else{
                    //cout << "SI YA NO HAY PARTICIONES TOMA EL TAMAÑO DEL ESPACIO CON EL FINAL DE LA PARTICION ->  "<< i << endl;
                    //SI YA NO HAY PARTICIONES TOMA EL TAMAÑO DEL ESPACIO CON EL FINAL DE LA PARTICION
                    sizes_fits[contador_espacios] = tmp_disco.mbr_size - starts_fits[contador_espacios];
                    contador_espacios++;
                    break;
                    }
                    contador_espacios++;
                }
                else{
                    //cout << "STATUS 0 PARTICION --->"<< i << endl;
                    starts_fits[contador_espacios] = tmp_disco.mbr_partitions[i].part_start + tmp_disco.mbr_partitions[i].size;

                    if((i < 3) && (tmp_disco.mbr_partitions[i+1].status == '1')){
                        //cout << "IF --->"<< i << endl;
                        sizes_fits[contador_espacios] = tmp_disco.mbr_partitions[i+1].part_start -  starts_fits[contador_espacios];
                    }
                    else{
                        //cout << "ELSE --->"<< i << endl;
                        cout << tmp_disco.mbr_partitions[i].part_start << endl;
                        sizes_fits[contador_espacios] = tmp_disco.mbr_size - starts_fits[contador_espacios];
                        contador_espacios++;
                        break;
                    }
                    contador_espacios++;
                }
            }
            else{
                //cout << "STATUS 0 PARTICION "<< i << endl;
                if(i == 0){
                    //cout << "IF "<< i << endl;
                    starts_fits[contador_espacios] = sizeof(tmp_disco);
                    sizes_fits[contador_espacios] = tmp_disco.mbr_size - sizeof(tmp_disco);
                    contador_espacios++;
                }
                else{
                    //cout << "ELSE "<< i << endl;
                    starts_fits[contador_espacios] = tmp_disco.mbr_partitions[i].part_start + tmp_disco.mbr_partitions[i].size;
                    sizes_fits[contador_espacios] = tmp_disco.mbr_size - starts_fits[contador_espacios];
                    contador_espacios++;
                }
                break;
            }
        }

        //------------------EMPEZAMOS A AGREGAR PARTICIONES----------------------
        bool ExisteEspacio;

        //VERIFICAR EL FIT
        if(fit == "F"){
            for(int i = 0; i < 4; i++){
                //cout << "-Start: " << starts_fits[i] << "-size "<< sizes_fits[i]<<endl;
                //cout  << disco->size << " >= "<< sizes_fits[i] <<endl;
                if(disco->size <= sizes_fits[i]){
                    new_partition.part_start = starts_fits[i];
                    ExisteEspacio = false;
                    break;
                }
            }
            if(ExisteEspacio){
                cout << "NO EXISTE UN ESPACIO PARA LA PARTICION " << disco->name <<endl;
                return;
            }
        }
        else{
            /*cout<<"INICIO DE ESPACIOsss--- \n" << endl;
            for(int i = 0; i < 4; i++){
                cout<<"INICIO DE ESPACIO -> "<< starts_fits[i] << "|| TAMAÑO DE ESPACIO -> "<< sizes_fits[i] << endl;
            }*/


            //ORDENAMIENTO DE BURBUJA PARA LOS ESPACIOS
            int aux_size, aux_start;
            for(int i = 1; i < 4; i++){
                for(int j = 0; j < 4-i; j++){
                    if(sizes_fits[j] > sizes_fits[j+1]){
                        //CAMBIAR TAMAÑOS
                        aux_size = sizes_fits[j+1];
                        sizes_fits[j+1] = sizes_fits[j];
                        sizes_fits[j] = aux_size;

                        //CAMBIAR PARTE INICIAL
                        aux_start = starts_fits[j+1];
                        starts_fits[j+1] = starts_fits[j];
                        starts_fits[j] = aux_start;
                    }
                }
            }
            //--------------------------------------------------------------


            if(fit == "B"){
                for(int i = 0; i < 4; i++){
                    if((disco->size <= sizes_fits[i]) && (sizes_fits[i] > 0)){
                       new_partition.part_start = starts_fits[i];
                        ExisteEspacio = false;
                        break;
                    }
                }
            }
            else if(fit == "W"){

                for(int i = 3; i >= 0; i--){
                   if((disco->size <= sizes_fits[i]) && (sizes_fits[i] > 0)){
                        new_partition.part_start = starts_fits[i];
                        ExisteEspacio = false;
                        break;
                    }
                }

            }
            else{
                cout << "OCURRIO UN ERROR CON EL FIT DE LA PARTICION " << disco->name <<endl;
                return;
            }

            if(ExisteEspacio){
                cout << "NO EXISTE UN ESPACIO PARA LA PARTICION " << disco->name <<endl;
                return;
            }
        }

        //GUARDAMOS LA PARTICION EN EL MBR
        tmp_disco.mbr_partitions[NumPartittion] = new_partition;


        //----------------------ORDENAMIENTO DE BURBUJA------------------
        structs::partition tmp_partition;
        for(int i = 1; i < 4; i++){
            for(int j = 0; j < 4 - i; j++){
                if((tmp_disco.mbr_partitions[j].part_start > tmp_disco.mbr_partitions[j + 1].part_start)){
                    //cout << "ordenare -> "<< tmp_disco.mbr_partitions[j].part_start << " con "<< tmp_disco.mbr_partitions[j+1].part_start << endl;
                    tmp_partition = tmp_disco.mbr_partitions[j+1];
                    tmp_disco.mbr_partitions[j+1] = tmp_disco.mbr_partitions[j];
                    tmp_disco.mbr_partitions[j] = tmp_partition;
                }
            }
        }
        //LLENAR UNA LISTA AUXILIAR CON LAS PARTICIONES ACTIVAS
        int contador = 0;
        structs::partition tmp_aux[4];
        for(int i = 0; i< 4 ; i++){
            if(tmp_disco.mbr_partitions[i].status == '1'){
                tmp_aux[contador] = tmp_disco.mbr_partitions[i];
                contador++;
            }
        }
        //REMPLAZAR LAS PARTICIONES EN EL MBR
        for(int i = 0; i< 4 ; i++){
               tmp_disco.mbr_partitions[i] = tmp_aux[i];
        }


        // ----------------------- GUARDAMOS LA PARTICION SI TUBO EXITO TODO EL PROCEDIMIENTO
        FILE *bfile = fopen(ruta.c_str(), "rb+");
        if (bfile != NULL)
        {
            fseek(bfile, 0, SEEK_SET);
            fwrite(&tmp_disco, sizeof(structs::MBR), 1, bfile);
            if (type == "E"){
                structs::EBR ebr;
                ebr.part_start = sizeof(ebr) + new_partition.part_start;
                ebr.status = '0';
                //cout << " -start extendida ebr "<< new_partition.part_start << endl;
                fseek(bfile,  new_partition.part_start , SEEK_SET);
                fwrite(&ebr, sizeof(structs::EBR), 1, bfile);
            }
        }
        fclose(bfile);
        cout << "          ¡SE CREO LA PARTICION EXITOSAMENTE!" << endl;
    }
    else{
        //---------------------------------------------------------------
        //------------------------- PARTICION LOGICA --------------------
        //---------------------------------------------------------------
        bool ExisteExtendida = false;
        int start_extendida = 0;
        int finish_extendida = 0;


        //VALIDAR SI EXISTE UNA PARTICION EXTENDIDA
        for(int i = 0; i < 4; i++){
            //cout << "-TYPE -> " << tmp_disco.mbr_partitions[i].type << endl;
            if(tmp_disco.mbr_partitions[i].type == 'E'){

                start_extendida = tmp_disco.mbr_partitions[i].part_start;
                finish_extendida = tmp_disco.mbr_partitions[i].part_start + tmp_disco.mbr_partitions[i].size;
                ExisteExtendida = true;
                break;
            }
        }
        //cout << " START EXTENDIDA -> "<< start_extendida << endl;
        //cout << " FINISH EXTENDIDA -> "<< finish_extendida << endl;
        if(!(ExisteExtendida)){
            cout << "!ERROR, NO EXISTE ALGUNA PARTICION EXTENDIDA!" <<endl;
            return;
        }

        //LIMPIAMOS LOS VECTORES EBRS
        structs::EBR ebr_aux[50];
        structs::EBR tmp_EBRS[50];
        for(int i = 0; i < 50; i++){
            tmp_EBRS[i] = ebr_aux[i];
        }

        bool ExisteSiguiente = true;
        int contador = 0;
        int siguiente = start_extendida;
        structs::EBR aux_ebr;

        //RECUPERAMOS LOS EBRS DEL DISCO
        while(ExisteSiguiente){
            FILE *archivo = fopen(ruta.c_str(), "rb+");
            fseek(archivo, siguiente , SEEK_SET);
            fread(&aux_ebr, sizeof(structs::EBR), 1, archivo);
            fclose(archivo);

            tmp_EBRS[contador] = aux_ebr;
            //cout << "|status "<<tmp_EBRS[contador].status << " - inicio " << tmp_EBRS[contador].part_start << " - siguiente " << tmp_EBRS[contador].part_next  << " - name "<< tmp_EBRS[contador].name <<endl;
            if(siguiente <= 0 && tmp_EBRS[contador].status != '3'){
                ExisteSiguiente = false;
                contador--;
                break;
            }
            siguiente = tmp_EBRS[contador].part_next;
            contador++;
        }

        //ELIMINAMOS LAS COMILLAS DEL NOMBRE
        bool ExisteElNombre = false;
        string Rename;
        if(disco->name[0] == '"'){
            for(int i = 1; i < disco->name.size()-1; i++){ //ELIMINAR COMILLAS
                Rename+= disco->name[i];
            }
        }else{
            Rename = disco->name;
        }

        //LIMPAMOS LOS EBRS QUE SE VIENEN COLADOS
        structs::EBR vector_EBRS[50];
        structs::EBR ebr;
        bool ExisteEBR = false;
        int contador_EBR = 0;
        for(int i = 0; i < 50; i++){
            if((tmp_EBRS[i].status == '1') || (tmp_EBRS[i].status == '0')){
                vector_EBRS[contador_EBR] = tmp_EBRS[i];
                //cout << "status "<<tmp_EBRS[i].status << " - inicio " << tmp_EBRS[i].part_start << " - siguiente " << tmp_EBRS[i].part_next  << " - name "<< tmp_EBRS[i].name <<endl;
                contador_EBR++;
                if(tmp_EBRS[i].name == Rename){
                    cout << "ERROR: Ya existe una particion logica con el nombre de "<< Rename << "!"<< endl;
                    return;
                }
                if(tmp_EBRS[i].status == '0'){
                    ExisteEBR = true;
                    break;
                }
            }
        }



        //cout << "NAME"<< Rename << endl;


        //ESCRIBIR EL START Y EL NEXT DEPENDIENDO SI ES LA PRIMERA PARTICION LOGICA O NO
        if(ExisteEBR){
            ebr.part_start = start_extendida + sizeof (ebr);
            ebr.part_next = ebr.part_start + size ;
        }
        else{
            //ebr.part_start = tmp_EBRS[contador-1].part_next + sizeof(ebr);
            //ebr.part_next = ebr.part_start + size;

            //-----------------------------------------------------------------------------
            //---------BUSCAMOS ESPACIOS ENTRE LAS PARTICIONES LOGICAS----------------------
            //-----------------------------------------------------------------------------
            int espacios_size[contador_EBR+1];
            int espacios_start[contador_EBR+1];

            //cout << "*contador "<< contador_EBR << endl;
            espacios_size[0] = vector_EBRS[0].part_start - start_extendida -sizeof(vector_EBRS[0]);
            espacios_start[0] = start_extendida;

            //cout<< "Espacio -> "<< espacios_size[0] << " || Inicio -> "<< espacios_start[0] << endl;
            for(int i = 1; i <(contador_EBR); i++){
                espacios_size[i] = vector_EBRS[i].part_start -  (vector_EBRS[i-1].part_start + vector_EBRS[i-1].size) -sizeof(vector_EBRS[i-1]);
                espacios_start[i] = vector_EBRS[i-1].part_start +vector_EBRS[i-1].size;
                //cout<< "Espacio -> "<< espacios_size[i] << " || Inicio -> "<< espacios_start[i] << endl;
            }
            espacios_size[contador_EBR] = finish_extendida - (vector_EBRS[contador_EBR-1].part_start + vector_EBRS[contador_EBR-1].size) -sizeof(contador_EBR-1);
            espacios_start[contador_EBR] = vector_EBRS[contador_EBR-1].part_start + vector_EBRS[contador_EBR-1].size;

            //cout<< "Espacio -> "<< espacios_size[contador_EBR] << " || Inicio -> "<< espacios_start[contador_EBR] << endl;


            //BUSCAMOS EL FIT
            if(fit == "F"){
                bool NoHayEspacio = true;
                for(int i = 0; i < contador_EBR+1; i++){
                    if(espacios_start[i] == start_extendida && i == 0){
                        if((size+ sizeof(structs::EBR)) <= espacios_size[i]){
                            //cout<< "ENTRARE EN EL PRIMER LUGAR QUE ENCUENTRE -> comienzo extendida" << endl;
                            //cout << "*start -> "<< start_extendida + sizeof(structs::EBR) <<endl;
                            //cout << "*next -> "<< vector_EBRS[i].part_start - sizeof(structs::EBR)  <<endl;
                            ebr.part_start = start_extendida + sizeof(structs::EBR);
                            ebr.part_next = vector_EBRS[i].part_start - sizeof(structs::EBR) ;
                            NoHayEspacio = false;
                            break;
                        }
                    }
                    else if(i == contador_EBR && size+ sizeof(structs::EBR) <= espacios_size[i]){
                        //cout<< "ENTRARE EN EL PRIMER LUGAR QUE ENCUENTRE -> final extendida" << endl;
                        //cout << "*start -> "<< vector_EBRS[i-1].part_next + sizeof(structs::EBR) <<endl;
                        //cout << "*next -> "<< vector_EBRS[i-1].part_next + sizeof(structs::EBR)+size <<endl;
                        ebr.part_start = vector_EBRS[i-1].part_next + sizeof(structs::EBR);
                        ebr.part_next = vector_EBRS[i-1].part_next + sizeof(structs::EBR) + size ;
                        NoHayEspacio = false;
                        break;
                    }
                    else if(size+ sizeof(structs::EBR) <= espacios_size[i]){
                        //cout<< "ENTRARE EN EL PRIMER LUGAR QUE ENCUENTRE -> else" << endl;
                        //cout << "*start -> "<< espacios_start[i] <<endl;
                        //cout << "*next -> "<< vector_EBRS[i+1].part_start <<endl;
                        ebr.part_start =espacios_start[i] + sizeof(structs::EBR);
                        ebr.part_next = vector_EBRS[i].part_start - sizeof(structs::EBR);

                        vector_EBRS[i-1].part_next = ebr.part_start - sizeof(structs::EBR);
                        vector_EBRS[i].part_start = ebr.part_next;
                        //cout << " cambio -1 -> "<< vector_EBRS[i-1].part_next <<endl;
                        //cout << " cambio +1 -> "<< vector_EBRS[i].part_start <<endl;

                        FILE *bfile = fopen(ruta.c_str(), "rb+");
                        if (bfile != NULL)
                        {
                            //cout << "ESCRIBIR START -> "<< vector_EBRS[i-1].part_start -sizeof(ebr) << " --- "<<vector_EBRS[i-1].name << endl;
                            fseek(bfile,  vector_EBRS[i-1].part_start - sizeof(ebr), SEEK_SET);
                            fwrite(&vector_EBRS[i-1], sizeof(structs::EBR), 1, bfile);
                            //cout << "ESCRIBIR START -> "<< vector_EBRS[i].part_start -sizeof(ebr) << " --- "<<vector_EBRS[i].name <<  endl;
                            fseek(bfile,  vector_EBRS[i].part_start - sizeof(ebr), SEEK_SET);
                            fwrite(&vector_EBRS[i], sizeof(structs::EBR), 1, bfile);
                        }
                        fclose(bfile);
                        NoHayEspacio = false;
                        break;
                    }
                }
                if(NoHayEspacio){
                    cout<< "¡ERROR, NO HAY ESPACIO PARA LA PARTICION LOGICA! " << endl;
                    return;
                }

            }
            else{
                /*for(int i = 0; i < (contador_EBR+1); i++){
                    cout << "-start -> "<< espacios_start[i]<< " -size -> "<< espacios_size[i]<< endl;
                }*/
                //cout << "ESCRIBIR START -> "<< endl;
                int aux_starts = 0;
                int aux_sizes = 0;
                for(int i = 1; i < contador_EBR+1; i++){
                    for(int j = 0; j < (contador_EBR+1)-i ; j++){
                        if(espacios_size[j] > espacios_size[j+1] ){
                            //ORDENAMOS LOS TAMAÑOS
                            aux_sizes = espacios_size[j+1] ;
                            espacios_size[j+1]  = espacios_size[j] ;
                            espacios_size[j]  = aux_sizes;
                            //ORDENAMOS LOS STARTS
                            aux_starts = espacios_start[j+1];
                            espacios_start[j+1] = espacios_start[j];
                            espacios_start[j] = aux_starts;
                        }
                    }
                }

                //cout << "BURBUJA ORDENADO "<< endl;
                for(int i = 0; i < contador_EBR+1; i++){
                    //cout << "SIZE "<< espacios_size[i] << " - START "<< espacios_start[i] <<endl;
                }

                if(fit == "B"){
                    for(int i = 0; i < contador_EBR+1; i++){
                        //cout << "Me toca el esppacio -> "<< espacios_size[i] <<endl;
                        if(size+ sizeof(structs::EBR) <= espacios_size[i] && espacios_size[i] > 0){
                            if(espacios_start[i] == start_extendida){
                                //cout<< "ENTRARE EN EL PRIMER LUGAR QUE ENCUENTRE -> if" << endl;
                                //cout << "*start -> "<< espacios_start[i] + sizeof(structs::EBR) <<endl;
                                //cout << "*next -> "<< vector_EBRS[0].part_start - sizeof(structs::EBR) <<endl;
                                ebr.part_start = espacios_start[i] + sizeof(structs::EBR) ;
                                ebr.part_next = vector_EBRS[0].part_start - sizeof(structs::EBR);
                                break;
                            }
                            else{
                                bool EncontreEspacio = false;
                                for(int j = 0; j < contador_EBR; j++){
                                    //cout<< (espacios_start[i] + espacios_size[i] )<< " == "<< tmp_EBRS[j].part_next<<endl;
                                    if((espacios_start[i] + espacios_size[i] ) == tmp_EBRS[j].part_next){

                                        //cout<< "ENTRARE EN EL PRIMER LUGAR QUE ENCUENTRE -> else" << endl;
                                        //cout << "*start -> "<< espacios_start[i] + sizeof(structs::EBR) <<endl;
                                        //cout << "*next -> "<< vector_EBRS[j].part_next  <<endl;
                                        ebr.part_start = espacios_start[i] + sizeof(structs::EBR);
                                        ebr.part_next =  vector_EBRS[j].part_next;

                                        vector_EBRS[j].part_next = ebr.part_start -sizeof(structs::EBR);

                                        FILE *bfile = fopen(ruta.c_str(), "rb+");
                                        if (bfile != NULL)
                                        {
                                            //cout << "ESCRIBIR START -> "<< vector_EBRS[j].part_start -sizeof(ebr) << " --- "<<vector_EBRS[j].name << endl;
                                            fseek(bfile,  vector_EBRS[j].part_start - sizeof(ebr), SEEK_SET);
                                            fwrite(&vector_EBRS[j], sizeof(structs::EBR), 1, bfile);
                                        }
                                        fclose(bfile);
                                        EncontreEspacio = true;
                                        break;
                                    }
                                }

                                if(EncontreEspacio){
                                    break;
                                }

                                //cout<< (espacios_size[i])+ espacios_start[i]+ sizeof(structs::EBR) << " <= "<< (finish_extendida - vector_EBRS[contador_EBR-1].part_next) <<endl;
                                //cout << "  "<< (size+ sizeof(structs::EBR)) << endl;

                                if((size+ sizeof(structs::EBR)) <= (finish_extendida - vector_EBRS[contador_EBR-1].part_next)){
                                    //cout<< "ENTRARE EN EL PRIMER LUGAR QUE ENCUENTRE -> fin extendida" << endl;
                                    //cout << "*start -> "<< vector_EBRS[contador_EBR-1].part_next + sizeof(structs::EBR)  <<endl;
                                    //cout << "*next -> "<< vector_EBRS[contador_EBR-1].part_next + sizeof(structs::EBR) + size <<endl;
                                    ebr.part_start = vector_EBRS[contador_EBR-1].part_next + sizeof(structs::EBR) ;
                                    ebr.part_next =  vector_EBRS[contador_EBR-1].part_next + sizeof(structs::EBR) + size;
                                    break;
                                }

                                cout<< "NO HAY ESPACIO PARA LA PARTICION LOGICA " << endl;
                                return;

                            }
                            break;
                        }
                    }
                }
                else if(fit == "W"){
                    for(int i = contador_EBR; i >= 0 ; i--){
                        if(size+ sizeof(structs::EBR) <= espacios_size[i]){
                            if(espacios_start[i] == start_extendida){
                                //cout<< "ENTRARE EN EL PRIMER LUGAR QUE ENCUENTRE -> if" << endl;
                                //cout << "*start -> "<< espacios_start[i] + sizeof(structs::EBR) <<endl;
                                //cout << "*next -> "<< vector_EBRS[0].part_start - sizeof(structs::EBR) <<endl;
                                ebr.part_start = espacios_start[i] + sizeof(structs::EBR) ;
                                ebr.part_next = vector_EBRS[0].part_start - sizeof(structs::EBR);
                                break;
                            }
                            else{
                                bool EncontreEspacio = false;
                                for(int j = 0; j < contador_EBR; j++){
                                    //cout<< (espacios_start[i] + espacios_size[i] )<< " == "<< tmp_EBRS[j].part_next<<endl;
                                    if((espacios_start[i] + espacios_size[i] ) == tmp_EBRS[j].part_next){
                                        //cout<< "ENTRARE EN EL PRIMER LUGAR QUE ENCUENTRE -> else" << endl;
                                        //cout << "*start -> "<< espacios_start[i] + sizeof(structs::EBR) <<endl;
                                        //cout << "*next -> "<< vector_EBRS[j-1].part_next  <<endl;
                                        /*ebr.part_start = espacios_start[i] + sizeof(structs::EBR);
                                        ebr.part_next =  vector_EBRS[j-1].part_next;

                                        vector_EBRS[j-1].part_next = ebr.part_start -sizeof(structs::EBR);

                                        FILE *bfile = fopen(ruta.c_str(), "rb+");
                                        if (bfile != NULL)
                                        {
                                            //cout << "ESCRIBIR START -> "<< vector_EBRS[j-1].part_start -sizeof(ebr) << " --- "<<vector_EBRS[j-1].name << endl;
                                            fseek(bfile,  vector_EBRS[j-1].part_start - sizeof(ebr), SEEK_SET);
                                            fwrite(&vector_EBRS[j-1], sizeof(structs::EBR), 1, bfile);
                                        }
                                        fclose(bfile);
                                        EncontreEspacio = true;
                                        break;*/
                                        //cout<< "ENTRARE EN EL PRIMER LUGAR QUE ENCUENTRE -> else" << endl;
                                        //cout << "*start -> "<< espacios_start[i] + sizeof(structs::EBR) <<endl;
                                        //cout << "*next -> "<< vector_EBRS[j].part_next  <<endl;
                                        ebr.part_start = espacios_start[i] + sizeof(structs::EBR);
                                        ebr.part_next =  vector_EBRS[j].part_next;

                                        vector_EBRS[j].part_next = ebr.part_start -sizeof(structs::EBR);

                                        FILE *bfile = fopen(ruta.c_str(), "rb+");
                                        if (bfile != NULL)
                                        {
                                            //cout << "ESCRIBIR START -> "<< vector_EBRS[j].part_start -sizeof(ebr) << " --- "<<vector_EBRS[j].name << endl;
                                            fseek(bfile,  vector_EBRS[j].part_start - sizeof(ebr), SEEK_SET);
                                            fwrite(&vector_EBRS[j], sizeof(structs::EBR), 1, bfile);
                                        }
                                        fclose(bfile);
                                        EncontreEspacio = true;
                                        break;
                                    }
                                }

                                if(EncontreEspacio){
                                    break;
                                }

                                //cout<< (espacios_size[i])+ espacios_start[i]+ sizeof(structs::EBR) << " <= "<< (finish_extendida - vector_EBRS[contador_EBR-1].part_next) <<endl;
                                //cout << "  "<< (size+ sizeof(structs::EBR)) << endl;

                                if((size+ sizeof(structs::EBR)) <= (finish_extendida - vector_EBRS[contador_EBR-1].part_next)){
                                    ebr.part_start = vector_EBRS[contador_EBR-1].part_next + sizeof(structs::EBR) ;
                                    ebr.part_next =  vector_EBRS[contador_EBR-1].part_next + sizeof(structs::EBR) + size;
                                    break;
                                }

                                cout<< "NO HAY ESPACIO PARA LA PARTICION LOGICA " << endl;
                                return;

                            }
                            break;
                        }
                    }
                }


            }
        }
        //cout << "Start: "<< ebr.part_start << "  Next: "<< ebr.part_next <<endl;



        strcpy(ebr.name, Rename.c_str());
        ebr.fit = fit[0];
        ebr.size = size;
        ebr.status = '1';
        //--------------------------------------------------------------------




        //-------------------GUARDAMOS EL EBR EN EL DISCO-----------------
        FILE *bfile = fopen(ruta.c_str(), "rb+");
        if (bfile != NULL)
        {
            //cout << "ESCRIBIR START -> "<< ebr.part_start -sizeof(ebr) << endl;
            fseek(bfile, ebr.part_start - sizeof(ebr), SEEK_SET);
            fwrite(&ebr, sizeof(structs::EBR), 1, bfile);
        }
        fclose(bfile);
    }





    /*cout<<"INICIO DE ESPACIOsss--- \n" << endl;
    for(int i = 0; i < 4; i++){
        cout<<"INICIO DE ESPACIO -> "<< starts_fits[i] << "|| TAMAÑO DE ESPACIO -> "<< sizes_fits[i] << endl;
    }*/

    /*for(int i = 0; i < 4; i++){
        cout << "\n---- name " << tmp_disco.mbr_partitions[i].name << endl;
        cout << "-status " << tmp_disco.mbr_partitions[i].status << endl;
        cout << "-size " << tmp_disco.mbr_partitions[i].size << endl;
        cout << "-start " << tmp_disco.mbr_partitions[i].part_start<< endl;
        cout << "-fit " << tmp_disco.mbr_partitions[i].fit<< endl;
        cout << "-type " << tmp_disco.mbr_partitions[i].type<< endl;
        cout << "-------------------------------" << endl;
    }*/




}

void obfdisk::add(obfdisk *disco){

    int size_add = 0;

    if((disco->unit == "M") || (disco->unit == "m")){
        size_add = disco->add_size* 1024 * 1024;
    }
    else if((disco->unit == "K") || (disco->unit == "k")){
        size_add = disco->add_size * 1024;
    }
    else if((disco->unit == "B") || (disco->unit == "b")){
        size_add = disco->add_size;
    }
    else{
        size_add= disco->add_size * 1024;
        cout << "LA UNIDAD INGRESADA ES ERRONEA, SE TOMARA LA UNIDAD DE KILOBYTES"<<endl;
    }

    disco->add_size = 0;
    //------------- QUITAR LAS COMILLAS --------------------
    string cadenas = disco->path;
    string ruta;
    if(cadenas[0] == '"'){
        for(int i = 1; i < cadenas.size()-1; i++){ //ELIMINAR COMILLAS DE LA RUTA
            ruta += cadenas[i];
        }
    }else{
        ruta = cadenas;
    }
    disco->path = ruta;

    //-----------RECUPERAMOS DATOS DEL DISCO-------------------------
    structs::MBR tmp_disco;
    FILE *file = fopen(ruta.c_str(), "rb+");
    if (file != NULL){
        fseek(file, 0, SEEK_SET);
        fread(&tmp_disco, sizeof(tmp_disco), 1, file); //RECUPERAR LOS DATOS DEL DISCO
    }
    else{
        cout << "Error. Path no existente." << endl;
        return;
    }
    fclose(file);

    //QUITAR COMILLAS DEL NOMBRE
    string Rename;
    if(disco->name[0] == '"'){
        for(int i = 1; i < disco->name.size()-1; i++){ //ELIMINAR COMILLAS
            Rename+= disco->name[i];
        }
    }else{
        Rename = disco->name;
    }

    bool ExisteExtendida = false;
    bool ExisteParticion = false;
    int contador = 0;
    structs::partitions tmp_partittion;
    for(int i = 0; i < 4; i++){
        //cout << tmp_disco.mbr_partitions[i].name<< " == "<< Rename << endl;
        //- ES UNA PARTICION PRIMARIA O EXTENDIDA?
        if(tmp_disco.mbr_partitions[i].name == Rename){
            ExisteParticion = true;
            tmp_partittion = tmp_disco.mbr_partitions[i];
            contador = i;
            break;
        }
        //SI HAY EXTENDIDA SE VERIFICARAN LAS EBRS
        if(tmp_disco.mbr_partitions[i].type == 'E'){
            ExisteExtendida= true;
            tmp_partittion = tmp_disco.mbr_partitions[i];
        }
    }
    //cout << " contador "<< contador<<endl;

    //-------- VERIFICAR SI ES UNA PARTICION PRIMARIA O EXTENDIDA
    if(ExisteParticion){
        //cout << "PARTICIONES PRIMARIAS Y EXTENDIDAS"<<endl;


        //SE AGREGA ESPACIO
        if(disco->add_type){
            int new_size = tmp_partittion.part_start + tmp_partittion.size + size_add;
            //cout << "POSITIVO PARA PARTICIONES "<<endl;
            //ESTA ENTRE LAS PRIMERAS 3 PARTICIONES
            if(contador < 3){
                if(tmp_disco.mbr_partitions[contador+1].status == '0'){
                    //cout << "EL QUE ME SIGUE ES 0" << endl;
                    if(new_size  <= tmp_disco.mbr_size){
                        //cout << "SI CUMPLE! -> final extendida " << new_size <<endl;
                        tmp_partittion.size = tmp_partittion.size + size_add;
                    }
                    else{
                        cout << "ERROR, NO SE PUEDE EXPANDIR LA PARTICION"<< endl;
                        return;
                    }
                }
                else{
                   //cout << "Comparando si entra -> "<< new_size <<" <= "<<tmp_disco.mbr_partitions[contador+1].part_start<<endl;
                    if(new_size <= tmp_disco.mbr_partitions[contador+1].part_start){
                        //cout << "SI CUMPLE! -> entre " << new_size <<endl;
                        tmp_partittion.size = tmp_partittion.size + size_add;

                    }else{
                        cout << "ERROR, NO SE PUEDE EXPANDIR LA PARTICION"<< endl;
                        return;
                    }
                }

            }
            else{
                if(new_size  <= tmp_disco.mbr_size){
                    //cout << "SI CUMPLE! -> final extendida " << new_size <<endl;
                    tmp_partittion.size = tmp_partittion.size + size_add;
                }
                else{
                    cout << "ERROR, NO SE PUEDE EXPANDIR LA PARTICION"<< endl;
                    return;
                }
            }

        }
        else{
            int new_size = tmp_partittion.size - size_add;
            //cout << "NEGATIVO PARA PARTICIONES "<<endl;
            if(new_size >= 0){
                tmp_partittion.size = new_size;
            }
            else{
                cout << "ERROR, NO SE PUEDE REDUCIR LA PARTICION"<< endl;
                return;
            }
        }
        //cout << "namw -> "<< tmp_disco.mbr_partitions[contador].name << endl;
        tmp_disco.mbr_partitions[contador].size = tmp_partittion.size;
        //GUARDAMOS LA PARTICION SI TIENE EXITO EL PROCESO
        FILE *bfile = fopen(ruta.c_str(), "rb+");
        if (bfile != NULL)
        {
            fseek(bfile, 0, SEEK_SET);
            fwrite(&tmp_disco, sizeof(structs::MBR), 1, bfile);
        }
        fclose(bfile);
        cout << "SE AGREGO CORRECTAMENTE EL ESPACIO!"<< endl;
        return;

    }
    else if(ExisteExtendida){
        //----------------------------------------------------------------
        //---------------- VERIFICAMOS SI EXISTEN PARTICIONES LOGICAS ----
        //----------------------------------------------------------------

        //LIMPIAMOS LOS VECTORES EBRS
        structs::EBR ebr_aux[50];
        structs::EBR tmp_EBRS[50];
        for(int i = 0; i < 50; i++){
            tmp_EBRS[i] = ebr_aux[i];
        }

        bool ExisteSiguiente = true;
        int contador = 0;
        int siguiente = tmp_partittion.part_start;
        structs::EBR aux_ebr;

        //RECUPERAMOS LOS EBRS DEL DISCO
        while(ExisteSiguiente){
            FILE *archivo = fopen(ruta.c_str(), "rb+");
            fseek(archivo, siguiente , SEEK_SET);
            fread(&aux_ebr, sizeof(structs::EBR), 1, archivo);
            fclose(archivo);

            tmp_EBRS[contador] = aux_ebr;
            //cout << "|status "<<tmp_EBRS[contador].status << " - inicio " << tmp_EBRS[contador].part_start << " - siguiente " << tmp_EBRS[contador].part_next  << " - name "<< tmp_EBRS[contador].name <<endl;
            if(siguiente <= 0 && tmp_EBRS[contador].status != '3'){
                ExisteSiguiente = false;
                contador--;
                break;
            }
            siguiente = tmp_EBRS[contador].part_next;
            contador++;
        }


        int contador_EBR=0;
        structs::EBR vector_EBRS[50];
        for(int i = 0; i < 50; i++){
            if((tmp_EBRS[i].status == '1') || (tmp_EBRS[i].status == '0')){
                vector_EBRS[contador_EBR] = tmp_EBRS[i];
                //cout << "|status "<<vector_EBRS[contador_EBR].status << " - inicio " << vector_EBRS[contador_EBR].part_start << " - siguiente " << vector_EBRS[contador_EBR].part_next  << " - name "<< vector_EBRS[contador_EBR].name <<endl;
                contador_EBR++;
            }
        }

        //VERIFICANDO SI EXISTE EL NOMBRE
        int posicion = 0;
        bool ExisteParticion = true;
        for(int i = 0; i < contador_EBR; i++){
            //cout << vector_EBRS[i].name << " == " << Rename << endl;
            if(vector_EBRS[i].name == Rename){
                //cout<<"ENCONTRE LA PARTICION LOGICA "<< Rename << endl;
                ExisteParticion = false;
                posicion = i;
                break;
            }
        }

        if(ExisteParticion ){
            //cout << "ERROR, NO EXISTE LA PARTICION LOGICA " <<Rename<< endl;
            return;
        }

        structs::EBR tmp_EBR = vector_EBRS[posicion];
        if(disco->add_type){
            //cout << "POSITIVO PARA PARTICIONES LOGICAS "<<endl;
            int new_size = tmp_EBR.part_start + tmp_EBR.size + size_add;
            //VERIFICAMOS SI ES LA ULTIMA PARTICION LOGICA
            if(posicion == (contador_EBR-1)){
                //COMPARAMOS SI NO SOBREPASA EL TAMAÑO DE LA PARTICION EXTENDIDA
                if(new_size <= tmp_partittion.part_start + tmp_partittion.size){
                    tmp_EBR.size = tmp_EBR.size + size_add;
                }
                else{
                    cout << "ERROR, NO SE PUEDE EXPANDER LA PARTICION"<< endl;
                    return;
                }
            }
            else{
                //COMPARAMOS SI NO SOBREPASA EL TAMAÑO DE LA PARTICION LOGICA
                if(new_size <= tmp_EBR.part_next){
                    tmp_EBR.size = tmp_EBR.size + size_add;
                }
                else{
                    cout << "ERROR, NO SE PUEDE EXPANDER LA PARTICION"<< endl;
                    return;
                }
            }
        }
        else{
            //cout << "NEGATIVO PARA PARTICIONES LOGICAS"<<endl;
            int new_size = tmp_EBR.size - size_add;
            //cout << tmp_EBR.size << " - "<< size_add << " = "<< new_size <<endl;
            if(new_size >= 0){
                tmp_partittion.size = new_size;
            }
            else{
                //cout << "ERROR, NO SE PUEDE REDUCIR LA PARTICION"<< endl;
                return;
            }

        }
        FILE *bfile = fopen(ruta.c_str(), "rb+");
        if (bfile != NULL)
        {
            fseek(bfile, tmp_EBR.part_start- sizeof(structs::EBR), SEEK_SET);
            fwrite(&tmp_EBR, sizeof(structs::EBR), 1, bfile);
        }
        fclose(bfile);
        cout << "SE AGREGO CORRECTAMENTE EL ESPACIO!"<< endl;
        return;
    }


}

void obfdisk::deleter(obfdisk *disco){

    structs::MBR tmp_disco;
    string cadenas = disco->path;
    string ruta;
    string Fast_Full = disco->delet;
    disco->delet = "";

    //----------------ELIMINANDO COMILLAS--------------
    if(cadenas[0] == '"'){
        for(int i = 1; i < cadenas.size()-1; i++){ //ELIMINAR COMILLAS DE LA RUTA
            ruta += cadenas[i];
        }
    }else{
        ruta = cadenas;
    }




    //-----------RECUPERANDO EL MBR ---------------
    FILE *file = fopen(ruta.c_str(), "rb+");
    if (file != NULL){
        fseek(file, 0, SEEK_SET);
        fread(&tmp_disco, sizeof(tmp_disco), 1, file); //RECUPERAR LOS DATOS DEL DISCO
    }
    else{
        cout << "Error. Path no existente." << endl;
        return;
    }
    fclose(file);

    //--------ELIMINANDO COMILLAS DEL NOMBRE DE ENTRADA ----------------
    bool ExisteElNombre = false;
    string Rename;
    if(disco->name[0] == '"'){
        for(int i = 1; i < disco->name.size()-1; i++){ //ELIMINAR COMILLAS
            Rename+= disco->name[i];
        }
    }else{
        Rename = disco->name;
    }
    int numero;
    int posicion_Extendida = 0;
    bool ExisteExtendida = false;

    //----------- VERIFICAR SI SE ENCUENTRA EL NOMBRE ENTRE LAS PARTICIONES DEL MBR ------------
    structs::partition tmp_partittion;
    for(int i = 0; i < 4; i++){
        //cout << tmp_disco.mbr_partitions[i].name << "  " << disco->name<< endl;
        if(tmp_disco.mbr_partitions[i].type == 'E'){
            posicion_Extendida = i;
            ExisteExtendida = true;
        }
        if(tmp_disco.mbr_partitions[i].name == Rename){
            numero = i;
            ExisteElNombre = true;
            break;
        }
    }


    //-----ENVIANDO INFORMACION SOBRE EL EXITO O FALLO DE ENCONTRAR LA PARTICION

    if(ExisteElNombre){
        string answer;
        cout << "Esta seguro de ELIMINAR la particion " << Rename << "? [s/N]" << endl;
        cout << "Respuesta -> ";
        cin >>  answer;
        if((answer == "S") || answer == "s"){

            tmp_disco.mbr_partitions[numero] = tmp_partittion;

            //----------------------ORDENAMIENTO DE BURBUJA------------------
            structs::partition tmp_partition;
            for(int i = 1; i < 4; i++){
                for(int j = 0; j < 4 - i; j++){
                    if((tmp_disco.mbr_partitions[j].part_start > tmp_disco.mbr_partitions[j + 1].part_start)){
                        tmp_partition = tmp_disco.mbr_partitions[j+1];
                        tmp_disco.mbr_partitions[j+1] = tmp_disco.mbr_partitions[j];
                        tmp_disco.mbr_partitions[j] = tmp_partition;
                    }
                }
            }
            //LLENAR UNA LISTA AUXILIAR CON LAS PARTICIONES ACTIVAS
            int contador = 0;
            structs::partition tmp_aux[4];
            for(int i = 0; i< 4 ; i++){
                if(tmp_disco.mbr_partitions[i].status == '1'){
                    tmp_aux[contador] = tmp_disco.mbr_partitions[i];
                    contador++;
                }
            }
            //REMPLAZAR LAS PARTICIONES EN EL MBR
            for(int i = 0; i< 4 ; i++){
                   tmp_disco.mbr_partitions[i] = tmp_aux[i];
            }




            // ----------------------- GUARDAMOS LA PARTICION SI TUBO EXITO TODO EL PROCEDIMIENTO------------
            FILE *bfile = fopen(ruta.c_str(), "rb+");
            if (bfile != NULL)
            {
                fseek(bfile, 0, SEEK_SET);
                fwrite(&tmp_disco, sizeof(structs::MBR), 1, bfile);
            }
            fclose(bfile);


        }
        else if((answer == "N") || answer == "n"){
            return;
        }
        else{
            cout << "¡Error, Comando invalido! \n" << endl;
            return;
        }
    }
    else if(ExisteExtendida){
        //-----------------------------------------------------
        //BUSCAMOS SI EXISTE UNA PARTICION LOGICA CON EL NOMBRE

        structs::EBR ebr_aux[50];
        structs::EBR tmp_EBRS[50];
        structs::EBR tmp_EBRS1[50];
        for(int i = 0; i < 50; i++){
            tmp_EBRS[i] = ebr_aux[i];
            tmp_EBRS1[i] = ebr_aux[i];
        }

        bool ExisteSiguiente = true;
        int contador = 0;
        int siguiente = tmp_disco.mbr_partitions[posicion_Extendida].part_start;

        structs::EBR aux_ebr;

        while(ExisteSiguiente){
            FILE *archivo = fopen(ruta.c_str(), "rb+");
            fseek(archivo, siguiente , SEEK_SET);
            fread(&aux_ebr, sizeof(structs::EBR), 1, archivo);
            fclose(archivo);

            tmp_EBRS1[contador] = aux_ebr;
            //cout << "status "<<tmp_EBRS1[contador].status << " - inicio " << tmp_EBRS1[contador].part_start << " - siguiente " << tmp_EBRS1[contador].part_next  << " - name "<< tmp_EBRS1[contador].name <<endl;
            if(siguiente <= 0){
                ExisteSiguiente = false;
                contador--;
                break;
            }

            siguiente = tmp_EBRS1[contador].part_next;
            contador++;
        }

        int contador_EBR = 0;
        for(int i = 0; i < 50; i++){
            if((tmp_EBRS1[i].status == '1') || (tmp_EBRS1[i].status == '0')){
                tmp_EBRS[contador_EBR] = tmp_EBRS1[i];
                contador_EBR++;
            }
        }

        //cout << "*contador -> "<< contador<<endl;
        //-------- BUSCAMOS LA PARTICION EN TODA LA PARTICION EXTENDIDA ------------
        for(int i=0; i< 50; i++){
            if(tmp_EBRS[i].name == Rename){
                cout<< "EXISTE UNA PARTICION LOGICA CON EL NOMBRE DE "<<Rename<<endl;

                string answer;
                cout << "Esta seguro de ELIMINAR la particion " << Rename << "? [s/N] " << endl;
                cout << "Respuesta -> ";
                cin >> answer;
                if((answer == "S") || answer == "s"){
                    structs::EBR limpiar;
                    int posicion_eliminar = tmp_EBRS[i].part_start - sizeof(tmp_EBRS[i]);
                    limpiar.part_next = tmp_EBRS[i].part_next;
                    if(i == 0){
                        tmp_EBRS[i].part_next = tmp_EBRS[i + 1].part_start - sizeof(tmp_EBRS[i+1]);
                        FILE *bfile = fopen(ruta.c_str(), "rb+");
                        if (bfile != NULL)
                        {
                            fseek(bfile,  tmp_EBRS[i].part_start -sizeof(tmp_EBRS[i]), SEEK_SET);
                            fwrite(&tmp_EBRS[i] , sizeof(structs::EBR), 1, bfile);

                        }
                        fclose(bfile);
                    }
                    else if(i > 0 && i < contador-1){
                        tmp_EBRS[i-1].part_next = tmp_EBRS[i+1].part_start - sizeof(tmp_EBRS[i+1]);
                        FILE *bfile = fopen(ruta.c_str(), "rb+");
                        if (bfile != NULL)
                        {
                            fseek(bfile,  tmp_EBRS[i-1].part_start -sizeof(tmp_EBRS[i-1]), SEEK_SET);
                            fwrite(&tmp_EBRS[i-1] , sizeof(structs::EBR), 1, bfile);

                        }
                        fclose(bfile);
                    }

                    //BORRAMOS LA PARTICION
                    //limpiar.status = '3';
                    tmp_EBRS[i] = limpiar;

                    //-------------------GUARDAMOS EL EBR EN EL DISCO-----------------
                    FILE *bfile = fopen(ruta.c_str(), "rb+");
                    if (bfile != NULL)
                    {
                        //cout << "ESCRIBIR START -> "<< ebr.part_start -sizeof(ebr) << endl;
                        //cout << "ME ESCRIBO EN "<<posicion_eliminar<<endl;
                        fseek(bfile,  posicion_eliminar, SEEK_SET);
                        fwrite(&tmp_EBRS[i] , sizeof(structs::EBR), 1, bfile);
                    }
                    fclose(bfile);
                    cout << "  Se ELIMINO correctamente la particion"<< endl;
                    return;
                }
                else if((answer == "N") || answer == "n"){
                    return;
                }
                break;
            }
        }

        cout << "¡Error, El nombre de la particion no existe!" << endl;
        return;

    }
    else{
        cout << "¡Error, El nombre de la particion no existe!" << endl;
        return;
    }

}





