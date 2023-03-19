#include "obexec.h"
#include "fstream"
#include <parser.h>  // Nuestro parser
#include <scanner.h>
#include <structs.h>// Nuestro scanner
extern int yyparse(); //
extern int linea; // Linea del token
extern int columna; // Columna de los tokens
extern int yylineno;

#include "fstream"
using namespace std;
obexec::obexec()
{

}

void obexec::exec() {
    if (this->path.empty()){
        cout << "ERROR el  parametro path es obligatorio." << endl;
        return;
    }

    FILE *file = NULL;
    file = fopen(this->path.c_str(), "r");
    if(file == NULL){
        cout << "ERROR La ruta del archivo es incorrecta o no existe." << endl;
        fclose(file);
        return;
    }
    fclose(file);

    ifstream script(this->path);
    string str;
    while (getline(script, str)) {
        std::cout << str << endl;
        if (str != "exit") {
            if (str != "") {
                YY_BUFFER_STATE buffer = yy_scan_string(str.c_str());

                if (yyparse() == 0) {
                    printf("\nComando ejecutado correctamente\n\n");
                } else {
                    printf("\nNo se pudo ejecutar el comando\n\n");
                }
                printf("\n********************************************************************************************\n");
                //yy_delete_buffer(buffer);
            }
        }
    }
}
