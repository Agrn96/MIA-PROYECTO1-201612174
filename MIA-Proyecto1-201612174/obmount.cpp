#include "obmount.h"
#include "iostream"
#include "structs.h"
#define STRING(num) #num
using namespace std;

extern structs::DiscosMontados Discos_Montados[99]; // 26 letras del alfabeto

obmount::obmount()
{

}

void obmount::mostrardatos(obmount *disco){


    cout<< "\n---------- MONTAJE DE DISCO -------------"<<endl;

    if(disco->name == "" || disco->name == ""){
        cout<<"FALTAN COMANDOS OBLIGATORIOS, NAME O PATH"<<endl;
        return;
    }

    string Nombre = EliminarComillas(disco->name);
    string Ruta = EliminarComillas(disco->path);

    if(!ExisteMBR(Ruta)){
        cout<<"NO EXISTE LA PARTICION"<<endl;
        return;
    }

    structs::MBR tmp_MBR = RecuperarMBR(Ruta);

    bool YaEstaMontada = false;
    for(int i = 0; i < 99;i++){
        for(int j = 0; j < 26; j++){
            if(Discos_Montados[i].partittion[j].nombre == Nombre && Discos_Montados[i].path == Ruta){
                YaEstaMontada = true;
            }
        }
    }

    if(YaEstaMontada){
        cout << "ERROR, LA PARTICION '"<< Nombre <<"' YA ESTA MONTADA" <<endl;
        return;
    }

    if(ExisteNombre(tmp_MBR, Nombre)){
        structs::partitions tmp_partittion = RecuperarParticion(tmp_MBR, Nombre);
        //cout << "Encontre la particion " << tmp_partittion.name <<endl;

        bool ExisteEspacioDisco = true;
        bool ExisteEspacioParticion = true;


        for(int i = 0; i < 99; i++){
            //cout <<Discos_Montados[i].path << " == " <<  Ruta<<endl;
            //Extract the disk name from the route
            if(Discos_Montados[i].path == Ruta){
                ExisteEspacioDisco = false;
                //cout << "ENTRE AL IF"<< endl;
                for(int j = 0; j < 26; j++){
                    if(Discos_Montados[i].partittion[j].status == 0){
                        Discos_Montados[i].partittion[j].status = 1;
                        Discos_Montados[i].partittion[j].nombre = Nombre;
                        Discos_Montados[i].partittion[j].letter = "Disco1"; //Change to the Disk name in the route

                        ExisteEspacioParticion = false;
                        break;
                    }
                }
                break;
            }

            else if(Discos_Montados[i].status == 0 ){
               //cout << "ENTRE AL else"<< endl;
               Discos_Montados[i].status = 1;
               Discos_Montados[i].path = Ruta;
               if(i < 10){
                 Discos_Montados[i].number = (i+641)+100;
               }
               else{
                  Discos_Montados[i].number = (i+641)+1000;
               }

               for(int j = 0; j < 26; j++){
                   if(Discos_Montados[i].partittion[j].status == 0){
                       Discos_Montados[i].partittion[j].status = 1;
                       Discos_Montados[i].partittion[j].nombre = Nombre;
                       Discos_Montados[i].partittion[j].letter = "Disco1"; //Change to the Disk name in the route

                       ExisteEspacioParticion = false;
                       break;
                   }
               }

               ExisteEspacioDisco = false;
               break;
            }
           //cout << Discos_Montados[i].status << " == "<< 0 <<endl;

        }

        if(ExisteEspacioDisco){
            cout << "NO EXISTE EL DISCO" << endl;
        }
        else if(ExisteEspacioParticion){
            cout << "NO EXISTE ESPACIO PARA LA PARTICION "<< Nombre << endl;
        }
        else{
            cout << "MONTAJE DE LA PARTICION "<< Nombre << " CORRECTAMENTE"<<endl;
            cout << "--------------------------------------------------------"<< endl;
        }

    }
    else{
        int n = ExisteExtendida(tmp_MBR);
        cout << "n -> "<<n<<endl;
        //VERIFICAR SI EXISTE EXTENDIDA
        if((n == 0)){
            cout << "NO EXISTE UNA PARTICION CON EL NOMBRE: "<< Nombre << endl;
            return;
        }
        else if(ExisteNombreLogica(Ruta, Nombre, n)){

            structs::EBR tmp_EBR = RecuperarParticionLogica(Ruta, Nombre, n);
            //cout << "Encontre la particion logica" << tmp_EBR.name <<endl;
            bool ExisteEspacioDisco = true;
            bool ExisteEspacioParticion = true;
            for(int i = 0; i < 99; i++){
                //cout <<Discos_Montados[i].path << " == " <<  Ruta<<endl;
                if(Discos_Montados[i].path == Ruta){
                    ExisteEspacioDisco = false;
                    //cout << "ENTRE AL if"<< endl;
                    for(int j = 0; j < 99; j++){
                        if(Discos_Montados[i].partittion[j].status == 0){
                            Discos_Montados[i].partittion[j].status = 1;
                            Discos_Montados[i].partittion[j].nombre = Nombre;
                            Discos_Montados[i].partittion[j].letter = j+65;

                            ExisteEspacioParticion = false;
                            break;
                        }
                    }
                    break;
                }
                else if( Discos_Montados[i].status == 0 ){
                   //cout << "ENTRE AL else"<< endl;
                   Discos_Montados[i].status = 1;
                   Discos_Montados[i].path = Ruta;

                   if(i < 10){
                     Discos_Montados[i].number = (i+641)+100;
                   }
                   else{
                      Discos_Montados[i].number = (i+641)+1000;
                   }

                   for(int j = 0; j < 99; j++){
                       if(Discos_Montados[i].partittion[j].status == 0){
                           Discos_Montados[i].partittion[j].status = 1;
                           Discos_Montados[i].partittion[j].nombre = Nombre;
                           Discos_Montados[i].partittion[j].letter = j+65;
                           ExisteEspacioParticion = false;
                           break;
                       }
                   }

                   ExisteEspacioDisco = false;
                   break;
                }

            }

            if(ExisteEspacioDisco){
                cout << "NO EXISTE EL DISCO" << endl;
            }
            else if(ExisteEspacioParticion){
                cout << "NO EXISTE ESPACIO PARA LA PARTICION "<< Nombre << endl;
            }
            else{
                cout << "MONTAJE DE LA PARTICION "<< Nombre << " CORRECTAMENTE"<<endl;
                cout << "--------------------------------------------------------"<< endl;
            }
        }
        else{
            cout << "NO EXISTE UNA PARTICION CON EL NOMBRE: "<< Nombre << endl;
            return;
        }

    }


    for(int i = 0; i < 99; i++){
        if(Discos_Montados[i].status == 1){
            cout<<"\n          ----- Disco "<< Discos_Montados[i].number << " -----"<<endl;
            for(int j = 0; j < 26; j++){
                if(Discos_Montados[i].partittion[j].status == 1){
                    cout<< Discos_Montados[i].path << "|" << Discos_Montados[i].partittion[j].nombre << "|"<< Discos_Montados[i].number << Discos_Montados[i].partittion[j].letter << endl;
                }
            }
        }

    }




    return;
}

structs::MBR obmount::RecuperarMBR(string ruta){
    //-----------RECUPERAMOS DATOS DEL DISCO-------------------------
    structs::MBR tmp_disco;
    FILE *file = fopen(ruta.c_str(), "rb+");
    fseek(file, 0, SEEK_SET);
    fread(&tmp_disco, sizeof(tmp_disco), 1, file); //RECUPERAR LOS DATOS DEL DISCO
    fclose(file);
    return tmp_disco;
}

bool obmount::ExisteMBR(string ruta){
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
    return true;
}

string obmount::EliminarComillas(string cadena){
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


bool obmount::ExisteNombre(structs::MBR tmp_MBR, string name){
    for(int i = 0; i<4; i++){
        //cout << "PARTICION --> "<<tmp_MBR.mbr_partitions[i].name<< " == "<< name <<endl;
        if(tmp_MBR.mbr_partitions[i].name == name){
            //cout << "ENCONTRE -> "<< tmp_MBR.mbr_partitions[i].name << endl;
            return true;
        }
    }
    return false;
}


int obmount::ExisteExtendida(structs::MBR tmp_MBR){
    for(int i = 0; i<4; i++){
        if(tmp_MBR.mbr_partitions[i].type == 'E'){
            return tmp_MBR.mbr_partitions[i].part_start;
        }
    }
    return 0;
}


structs::partitions obmount::RecuperarParticion(structs::MBR tmp_MBR, string name){
    for(int i = 0; i<4; i++){
        if(tmp_MBR.mbr_partitions[i].name == name){
            return tmp_MBR.mbr_partitions[i];
        }
    }
}

bool obmount::ExisteNombreLogica(string ruta, string name, int part_start_ext){

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
    //BUSCAMOS SI EXISTE EL NOMBRE
    for(int i = 0; i < 50; i++){
        if((tmp_EBRS[i].status == '1') || (tmp_EBRS[i].status == '0')){
            if(tmp_EBRS[i].name == name){
                return true;
            }
        }
    }
    return false;
}


structs::EBR obmount::RecuperarParticionLogica(string ruta, string name, int part_start_ext){

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
    //BUSCAMOS SI EXISTE EL NOMBRE
    for(int i = 0; i < 50; i++){
        if((tmp_EBRS[i].status == '1') || (tmp_EBRS[i].status == '0')){
            if(tmp_EBRS[i].name == name){
                return tmp_EBRS[i];
            }
        }
    }
}


void obmount::DesmontarMount(obmount *disco){
    string codigo = EliminarComillas(disco->id);
    bool Existe = true;
    for(int i = 0; i < 99;i++){
        if(Discos_Montados[i].status == 1){
            for(int j = 0; j < 26; j++){
                if(Discos_Montados[i].partittion[j].status == 1){
                    string id_aux(std::to_string(Discos_Montados[i].number));
                    id_aux += Discos_Montados[i].partittion[j].letter;
                    //cout << id_aux << " == " << codigo<<endl;
                    if(id_aux == codigo ){
                        structs::ParticionMontada eliminar;
                        Discos_Montados[i].partittion[j] = eliminar;
                        Existe = false;
                        break;
                    }
                }
            }
        }
    }

    if(Existe){
        cout<<"NO SE ENCONTRO LA PARTICION CON EL ID: "<< id << endl;
    }
    else{
        cout<<"SE DESMONTO CORRECTAMENTE LA PARTICION"<<endl;
    }
}

