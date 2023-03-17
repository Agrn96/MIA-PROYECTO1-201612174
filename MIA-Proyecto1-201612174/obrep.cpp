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

    if(Metodo == "MBR"){
        CrearTablaMBR(Ruta);
    }
    else if(Metodo == "DISK"){
        CrearDisk(Ruta);
    }
    else if(Metodo == "FILE"){
        CrearFILE(Ruta, Start_Part, this->ruta_file);
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
