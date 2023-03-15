%{
#include "scanner.h"//se importa el header del analisis sintactico
#include <QString>
#include <string>
#include "qdebug.h"
#include <iostream>
#include "list"

#include "obmkdisk.h"
#include "obfdisk.h"
#include "obmount.h"
#include "obrm.h"
#include "obexec.h"

using namespace std;
extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON

int yyerror(const char* mens)
{
std::cout << mens <<" "<<yytext<< std::endl;
return 0;
}
%}
//error-verbose si se especifica la opcion los errores sintacticos son especificados por BISON
%defines "parser.h"
%output "parser.cpp"
%locations
%union{
//se especifican los tipo de valores para los no terminales y lo terminales
//char TEXT [256];
//QString TEXT;
char TEXT[256];

class obmkdisk *mdisk;
class obmrdisk *mrdisk;
class obfdisk *fdisk;
class obmount *montar;
class obmkfs *mkfs;
class obtouch *touch;
class obrep *rep;
class obmkdir *mkdir;
class obcat *cat;
class obren *ren;
class obrm *rm;
class obedit *edit;
class obexec *exec;
class obcp *copy;
class obmv *mv;
class obfind *find;
class oblogin *login;
class obmkgrp *mkgrp;
class obrmgrp *rmgrp;
class obmkusr *mkusr;
class obrmusr *rmusr;
class obchgrp *chgrp;
class obrecovery *recovery;

}
//TERMINALES DE TIPO TEXT, SON STRINGS


%token<TEXT> psize;
%token<TEXT> pmkdisk;
%token<TEXT> pmrdisk;
%token<TEXT> ppath;
%token<TEXT> punit;
%token<TEXT> pfit;
%token<TEXT> pfdisk;
%token<TEXT> ptype;
%token<TEXT> pname;
%token<TEXT> pdelete;
%token<TEXT> padd;
%token<TEXT> pmount;
%token<TEXT> pumount;
%token<TEXT> pid;
%token<TEXT> pmkfs;
%token<TEXT> pfs;
%token<TEXT> p2fs;
%token<TEXT> p3fs;

%token<TEXT> punto;
%token<TEXT> bracketabre;
%token<TEXT> bracketcierra;
%token<TEXT> corcheteabre;
%token<TEXT> corchetecierra;
%token<TEXT> puntocoma;
%token<TEXT> potencia;
%token<TEXT> coma;
%token<TEXT> parentesisabre;
%token<TEXT> parentesiscierra;

%token<TEXT> llaveabre;
%token<TEXT> llavecierra;
%token<TEXT> mas;
%token<TEXT> menos;
%token<TEXT> multiplicacion;
%token<TEXT> igual;
%token<TEXT> dolar;
%token<TEXT> dospuntos;
%token<TEXT> interrogacion;

%token<TEXT> entero;
%token<TEXT> concero;
%token<TEXT> positivo
%token<TEXT> numnegativo;
%token<TEXT> cadena;
%token<TEXT> identificador;
%token<TEXT> caracter;
%token<TEXT> ruta;
%token<TEXT> rutacualquiera;
%token<TEXT> uunit;
%token<TEXT> ffit;
%token<TEXT> caracterunico
%token<TEXT> codigo
%token<TEXT> ptouch
%token<TEXT> pr
%token<TEXT> pcont
%token<TEXT> pstdin
%token<TEXT> prep
%token<TEXT> pmkdir
%token<TEXT> pcat
%token<TEXT> pren
%token<TEXT> proot
%token<TEXT> prm
%token<TEXT> pedit
%token<TEXT> pruta
%token<TEXT> pexec
%token<TEXT> pdest
%token<TEXT> pcp
%token<TEXT> pmv
%token<TEXT> pfind
%token<TEXT> ppause
%token<TEXT> plogin
%token<TEXT> puser
%token<TEXT> ppwd
%token<TEXT> plogout
%token<TEXT> prmgrp
%token<TEXT> pmkgrp
%token<TEXT> pmkusr
%token<TEXT> prmusr
%token<TEXT> pusr
%token<TEXT> pgrp
%token<TEXT> pchgrp
%token<TEXT> precovery
%token<TEXT> ploss

%type<mdisk> OPTIONMKDISK; // lista de instrucciones
%type<mrdisk> GMRDISK;
%type<fdisk> OPTIONFDISK;
%type<montar> OPTIONMOUNT;
%type<mkfs> OPTIONMKFS;
%type<touch> OPTIONTOUCH;
%type<rep> OPTIONREP;
%type<mkdir> OPTIONMKDIR
%type<cat> OPTIONCAT
%type<ren> OPTIONREN
%type<rm> GRM;
%type<edit> OPTIONEDIT
%type<exec> GEXEC;
%type<copy> OPTIONCP
%type<mv> OPTIONMV
%type<find> OPTIONFIND
%type<login> OPTIONLOGIN
%type<mkusr> OPTIONMKUSR
%type<rmusr> GRMUSR
%type<chgrp> OPTIONCHGRP

%left suma menos
%left multi division
%left potencia
%start INICIO
%%

INICIO:
    INSTRUCCIONES {}
;

INSTRUCCIONES:
      INSTRUCCIONES INSTRUCCION     {}
    | INSTRUCCION                   {}
;

INSTRUCCION :
          GMKDISK {   }
        | GMRDISK {   }
        | GFDISK  {   }
        | GMOUNT  {   }
        | GMKFS   {   }
        | GTOUCH  {   }
        | GREP    {   }
        | GMKDIR  {   }
        | GCAT    {   }
        | GCREN   {   }
        | GRM     {   }
        | GEDIT   {   }
        | GEXEC   {   }
        | GCP     {   }
        | GMV     {   }
        | GFIND   {   }
        | GPAUSE  {   }
        | GLOGIN  {   }
        | GLOGOUT {   }
        | GMKGRP  {   }
        | GRMGRP  {   }
        | GMKUSR  {   }
        | GRMUSR  {   }
        | GCHGRP  {   }
        | GRECOVERY {   }
;

GMKDISK:
    pmkdisk OPTIONMKDISK {$2->mostrardatos($2);}
;

OPTIONMKDISK:
    //bracketcierra psize igual entero { int tam=atoi($4); obmkdisk *disco=new obmkdisk(); disco->size=tam; $$=disco; }
      OPTIONMKDISK bracketcierra psize igual positivo  { int tam=atoi($5);  $1->size=tam; $$=$1; } //SIZE
    | OPTIONMKDISK bracketcierra psize igual entero  { int tam=atoi($5);  $1->size=tam; $$=$1; } //SIZE
    | OPTIONMKDISK bracketcierra ppath igual cadena  { $1->path=$5; $$=$1; } //PATH
    | OPTIONMKDISK bracketcierra ppath igual ruta  { $1->path=$5; $$=$1; } //PATH
    | OPTIONMKDISK bracketcierra punit igual uunit  { $1->unit=$5; $$=$1; } //UNIT
    | OPTIONMKDISK bracketcierra pfit igual identificador  { $1->fit=$5; $$=$1; } //FIT

    | bracketcierra psize igual positivo {obmkdisk *disco=new obmkdisk(); int tam=atoi($4); disco->size=tam; $$=disco; } //SIZE
    | bracketcierra psize igual entero {obmkdisk *disco=new obmkdisk(); int tam=atoi($4); disco->size=tam; $$=disco; } //SIZE
    | bracketcierra ppath igual cadena {obmkdisk *disco=new obmkdisk(); disco->path=$4; $$=disco; } //PATH
    | bracketcierra ppath igual ruta {obmkdisk *disco=new obmkdisk(); disco->path=$4; $$=disco; } //PATH
    | bracketcierra punit igual uunit  {obmkdisk *disco=new obmkdisk(); disco->unit=$4; $$=disco; }  //UNIT
    | bracketcierra pfit igual identificador    {obmkdisk *disco=new obmkdisk(); disco->fit=$4; $$=disco; }  //FIT
;

GMRDISK :
    pmrdisk bracketcierra ppath igual cadena {obmrdisk *disco=new obmrdisk(); disco->mostrardatos($5); }
;

GFDISK:
    pfdisk OPTIONFDISK { $2->mostrardatos($2); }
;

OPTIONFDISK:
      OPTIONFDISK bracketcierra psize igual entero { int tam=atoi($5); $1->size=tam;  $$=$1;  } //SIZE
    | OPTIONFDISK bracketcierra psize igual positivo { int tam=atoi($5); $1->size=tam;  $$=$1;  } //SIZE
    | OPTIONFDISK bracketcierra punit igual uunit { $1->unit=$5;  $$=$1; } //UNIT
    | OPTIONFDISK bracketcierra ppath igual cadena  { $1->path=$5;  $$=$1; } //PATH
    | OPTIONFDISK bracketcierra ppath igual ruta  { $1->path=$5;  $$=$1; } //PATH
    | OPTIONFDISK bracketcierra ptype igual identificador  { $1->type=$5;  $$=$1; } //TYPE
    | OPTIONFDISK bracketcierra pfit igual identificador  { $1->fit=$5;  $$=$1; } //FIT
    | OPTIONFDISK bracketcierra pname igual cadena  { $1->name=$5;   $$=$1;  } //NAME
    | OPTIONFDISK bracketcierra pname igual identificador  { $1->name=$5;  $$=$1; } //NAME
    | OPTIONFDISK bracketcierra pdelete igual identificador  { $1->delet=$5;  $$=$1; } //DELETE
    | OPTIONFDISK bracketcierra padd igual positivo  { int tam=atoi($5); $1->add_type=true; $1->add_size=tam; $$=$1; } //ADD
    | OPTIONFDISK bracketcierra padd igual entero  { int tam=atoi($5); $1->add_type=true; $1->add_size=tam; $$=$1; } //ADD
    | OPTIONFDISK bracketcierra padd igual bracketcierra positivo  { int tam=atoi($6); $1->add_type=false; $1->add_size=tam; $$=$1; } //ADD
    | OPTIONFDISK bracketcierra padd igual bracketcierra entero { int tam=atoi($6); $1->add_type=false; $1->add_size=tam; $$=$1; } //ADD

    | bracketcierra psize igual entero { int tam=atoi($4); obfdisk *disco=new obfdisk(); disco->size=tam;     $$=disco; } // SIZE
    | bracketcierra psize igual positivo { int tam=atoi($4); obfdisk *disco=new obfdisk(); disco->size=tam;      $$=disco; } // SIZE
    | bracketcierra punit igual uunit { obfdisk *disco=new obfdisk(); disco->unit=$4;      $$=disco;} //UNIT
    | bracketcierra ppath igual cadena  { obfdisk *disco=new obfdisk(); disco->path=$4;      $$=disco; } //PATH
    | bracketcierra ppath igual ruta  { obfdisk *disco=new obfdisk(); disco->path=$4;      $$=disco;  } //PATH
    | bracketcierra ptype igual identificador  { obfdisk *disco=new obfdisk(); disco->type=$4;      $$=disco; } //TYPE
    | bracketcierra pfit igual identificador  { obfdisk *disco=new obfdisk(); disco->fit=$4;     $$=disco; } //FIT
    | bracketcierra pname igual cadena  { obfdisk *disco=new obfdisk(); disco->name=$4;      $$=disco; } //NAME
    | bracketcierra pname igual identificador  { obfdisk *disco=new obfdisk(); disco->name=$4;      $$=disco; } //NAME
    | bracketcierra pdelete igual identificador  { obfdisk *disco=new obfdisk(); disco->delet=$4;      $$=disco; } //DELETE
    | bracketcierra padd igual positivo  { int tam=atoi($4); obfdisk *disco=new obfdisk(); disco->add_type=true; disco->add_size=tam; $$=disco; } //ADD
    | bracketcierra padd igual entero  { int tam=atoi($4); obfdisk *disco=new obfdisk(); disco->add_type=true; disco->add_size=tam; $$=disco; } //ADD
    | bracketcierra padd igual bracketcierra positivo  { int tam=atoi($5); obfdisk *disco=new obfdisk(); disco->add_type=false; disco->add_size=tam; $$=disco; } //ADD
    | bracketcierra padd igual bracketcierra entero  { int tam=atoi($5); obfdisk *disco=new obfdisk(); disco->add_type=false; disco->add_size=tam; $$=disco; } //ADD
;

GMOUNT:
    pmount OPTIONMOUNT { $2->mostrardatos($2);}
    | pumount bracketcierra pid igual codigo  { obmount *disco = new obmount(); disco->id=$5;  disco->DesmontarMount(disco); }
;

OPTIONMOUNT:
    OPTIONMOUNT bracketcierra ppath igual cadena { $1->path=$5; $$=$1; }
    | OPTIONMOUNT bracketcierra ppath igual ruta { $1->path=$5; $$=$1; }
    | OPTIONMOUNT bracketcierra pname igual identificador { $1->name=$5; $$=$1;  }
    | OPTIONMOUNT bracketcierra pname igual cadena { $1->name=$5; $$=$1;  }

    | bracketcierra ppath igual cadena { obmount *disco = new obmount(); disco->path=$4; $$=disco; }
    | bracketcierra ppath igual ruta { obmount *disco = new obmount(); disco->path=$4; $$=disco; }
    | bracketcierra pname igual identificador { obmount *disco = new obmount(); disco->name=$4; $$=disco;  }
    | bracketcierra pname igual cadena { obmount *disco = new obmount(); disco->name=$4; $$=disco;  }
;

GMKFS:
    pmkfs OPTIONMKFS { $2->mostrardatos($2); }
;


OPTIONMKFS:
    OPTIONMKFS bracketcierra pid igual codigo               { $1->id=$5; $$=$1; }
    | OPTIONMKFS bracketcierra ptype igual identificador    { $1->type=$5; $$=$1; }
    | OPTIONMKFS bracketcierra pfs igual p2fs           { $1->fs=$5; $$=$1; }
    | OPTIONMKFS bracketcierra pfs igual p3fs           { $1->fs=$5; $$=$1; }

    | bracketcierra pid igual codigo            { obmkfs *disco = new obmkfs(); disco->id=$4; $$=disco; }
    | bracketcierra ptype igual identificador   { obmkfs *disco = new obmkfs(); disco->type=$4; $$=disco; }
    | bracketcierra pfs igual p2fs         { obmkfs *disco = new obmkfs(); disco->fs=$4; $$=disco; }
    | bracketcierra pfs igual p3fs         { obmkfs *disco = new obmkfs(); disco->fs=$4; $$=disco; }
;

GTOUCH:
    ptouch OPTIONTOUCH  {$2->mostrardatos($2); }
;

OPTIONTOUCH:
    OPTIONTOUCH bracketcierra ppath igual cadena    { $1->path=$5; $$=$1; }
    | OPTIONTOUCH bracketcierra ppath igual ruta    { $1->path=$5; $$=$1; }
    | OPTIONTOUCH bracketcierra pr                  { $1->r=true; $$=$1; }
    | OPTIONTOUCH bracketcierra psize igual positivo{ int tam=atoi($5); $1->size=tam; $$=$1; }
    | OPTIONTOUCH bracketcierra psize igual entero  { int tam=atoi($5); $1->size=tam; $$=$1; }
    | OPTIONTOUCH bracketcierra pcont igual cadena  { $1->cont=$5; $$=$1; }
    | OPTIONTOUCH bracketcierra pcont igual ruta    { $1->cont=$5; $$=$1; }
    | OPTIONTOUCH bracketcierra pstdin              { $1->stdin=true; $$=$1;}

    | bracketcierra ppath igual cadena  { obtouch *disco = new obtouch();  disco->path=$4; $$=disco; }
    | bracketcierra ppath igual ruta    { obtouch *disco = new obtouch();  disco->path=$4; $$=disco; }
    | bracketcierra pr                  { obtouch *disco = new obtouch();  disco->r=true; $$=disco; }
    | bracketcierra psize igual positivo{ int tam=atoi($4); obtouch *disco = new obtouch();  disco->size=tam; $$=disco; }
    | bracketcierra psize igual entero  { int tam=atoi($4); obtouch *disco = new obtouch();  disco->size=tam; $$=disco; }
    | bracketcierra pcont igual cadena  { obtouch *disco = new obtouch();  disco->cont=$4; $$=disco; }
    | bracketcierra pcont igual ruta    { obtouch *disco = new obtouch();  disco->cont=$4; $$=disco; }
    | bracketcierra pstdin              { obtouch *disco = new obtouch();  disco->stdin=true; $$=disco; }
;

GREP:
    prep OPTIONREP  {$2->mostrardatos($2); }
;

OPTIONREP:
      OPTIONREP bracketcierra pid igual codigo     { $1->id=$5; $$=$1; }
    | OPTIONREP bracketcierra ppath igual cadena { $1->path=$5; $$=$1; }
    | OPTIONREP bracketcierra ppath igual ruta { $1->path=$5; $$=$1; }
    | OPTIONREP bracketcierra pname igual identificador { $1->name=$5; $$=$1;  }
    | OPTIONREP bracketcierra pname igual cadena {int tam=atoi($5); $1->name=$5; $$=$1;  }
    | OPTIONREP bracketcierra proot igual positivo { int tam=atoi($5); $1->root=tam; $$=$1;  }
    | OPTIONREP bracketcierra proot igual entero { int tam=atoi($5); $1->root=tam; $$=$1;  }
    | OPTIONREP bracketcierra pruta igual cadena { $1->ruta_file=$5; $$=$1; }
    | OPTIONREP bracketcierra pruta igual ruta { $1->ruta_file=$5; $$=$1; }

    | bracketcierra pid igual codigo     { obrep *disco = new obrep(); disco->id=$4; $$=disco; }
    | bracketcierra ppath igual cadena { obrep *disco = new obrep(); disco->path=$4; $$=disco; }
    | bracketcierra ppath igual ruta { obrep *disco = new obrep(); disco->path=$4; $$=disco; }
    | bracketcierra pname igual identificador { obrep *disco = new obrep(); disco->name=$4; $$=disco;  }
    | bracketcierra pname igual cadena { obrep *disco = new obrep(); disco->name=$4; $$=disco;  }
    | bracketcierra proot igual positivo { int tam=atoi($4); obrep *disco = new obrep(); disco->root=tam; $$=disco; }
    | bracketcierra proot igual entero { int tam=atoi($4); obrep *disco = new obrep(); disco->root=tam; $$=disco; }
    | bracketcierra pruta igual cadena { obrep *disco = new obrep(); disco->ruta_file=$4; $$=disco; }
    | bracketcierra pruta igual ruta { obrep *disco = new obrep(); disco->ruta_file=$4; $$=disco; }
;


GMKDIR:
      pmkdir OPTIONMKDIR  {$2->mostrardatos($2); }
;

OPTIONMKDIR:
    OPTIONMKDIR bracketcierra identificador    { $1->p_entrada=$3; $1->r=true;  $$=$1; }
  | OPTIONMKDIR bracketcierra ppath igual cadena { $1->path=$5; $$=$1; }
  | OPTIONMKDIR bracketcierra ppath igual ruta { $1->path=$5; $$=$1; }

  | bracketcierra identificador     { obmkdir *disco = new obmkdir(); disco->p_entrada=$2;  disco->r=true;  $$=disco; }
  | bracketcierra ppath igual cadena { obmkdir *disco = new obmkdir(); disco->path=$4; $$=disco; }
  | bracketcierra ppath igual ruta { obmkdir *disco = new obmkdir(); disco->path=$4; $$=disco; }
;

GCAT:
    pcat OPTIONCAT { $2->mostrardatos($2); }
;

OPTIONCAT:
      OPTIONCAT bracketcierra identificador igual cadena { $1->path.push_back($5); $$=$1; }
    | OPTIONCAT bracketcierra identificador igual ruta { $1->path.push_back($5); $$=$1; }

    | bracketcierra identificador igual cadena { obcat *disco = new obcat();  disco->path.clear(); disco->path.push_back($4); $$=disco; }
    | bracketcierra identificador igual ruta { obcat *disco = new obcat();  disco->path.clear(); disco->path.push_back($4); $$=disco; }
;

GCREN:
  pren OPTIONREN { $2->mostrardatos($2); }
;

OPTIONREN:
    OPTIONREN bracketcierra ppath igual cadena { $1->path=$5; $$=$1; }
  | OPTIONREN bracketcierra ppath igual ruta { $1->path=$5; $$=$1; }
  | OPTIONREN bracketcierra pname igual identificador { $1->name=$5; $$=$1;  }
  | OPTIONREN bracketcierra pname igual cadena { $1->name=$5; $$=$1;  }

  | bracketcierra ppath igual cadena { obren *disco = new obren(); disco->path=$4; $$=disco; }
  | bracketcierra ppath igual ruta { obren *disco = new obren(); disco->path=$4; $$=disco; }
  | bracketcierra pname igual identificador { obren *disco = new obren(); disco->name=$4; $$=disco;  }
  | bracketcierra pname igual cadena { obren *disco = new obren(); disco->name=$4; $$=disco;  }
;


GRM:
      prm bracketcierra ppath igual cadena { obrm *disco = new obrm(); disco->path=$5; disco->mostrardatos(disco);}
    | prm bracketcierra ppath igual ruta { obrm *disco = new obrm(); disco->path=$5; disco->mostrardatos(disco); }
;

GEDIT:
    pedit OPTIONEDIT { $2->mostrardatos($2); }
;

OPTIONEDIT:
      OPTIONEDIT bracketcierra ppath igual cadena    { $1->path=$5; $$=$1; }
    | OPTIONEDIT bracketcierra ppath igual ruta    { $1->path=$5; $$=$1; }
    | OPTIONEDIT bracketcierra pcont igual cadena  { $1->cont=$5; $$=$1; }
    | OPTIONEDIT bracketcierra pcont igual ruta    { $1->cont=$5; $$=$1; }
    | OPTIONEDIT bracketcierra pstdin              { $1->stdin=true; $$=$1;}

    | bracketcierra ppath igual cadena  { obedit *disco = new obedit();  disco->path=$4; $$=disco; }
    | bracketcierra ppath igual ruta    { obedit *disco = new obedit();  disco->path=$4; $$=disco; }
    | bracketcierra pcont igual cadena  { obedit *disco = new obedit();  disco->cont=$4; $$=disco; }
    | bracketcierra pcont igual ruta    { obedit *disco = new obedit();  disco->cont=$4; $$=disco; }
    | bracketcierra pstdin              { obedit *disco = new obedit();  disco->stdin=true; $$=disco; }
;

GEXEC:
    pexec bracketcierra ppath igual cadena  { obexec *disco = new obexec();  disco->path=$5; disco->exec(); }
  | pexec bracketcierra ppath igual ruta    { obexec *disco = new obexec();  disco->path=$5; disco->exec();  }
;


GCP:
    pcp OPTIONCP { $2->mostrardatos($2); }
;

OPTIONCP:
      OPTIONCP bracketcierra ppath igual cadena    { $1->path=$5; $$=$1; }
    | OPTIONCP bracketcierra ppath igual ruta    { $1->path=$5; $$=$1; }
    |  OPTIONCP bracketcierra pdest igual cadena    { $1->destino=$5; $$=$1; }
    | OPTIONCP bracketcierra pdest igual ruta    { $1->destino=$5; $$=$1; }

    | bracketcierra ppath igual cadena  { obcp *disco = new obcp();  disco->path=$4; $$=disco; }
    | bracketcierra ppath igual ruta    { obcp *disco = new obcp();  disco->path=$4; $$=disco; }
    | bracketcierra pdest igual cadena  { obcp *disco = new obcp();  disco->destino=$4; $$=disco; }
    | bracketcierra pdest igual ruta    { obcp *disco = new obcp();  disco->destino=$4; $$=disco; }
;


GMV:
  pmv OPTIONMV { $2->mostrardatos($2); }
;

OPTIONMV:
    OPTIONMV bracketcierra ppath igual cadena    { $1->path=$5; $$=$1; }
  | OPTIONMV bracketcierra ppath igual ruta    { $1->path=$5; $$=$1; }
  | OPTIONMV bracketcierra pdest igual cadena    { $1->destino=$5; $$=$1; }
  | OPTIONMV bracketcierra pdest igual ruta    { $1->destino=$5; $$=$1; }

  | bracketcierra ppath igual cadena  { obmv *disco = new obmv();  disco->path=$4; $$=disco; }
  | bracketcierra ppath igual ruta    { obmv *disco = new obmv();  disco->path=$4; $$=disco; }
  | bracketcierra pdest igual cadena  { obmv *disco = new obmv();  disco->destino=$4; $$=disco; }
  | bracketcierra pdest igual ruta    { obmv *disco = new obmv();  disco->destino=$4; $$=disco; }
;

GFIND:
  pfind OPTIONFIND { $2->mostrardatos($2); }
;

OPTIONFIND:
    OPTIONFIND bracketcierra ppath igual cadena             { $1->path=$5; $$=$1; }
  | OPTIONFIND bracketcierra ppath igual ruta               { $1->path=$5; $$=$1; }
  | OPTIONFIND bracketcierra pname igual caracterunico      { $1->caracter=$5; $1->name=$5; $$=$1; }
  | OPTIONFIND bracketcierra pname igual identificador      { $1->caracter=$5; $1->name=$5; $$=$1; }
  | OPTIONFIND bracketcierra pname igual cadena             { $1->caracter=$5; $1->name=$5; $$=$1; }
  | OPTIONFIND bracketcierra pname igual  multiplicacion    { $1->caracter=$5; $1->name=$5; $$=$1; }

  | bracketcierra ppath igual cadena                            { obfind *disco = new obfind();  disco->path=$4; $$=disco; }
  | bracketcierra ppath igual ruta                              { obfind *disco = new obfind();  disco->path=$4; $$=disco; }
  | bracketcierra pname igual caracterunico       { obfind *disco = new obfind();  disco->caracter=$4; disco->name=$4; $$=disco; }
  | bracketcierra pname igual identificador       { obfind *disco = new obfind();  disco->caracter=$4; disco->name=$4; $$=disco; }
  | bracketcierra pname igual cadena       { obfind *disco = new obfind();  disco->caracter=$4; disco->name=$4; $$=disco; }
  | bracketcierra pname igual multiplicacion        { obfind *disco = new obfind();  disco->caracter=$4; disco->name=$4; $$=disco; }
;

GPAUSE:
    ppause  {  system("read -p 'Presione Enter para continuar...' var"); }
;

GLOGIN:
    plogin OPTIONLOGIN  { $2->mostrardatos($2);  }
;

OPTIONLOGIN:
      OPTIONLOGIN bracketcierra puser igual identificador   { $1->usuario=$5; $$=$1; }
    | OPTIONLOGIN bracketcierra puser igual proot           { $1->usuario=$5; $$=$1; }
    | OPTIONLOGIN bracketcierra puser igual caracterunico   { $1->usuario=$5; $$=$1; }
    | OPTIONLOGIN bracketcierra puser igual codigo          { $1->usuario=$5; $$=$1; }
    | OPTIONLOGIN bracketcierra ppwd igual identificador    { $1->pasword=$5; $$=$1; }
    | OPTIONLOGIN bracketcierra ppwd igual caracterunico    { $1->pasword=$5; $$=$1; }
    | OPTIONLOGIN bracketcierra ppwd igual entero           { $1->pasword=$5; $$=$1; }
    | OPTIONLOGIN bracketcierra ppwd igual positivo         { $1->pasword=$5; $$=$1; }
    | OPTIONLOGIN bracketcierra pid igual codigo            { $1->id_particion=$5; $$=$1; }

    | bracketcierra puser igual identificador       { oblogin *disco = new oblogin(); disco->usuario=$4; $$=disco;}
    | bracketcierra puser igual proot               { oblogin *disco = new oblogin(); disco->usuario=$4; $$=disco;}
    | bracketcierra puser igual caracterunico       { oblogin *disco = new oblogin(); disco->usuario=$4; $$=disco;}
    | bracketcierra puser igual codigo              { oblogin *disco = new oblogin(); disco->usuario=$4; $$=disco;}
    | bracketcierra ppwd igual identificador        { oblogin *disco = new oblogin(); disco->pasword=$4; $$=disco;}
    | bracketcierra ppwd igual caracterunico        { oblogin *disco = new oblogin(); disco->pasword=$4; $$=disco; }
    | bracketcierra ppwd igual entero               { oblogin *disco = new oblogin(); disco->pasword=$4; $$=disco;}
    | bracketcierra ppwd igual positivo             { oblogin *disco = new oblogin(); disco->pasword=$4; $$=disco; }
    | bracketcierra pid igual codigo                { oblogin *disco = new oblogin(); disco->id_particion=$4; $$=disco; }
;

GLOGOUT:
  plogout { oblogin *disco = new oblogin(); disco->logout();  }
;

GMKGRP:
    pmkgrp bracketcierra pname igual identificador      { obmkgrp *disco = new obmkgrp(); disco->name=$5; disco->mostrardatos(disco); }
    | pmkgrp bracketcierra pname igual caracterunico    { obmkgrp *disco = new obmkgrp(); disco->name=$5; disco->mostrardatos(disco); }
    | pmkgrp bracketcierra pname igual codigo           { obmkgrp *disco = new obmkgrp(); disco->name=$5; disco->mostrardatos(disco); }
    | pmkgrp bracketcierra pname igual proot            { obmkgrp *disco = new obmkgrp(); disco->name=$5; disco->mostrardatos(disco); }
    | pmkgrp bracketcierra pname igual cadena           { obmkgrp *disco = new obmkgrp(); disco->name=$5; disco->mostrardatos(disco); }
;

GRMGRP:
      prmgrp bracketcierra pname igual identificador    { obrmgrp *disco = new obrmgrp(); disco->name=$5; disco->mostrardatos(disco); }
    | prmgrp bracketcierra pname igual caracterunico    { obrmgrp *disco = new obrmgrp(); disco->name=$5; disco->mostrardatos(disco); }
    | prmgrp bracketcierra pname igual codigo           { obrmgrp *disco = new obrmgrp(); disco->name=$5; disco->mostrardatos(disco); }
    | prmgrp bracketcierra pname igual proot            { obrmgrp *disco = new obrmgrp(); disco->name=$5; disco->mostrardatos(disco); }
    | prmgrp bracketcierra pname igual cadena           { obrmgrp *disco = new obrmgrp(); disco->name=$5; disco->mostrardatos(disco); }
;

GMKUSR:
    pmkusr OPTIONMKUSR { $2->mostrardatos($2); }
;
OPTIONMKUSR:
      OPTIONMKUSR bracketcierra pusr igual identificador   { $1->name=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra pusr igual proot           { $1->name=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra pusr igual caracterunico   { $1->name=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra pusr igual codigo          { $1->name=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra pusr igual cadena          { $1->name=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra ppwd igual identificador   { $1->password=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra ppwd igual caracterunico   { $1->password=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra ppwd igual codigo          { $1->password=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra ppwd igual cadena          { $1->password=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra ppwd igual entero          { $1->password=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra ppwd igual positivo        { $1->password=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra ppwd igual proot           { $1->password=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra pgrp igual identificador   { $1->grupo=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra pgrp igual caracterunico   { $1->grupo=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra pgrp igual codigo          { $1->grupo=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra pgrp igual cadena          { $1->grupo=$5; $$=$1; }
    | OPTIONMKUSR bracketcierra pgrp igual proot           { $1->grupo=$5; $$=$1; }

    | bracketcierra pusr igual identificador    { obmkusr *disco = new obmkusr(); disco->name=$4; $$=disco; }
    | bracketcierra pusr igual caracterunico    { obmkusr *disco = new obmkusr(); disco->name=$4; $$=disco; }
    | bracketcierra pusr igual codigo           { obmkusr *disco = new obmkusr(); disco->name=$4; $$=disco; }
    | bracketcierra pusr igual proot            { obmkusr *disco = new obmkusr(); disco->name=$4; $$=disco; }
    | bracketcierra pusr igual cadena           { obmkusr *disco = new obmkusr(); disco->name=$4; $$=disco; }
    | bracketcierra ppwd igual identificador    { obmkusr *disco = new obmkusr(); disco->password=$4; $$=disco; }
    | bracketcierra ppwd igual caracterunico    { obmkusr *disco = new obmkusr(); disco->password=$4; $$=disco; }
    | bracketcierra ppwd igual codigo           { obmkusr *disco = new obmkusr(); disco->password=$4; $$=disco; }
    | bracketcierra ppwd igual cadena           { obmkusr *disco = new obmkusr(); disco->password=$4; $$=disco; }
    | bracketcierra ppwd igual entero           { obmkusr *disco = new obmkusr(); disco->password=$4; $$=disco; }
    | bracketcierra ppwd igual positivo         { obmkusr *disco = new obmkusr(); disco->password=$4; $$=disco; }
    | bracketcierra ppwd igual proot            { obmkusr *disco = new obmkusr(); disco->password=$4; $$=disco; }
    | bracketcierra pgrp igual identificador    { obmkusr *disco = new obmkusr(); disco->grupo=$4; $$=disco; }
    | bracketcierra pgrp igual caracterunico    { obmkusr *disco = new obmkusr(); disco->grupo=$4; $$=disco; }
    | bracketcierra pgrp igual codigo           { obmkusr *disco = new obmkusr(); disco->grupo=$4; $$=disco; }
    | bracketcierra pgrp igual cadena           { obmkusr *disco = new obmkusr(); disco->grupo=$4; $$=disco; }
    | bracketcierra pgrp igual proot            { obmkusr *disco = new obmkusr(); disco->grupo=$4; $$=disco; }
;

GRMUSR:
      prmusr bracketcierra pusr igual identificador    { obrmusr *disco = new obrmusr(); disco->name=$5; disco->mostrardatos(disco); }
    | prmusr bracketcierra pusr igual caracterunico    { obrmusr *disco = new obrmusr(); disco->name=$5; disco->mostrardatos(disco); }
    | prmusr bracketcierra pusr igual codigo           { obrmusr *disco = new obrmusr(); disco->name=$5; disco->mostrardatos(disco);}
    | prmusr bracketcierra pusr igual proot            { obrmusr *disco = new obrmusr(); disco->name=$5; disco->mostrardatos(disco); }
    | prmusr bracketcierra pusr igual cadena           { obrmusr *disco = new obrmusr(); disco->name=$5; disco->mostrardatos(disco); }
;

GCHGRP:
  pchgrp OPTIONCHGRP { $2->mostrardatos($2); }
;
OPTIONCHGRP:
    OPTIONCHGRP bracketcierra pusr igual identificador   { $1->name=$5; $$=$1; }
  | OPTIONCHGRP bracketcierra pusr igual proot           { $1->name=$5; $$=$1; }
  | OPTIONCHGRP bracketcierra pusr igual caracterunico   { $1->name=$5; $$=$1; }
  | OPTIONCHGRP bracketcierra pusr igual codigo          { $1->name=$5; $$=$1; }
  | OPTIONCHGRP bracketcierra pusr igual cadena          { $1->name=$5; $$=$1; }
  | OPTIONCHGRP bracketcierra pgrp igual identificador   { $1->grupo=$5; $$=$1; }
  | OPTIONCHGRP bracketcierra pgrp igual caracterunico   { $1->grupo=$5; $$=$1; }
  | OPTIONCHGRP bracketcierra pgrp igual codigo          { $1->grupo=$5; $$=$1; }
  | OPTIONCHGRP bracketcierra pgrp igual cadena          { $1->grupo=$5; $$=$1; }
  | OPTIONCHGRP bracketcierra pgrp igual proot           { $1->grupo=$5; $$=$1; }

  | bracketcierra pusr igual identificador    { obchgrp *disco = new obchgrp(); disco->name=$4; $$=disco; }
  | bracketcierra pusr igual caracterunico    { obchgrp *disco = new obchgrp(); disco->name=$4; $$=disco; }
  | bracketcierra pusr igual codigo           { obchgrp *disco = new obchgrp(); disco->name=$4; $$=disco; }
  | bracketcierra pusr igual proot            { obchgrp *disco = new obchgrp(); disco->name=$4; $$=disco; }
  | bracketcierra pusr igual cadena           { obchgrp *disco = new obchgrp(); disco->name=$4; $$=disco; }
  | bracketcierra pgrp igual identificador    { obchgrp *disco = new obchgrp(); disco->grupo=$4; $$=disco; }
  | bracketcierra pgrp igual caracterunico    { obchgrp *disco = new obchgrp(); disco->grupo=$4; $$=disco; }
  | bracketcierra pgrp igual codigo           { obchgrp *disco = new obchgrp(); disco->grupo=$4; $$=disco; }
  | bracketcierra pgrp igual cadena           { obchgrp *disco = new obchgrp(); disco->grupo=$4; $$=disco; }
  | bracketcierra pgrp igual proot            { obchgrp *disco = new obchgrp(); disco->grupo=$4; $$=disco; }
;

GRECOVERY:
      precovery bracketcierra pid igual codigo { obrecovery *disco = new obrecovery(); disco->id=$5; disco->mostrardatos(disco); disco->recovery(); }
    | precovery bracketcierra pid igual cadena { obrecovery *disco = new obrecovery(); disco->id=$5; disco->mostrardatos(disco); disco->recovery(); }
    | ploss bracketcierra pid igual codigo { obrecovery *disco = new obrecovery(); disco->id=$5; disco->mostrardatos(disco); disco->simulateloss(); }
    | ploss bracketcierra pid igual cadena { obrecovery *disco = new obrecovery(); disco->id=$5; disco->mostrardatos(disco); disco->simulateloss(); }
;

