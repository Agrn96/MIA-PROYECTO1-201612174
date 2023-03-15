#include "obmkdisk.h"
#include "iostream"
#include "structs.h"
#include <time.h>
#include <ctime>
using namespace std;

obmkdisk::obmkdisk()
{

}


void obmkdisk::mostrardatos(obmkdisk *disco){

     cout << "\n     ----COMANDO MKDISK----\n"<<endl;

    int size;
    string path;

    char tmp[16];


    if((disco->unit == "K") || (disco->unit == "k")){
        cout<<"La unidad son kilobytes"<<endl;
        size = disco->size*1024;
    }
    else if((disco->unit == "M") || (disco->unit == "m")){
        size = disco->size*1024*1024;
    }else{

        size = disco->size*1024*1024;
    }

    if(disco->fit == ""){
        disco->fit = "BF";
    }

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

    //cout << "Prueba: " << ruta <<endl;



    FILE *afile = fopen(ruta.c_str(), "r");
    if (afile != NULL){
        cout<<"El disco ya existe"<<endl;
        fclose(afile);
        return;
    }

    structs::MBR nuevo_MBR;
    nuevo_MBR.mbr_size = size;
    nuevo_MBR.mbr_disk_signature = rand() % 100;
    nuevo_MBR.mbr_fit = toupper(disco->fit[0]);


    nuevo_MBR.mbr_date = time(0);
    strftime(tmp, 20, "%d/%m/%Y %H:%M", localtime(&nuevo_MBR.mbr_date));



    FILE *file = fopen(ruta.c_str(), "wb");
    if (file != NULL)
    {
        fwrite("\0", 1, 1, file);
        fseek(file, size - 1, SEEK_SET);
        fwrite("\0", 1, 1, file);
        rewind(file);
        fwrite(&nuevo_MBR, sizeof(structs::MBR), 1, file);

        cout<<"\n/// Se creo el disco exitosamente ///"<<endl;

    }
    else{
        string cmd1 = "mkdir -p \"" + ruta + "\"";
        system(cmd1.c_str());
        string cmd2 = "rmdir \"" + ruta + "\"";
        system(cmd2.c_str());

        file = fopen(ruta.c_str(), "wb");
        fwrite("\0", 1, 1, file);
        fseek(file, size - 1, SEEK_SET);
        fwrite("\0", 1, 1, file);
        rewind(file);
        fwrite(&nuevo_MBR, sizeof(structs::MBR), 1, file);

        cout<<"\n/// Se creo el disco en carpetas exitosamente ///"<<endl;

    }
    fclose(file);


    printf("\n---------DATOS----------\n");
    cout<<"-SIZE = "<<disco->size<<endl;
    cout<<"-UNIT = "<<disco->unit<<endl;
    cout<<"-FIT  = "<<disco->fit<<endl;
    cout<<"-PATH = "<<disco->path<<endl;
    cout<<"-MBR SIZE = "<<sizeof(structs::MBR)<<endl;
    cout<<"-FECHA Y HORA = "<< tmp <<endl;



    /*char sc[ruta.size() + 1];
    strcpy(sc, ruta.c_str());
    FILE *file=NULL;
    file=fopen(sc,"r");
    if(file!=NULL){
        cout<<"Ya existe el disco"<<endl;
        return;
    }


    file=fopen(sc,"wb");
    fwrite("\0",1,1,file);
    fseek(file,size,SEEK_SET); // objeto file, size de cuanto se quiere mover, al inicio
    fwrite("\0",1,1,file);


    fclose(file);*/
}
