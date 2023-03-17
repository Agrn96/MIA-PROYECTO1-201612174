/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    psize = 258,
    pmkdisk = 259,
    pmrdisk = 260,
    ppath = 261,
    punit = 262,
    pfit = 263,
    pfdisk = 264,
    ptype = 265,
    pname = 266,
    pdelete = 267,
    padd = 268,
    pmount = 269,
    pumount = 270,
    pid = 271,
    pmkfs = 272,
    pfs = 273,
    p2fs = 274,
    p3fs = 275,
    punto = 276,
    bracketabre = 277,
    bracketcierra = 278,
    corcheteabre = 279,
    corchetecierra = 280,
    puntocoma = 281,
    potencia = 282,
    coma = 283,
    parentesisabre = 284,
    parentesiscierra = 285,
    llaveabre = 286,
    llavecierra = 287,
    mas = 288,
    menos = 289,
    multiplicacion = 290,
    igual = 291,
    dolar = 292,
    dospuntos = 293,
    interrogacion = 294,
    entero = 295,
    concero = 296,
    positivo = 297,
    numnegativo = 298,
    cadena = 299,
    identificador = 300,
    caracter = 301,
    ruta = 302,
    rutacualquiera = 303,
    uunit = 304,
    ffit = 305,
    caracterunico = 306,
    codigo = 307,
    ptouch = 308,
    pr = 309,
    pcont = 310,
    pstdin = 311,
    prep = 312,
    pmkdir = 313,
    pcat = 314,
    pren = 315,
    proot = 316,
    prm = 317,
    pedit = 318,
    pruta = 319,
    pexec = 320,
    pdest = 321,
    pcp = 322,
    pmv = 323,
    pfind = 324,
    ppause = 325,
    plogin = 326,
    puser = 327,
    ppwd = 328,
    plogout = 329,
    prmgrp = 330,
    pmkgrp = 331,
    pmkusr = 332,
    prmusr = 333,
    pusr = 334,
    pgrp = 335,
    pchgrp = 336,
    precovery = 337,
    ploss = 338,
    suma = 339,
    multi = 340,
    division = 341
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 48 "parser.y"

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


#line 176 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
