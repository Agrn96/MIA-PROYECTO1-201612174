/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include "scanner.h"//se importa el header del analisis sintactico
#include <QString>
#include <string>
#include "qdebug.h"
#include <iostream>
#include "list"

#include "obmkdisk.h"
#include "obmrdisk.h"
#include "obfdisk.h"
#include "obmount.h"
#include "obmkfs.h"
#include "obtouch.h"
#include "obrep.h"
#include "obmkdir.h"
#include "obcat.h"
#include "obren.h"
#include "obrm.h"
#include "obedit.h"
#include "obexec.h"
#include "obcp.h"
#include "obmv.h"
#include "obfind.h"
#include "oblogin.h"
#include "obmkgrp.h"
#include "obrmgrp.h"
#include "obmkusr.h"
#include "obrmusr.h"
#include "obchgrp.h"
#include "obrecovery.h"

using namespace std;
extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON

int yyerror(const char* mens)
{
std::cout << mens <<" "<<yytext<< std::endl;
return 0;
}

#line 114 "parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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


#line 285 "parser.cpp"

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



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  97
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   543

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  87
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  290
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  558

#define YYUNDEFTOK  2
#define YYMAXUTOK   341


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   195,   195,   199,   200,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     232,   237,   238,   239,   240,   241,   242,   244,   245,   246,
     247,   248,   249,   253,   257,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   293,   294,   298,   299,   300,   301,   303,
     304,   305,   306,   310,   315,   316,   317,   318,   320,   321,
     322,   323,   327,   331,   332,   333,   334,   335,   336,   337,
     338,   340,   341,   342,   343,   344,   345,   346,   347,   351,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   378,   382,
     383,   384,   386,   387,   388,   392,   396,   397,   399,   400,
     404,   408,   409,   410,   411,   413,   414,   415,   416,   421,
     422,   426,   430,   431,   432,   433,   434,   436,   437,   438,
     439,   440,   444,   445,   450,   454,   455,   456,   457,   459,
     460,   461,   462,   467,   471,   472,   473,   474,   476,   477,
     478,   479,   483,   487,   488,   489,   490,   491,   492,   494,
     495,   496,   497,   498,   499,   503,   507,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   533,   537,   538,   539,   540,
     541,   545,   546,   547,   548,   549,   553,   556,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   594,   595,   596,   597,   598,   602,   605,   606,   607,
     608,   609,   610,   611,   612,   613,   614,   616,   617,   618,
     619,   620,   621,   622,   623,   624,   625,   629,   630,   631,
     632
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "psize", "pmkdisk", "pmrdisk", "ppath",
  "punit", "pfit", "pfdisk", "ptype", "pname", "pdelete", "padd", "pmount",
  "pumount", "pid", "pmkfs", "pfs", "p2fs", "p3fs", "punto", "bracketabre",
  "bracketcierra", "corcheteabre", "corchetecierra", "puntocoma",
  "potencia", "coma", "parentesisabre", "parentesiscierra", "llaveabre",
  "llavecierra", "mas", "menos", "multiplicacion", "igual", "dolar",
  "dospuntos", "interrogacion", "entero", "concero", "positivo",
  "numnegativo", "cadena", "identificador", "caracter", "ruta",
  "rutacualquiera", "uunit", "ffit", "caracterunico", "codigo", "ptouch",
  "pr", "pcont", "pstdin", "prep", "pmkdir", "pcat", "pren", "proot",
  "prm", "pedit", "pruta", "pexec", "pdest", "pcp", "pmv", "pfind",
  "ppause", "plogin", "puser", "ppwd", "plogout", "prmgrp", "pmkgrp",
  "pmkusr", "prmusr", "pusr", "pgrp", "pchgrp", "precovery", "ploss",
  "suma", "multi", "division", "$accept", "INICIO", "INSTRUCCIONES",
  "INSTRUCCION", "GMKDISK", "OPTIONMKDISK", "GMRDISK", "GFDISK",
  "OPTIONFDISK", "GMOUNT", "OPTIONMOUNT", "GMKFS", "OPTIONMKFS", "GTOUCH",
  "OPTIONTOUCH", "GREP", "OPTIONREP", "GMKDIR", "OPTIONMKDIR", "GCAT",
  "OPTIONCAT", "GCREN", "OPTIONREN", "GRM", "GEDIT", "OPTIONEDIT", "GEXEC",
  "GCP", "OPTIONCP", "GMV", "OPTIONMV", "GFIND", "OPTIONFIND", "GPAUSE",
  "GLOGIN", "OPTIONLOGIN", "GLOGOUT", "GMKGRP", "GRMGRP", "GMKUSR",
  "OPTIONMKUSR", "GRMUSR", "GCHGRP", "OPTIONCHGRP", "GRECOVERY", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341
};
# endif

#define YYPACT_NINF (-69)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -4,   -16,    21,    24,    59,    64,    70,    79,    88,   120,
     130,   146,   156,   185,   203,   223,   332,   333,   -69,   334,
     -69,   335,   336,   337,   338,   339,   340,   341,   271,    -4,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   241,   342,   286,   217,
     343,    51,   344,   352,     4,   346,    36,   347,    19,   348,
      -3,   349,   328,   351,   136,   353,   369,    42,   354,   372,
       2,   356,     3,   357,   143,   358,    13,   359,   373,   374,
      35,   360,   314,   -68,   364,   375,   376,   -69,   -69,   361,
     362,   363,   365,   248,   366,   367,   368,   370,   371,   377,
     378,   379,   380,   228,   381,   382,   162,   383,   384,   385,
     386,   227,   387,   388,   -69,   389,   -69,    40,   390,   391,
     392,   393,   394,    20,   395,   -69,     0,   396,   345,   397,
     398,   169,   399,   400,   401,   -69,    44,   402,   403,   404,
       9,   405,   406,    10,   407,   408,   190,   409,   410,   411,
      16,   412,   413,   414,   415,   416,    37,   417,   418,   419,
     -20,   420,   421,   155,    -7,   422,   350,   423,   424,   425,
     426,   428,   179,    -6,   427,   355,   429,   188,   430,   -21,
     431,   432,   433,   434,   437,   441,   442,   443,    80,   208,
     444,   445,   435,   438,   436,   320,   446,   448,   449,   283,
     170,   213,   450,   453,   -69,   454,   -69,   214,   297,   439,
     284,   215,   456,   457,   458,   459,   460,   219,   461,   -69,
     220,   462,   221,   299,   463,   464,   225,   226,   230,   465,
     466,   -69,   231,   232,   236,   467,   468,   237,   238,   469,
     470,   242,   -17,   471,   472,   473,   -28,   167,   474,   475,
     476,    85,    87,    89,    61,   100,   477,   478,   479,   111,
     113,   115,   480,   481,    76,   198,   -69,   -69,   -69,   -69,
     -69,   -69,   287,   243,   482,   483,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   288,   -69,   -69,
     291,   244,   484,   485,   487,   301,   489,    81,   -69,   -69,
     -69,   -69,   249,   303,   -69,   -69,   -69,   -69,   -69,   490,
     486,   330,   -69,   -69,   -69,   -69,   -69,   -69,   292,   250,
     251,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     255,   307,   488,   295,   256,   -69,   -69,   257,   -69,   -69,
     261,   -69,   -69,   -69,   -69,   262,   309,   -69,   -69,   -69,
     -69,   -69,   -69,   263,   267,   -69,   -69,   -69,   -69,   -69,
     -69,   268,   269,   -69,   -69,   -69,   -69,   273,   274,   -69,
     -69,   -69,   -69,   -69,   -69,   275,   159,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   491,   154,   171,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   126,    74,   137,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   139,   141,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   296,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   195,     0,
     215,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,     0,    30,     0,     0,
      44,     0,    73,     0,     0,    83,     0,    92,     0,   109,
       0,   128,     0,   135,     0,   140,     0,     0,   151,     0,
       0,   164,     0,   173,     0,   182,     0,   196,     0,     0,
       0,   226,     0,     0,   266,     0,     0,     1,     3,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   103,     0,   108,     0,     0,     0,
       0,     0,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   129,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   156,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    38,    37,    39,    40,
      41,    42,     0,     0,     0,     0,    43,    59,    60,    62,
      63,    61,    65,    64,    66,    67,    68,     0,    70,    69,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      82,    81,     0,     0,    74,    89,    88,    90,    91,     0,
       0,     0,   105,   104,   101,   102,   106,   107,     0,     0,
       0,   120,   121,   123,   122,   119,   125,   124,   126,   127,
       0,     0,     0,     0,     0,   133,   134,     0,   138,   139,
       0,   145,   146,   148,   147,     0,     0,   149,   150,   157,
     158,   159,   160,     0,     0,   162,   163,   169,   170,   171,
     172,     0,     0,   178,   179,   180,   181,     0,     0,   189,
     190,   194,   193,   192,   191,     0,     0,   214,   206,   208,
     209,   207,   212,   213,   210,   211,     0,     0,     0,   225,
     221,   222,   223,   224,   220,   216,   217,   218,   219,   248,
     244,   245,   246,   247,   253,   254,   252,   249,   250,   251,
     255,   259,   256,   257,   258,   260,     0,     0,     0,   265,
     261,   262,   263,   264,   281,   277,   278,   279,   280,   285,
     282,   283,   284,   286,     0,     0,   288,   287,   290,   289,
      32,    31,    33,    34,    35,    36,    72,    71,    45,    46,
      48,    49,    47,    51,    50,    52,    53,    54,     0,    56,
      55,    75,    76,    78,    77,    85,    84,    86,    87,    97,
      96,    93,    94,    98,    99,   111,   112,   114,   113,   110,
     116,   115,   117,   118,   130,   131,   136,   137,   141,   142,
     144,   143,   152,   153,   154,   155,   165,   166,   167,   168,
     174,   175,   176,   177,   183,   184,   188,   187,   186,   185,
     205,   197,   199,   200,   198,   203,   204,   201,   202,   231,
     227,   229,   230,   228,   236,   237,   235,   232,   233,   234,
     238,   242,   239,   240,   241,   243,   271,   267,   269,   270,
     268,   275,   272,   273,   274,   276,    58,    57
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -69,   -69,   -69,   492,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    28,    29,    30,    31,    57,    32,    33,    60,    34,
      62,    35,    65,    36,    67,    37,    69,    38,    71,    39,
      73,    40,    75,    41,    42,    78,    43,    44,    81,    45,
      83,    46,    85,    47,    48,    87,    49,    50,    51,    52,
      91,    53,    54,    94,    55
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       1,     2,   297,   134,   168,     3,   228,    56,   148,   151,
       4,     5,   169,     6,   118,   245,   249,   388,   381,   298,
     119,   299,   120,   389,   390,   128,   222,   382,   383,   157,
     129,   223,   258,   391,   384,   130,   224,   278,   289,   122,
     279,   290,   123,   212,    58,   229,   213,    59,   143,     7,
     239,   135,   272,     8,     9,    10,    11,   114,    12,    13,
     273,    14,   115,    15,    16,    17,    18,    19,   149,   152,
      20,    21,    22,    23,    24,   246,   250,    25,    26,    27,
     131,   225,    61,   132,   226,   158,   159,    63,   259,   260,
     124,   125,   126,    64,   214,   215,   216,   144,   145,   240,
     241,   414,    66,   415,   468,   416,   417,   163,   164,   266,
     267,    68,   418,   419,   534,   165,   535,   268,   536,   537,
     446,   469,   420,   470,   308,   538,   539,   309,   447,   399,
     400,   404,   405,   409,   410,   540,   401,   402,   406,   407,
     411,   412,   139,    70,   421,   422,   403,   140,   408,   154,
     413,   423,   424,    72,   155,   429,   430,   434,   435,   439,
     440,   425,   431,   432,   436,   437,   441,   442,   200,    74,
     529,   530,   433,   201,   438,   234,   443,   531,   532,    76,
     235,   541,   542,   546,   547,   551,   552,   533,   543,   544,
     548,   549,   553,   554,   516,   276,   253,   277,   545,   521,
     550,   254,   555,   517,   518,   522,   523,   392,    77,   393,
     519,   525,   394,   526,   324,   524,   527,   325,   395,   287,
     105,   288,   528,   106,   107,   108,    79,   109,   110,   111,
     112,   190,   294,   295,   191,   192,   193,   206,   194,   195,
     196,   197,   448,   207,    99,   208,    80,   100,   101,   102,
     449,   177,   310,   311,   178,   179,   180,   326,   331,   338,
     327,   332,   339,   345,   348,   351,   346,   349,   352,   357,
     359,    97,   358,   360,   361,   365,   367,   362,   366,   368,
     369,   373,   375,   370,   374,   376,   379,   452,   460,   380,
     453,   461,   104,   471,   481,   483,   472,   482,   484,   485,
     492,   494,   486,   493,   495,   496,   498,   502,   497,   499,
     503,   504,   506,   508,   505,   507,   509,   510,   512,   514,
     511,   513,   515,   322,   336,   323,   337,   450,   456,   451,
     457,   458,   479,   459,   480,   490,   556,   491,   557,   317,
     318,   333,   334,   353,   354,   465,   466,   473,   474,   477,
     478,   487,   488,   500,   501,    82,    84,    86,    88,    89,
      90,    92,    93,    95,    96,   103,   113,   116,   117,   121,
     127,   133,   136,   137,   138,   142,   141,   146,   147,   150,
     153,   156,   160,   166,   161,   162,   167,   170,     0,     0,
     231,   171,   172,     0,     0,   281,     0,   173,   174,   175,
     292,   176,   181,   182,   183,     0,   184,   185,     0,     0,
       0,     0,     0,   186,   187,   188,   189,   198,   199,   202,
     203,   204,   205,   209,   210,   211,   217,   218,   219,   220,
     221,   227,   230,   232,   233,   236,   237,   238,   242,   243,
     244,   247,   248,   251,   252,   255,   256,   257,   261,   262,
     263,   264,   265,   269,   270,   271,   274,   275,     0,   282,
     283,   284,   285,     0,     0,     0,     0,   300,   301,   302,
     303,   280,   286,   304,   293,   296,   291,   305,   306,   307,
     312,   313,   319,   315,   320,   321,   328,   314,   316,   329,
     330,   335,   340,   341,   342,   343,   344,   347,   350,   355,
     356,   363,   364,   371,   372,   377,   378,   385,   386,     0,
     396,   397,   398,   426,   427,   428,   444,   445,     0,     0,
       0,    98,     0,     0,     0,   387,     0,     0,   455,     0,
     463,   454,   464,   462,   467,   475,     0,     0,   476,     0,
     489,     0,     0,   520
};

static const yytype_int8 yycheck[] =
{
       4,     5,    23,     6,    72,     9,     6,    23,     6,     6,
      14,    15,    80,    17,    10,     6,     6,    45,    35,    40,
      16,    42,    18,    51,    52,     6,     6,    44,    45,    16,
      11,    11,    16,    61,    51,    16,    16,    44,    44,     3,
      47,    47,     6,     3,    23,    45,     6,    23,     6,    53,
       6,    54,    72,    57,    58,    59,    60,     6,    62,    63,
      80,    65,    11,    67,    68,    69,    70,    71,    66,    66,
      74,    75,    76,    77,    78,    66,    66,    81,    82,    83,
      61,    61,    23,    64,    64,    72,    73,    23,    72,    73,
      54,    55,    56,    23,    54,    55,    56,    55,    56,    55,
      56,    40,    23,    42,    23,    44,    45,    72,    73,    72,
      73,    23,    51,    52,    40,    80,    42,    80,    44,    45,
      44,    40,    61,    42,    44,    51,    52,    47,    52,    44,
      45,    44,    45,    44,    45,    61,    51,    52,    51,    52,
      51,    52,     6,    23,    44,    45,    61,    11,    61,     6,
      61,    51,    52,    23,    11,    44,    45,    44,    45,    44,
      45,    61,    51,    52,    51,    52,    51,    52,     6,    23,
      44,    45,    61,    11,    61,     6,    61,    51,    52,    23,
      11,    44,    45,    44,    45,    44,    45,    61,    51,    52,
      51,    52,    51,    52,    35,    40,     6,    42,    61,    45,
      61,    11,    61,    44,    45,    51,    52,    40,    23,    42,
      51,    40,    45,    42,    44,    61,    45,    47,    51,    40,
       3,    42,    51,     6,     7,     8,    23,    10,    11,    12,
      13,     3,    44,    45,     6,     7,     8,    10,    10,    11,
      12,    13,    44,    16,     3,    18,    23,     6,     7,     8,
      52,     3,    44,    45,     6,     7,     8,    44,    44,    44,
      47,    47,    47,    44,    44,    44,    47,    47,    47,    44,
      44,     0,    47,    47,    44,    44,    44,    47,    47,    47,
      44,    44,    44,    47,    47,    47,    44,    44,    44,    47,
      47,    47,     6,    44,    44,    44,    47,    47,    47,    44,
      44,    44,    47,    47,    47,    44,    44,    44,    47,    47,
      47,    44,    44,    44,    47,    47,    47,    44,    44,    44,
      47,    47,    47,    40,    40,    42,    42,    40,    40,    42,
      42,    40,    40,    42,    42,    40,    40,    42,    42,    19,
      20,    44,    45,    44,    45,    44,    45,    44,    45,    19,
      20,    44,    45,    44,    45,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    23,    23,    16,    23,
      23,    23,    23,    45,    23,     6,    23,    23,     6,    23,
      23,    23,    23,    23,    11,    11,    72,    23,    -1,    -1,
      45,    16,    16,    -1,    -1,    45,    -1,    36,    36,    36,
      45,    36,    36,    36,    36,    -1,    36,    36,    -1,    -1,
      -1,    -1,    -1,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    -1,    36,
      36,    36,    36,    -1,    -1,    -1,    -1,    36,    36,    36,
      36,    49,    44,    36,    45,    45,    49,    36,    36,    36,
      36,    36,    36,    45,    36,    36,    36,    52,    52,    36,
      36,    52,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    -1,
      36,    36,    36,    36,    36,    36,    36,    36,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    52,    -1,    -1,    45,    -1,
      45,    49,    45,    49,    45,    45,    -1,    -1,    52,    -1,
      52,    -1,    -1,    52
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,     9,    14,    15,    17,    53,    57,    58,
      59,    60,    62,    63,    65,    67,    68,    69,    70,    71,
      74,    75,    76,    77,    78,    81,    82,    83,    88,    89,
      90,    91,    93,    94,    96,    98,   100,   102,   104,   106,
     108,   110,   111,   113,   114,   116,   118,   120,   121,   123,
     124,   125,   126,   128,   129,   131,    23,    92,    23,    23,
      95,    23,    97,    23,    23,    99,    23,   101,    23,   103,
      23,   105,    23,   107,    23,   109,    23,    23,   112,    23,
      23,   115,    23,   117,    23,   119,    23,   122,    23,    23,
      23,   127,    23,    23,   130,    23,    23,     0,    90,     3,
       6,     7,     8,    23,     6,     3,     6,     7,     8,    10,
      11,    12,    13,    23,     6,    11,    23,    16,    10,    16,
      18,    23,     3,     6,    54,    55,    56,    23,     6,    11,
      16,    61,    64,    23,     6,    54,    23,    45,    23,     6,
      11,    23,     6,     6,    55,    56,    23,     6,     6,    66,
      23,     6,    66,    23,     6,    11,    23,    16,    72,    73,
      23,    11,    11,    72,    73,    80,    23,    72,    72,    80,
      23,    16,    16,    36,    36,    36,    36,     3,     6,     7,
       8,    36,    36,    36,    36,    36,    36,    36,    36,    36,
       3,     6,     7,     8,    10,    11,    12,    13,    36,    36,
       6,    11,    36,    36,    36,    36,    10,    16,    18,    36,
      36,    36,     3,     6,    54,    55,    56,    36,    36,    36,
      36,    36,     6,    11,    16,    61,    64,    36,     6,    45,
      36,    45,    36,    36,     6,    11,    36,    36,    36,     6,
      55,    56,    36,    36,    36,     6,    66,    36,    36,     6,
      66,    36,    36,     6,    11,    36,    36,    36,    16,    72,
      73,    36,    36,    36,    36,    36,    72,    73,    80,    36,
      36,    36,    72,    80,    36,    36,    40,    42,    44,    47,
      49,    45,    36,    36,    36,    36,    44,    40,    42,    44,
      47,    49,    45,    45,    44,    45,    45,    23,    40,    42,
      36,    36,    36,    36,    36,    36,    36,    36,    44,    47,
      44,    45,    36,    36,    52,    45,    52,    19,    20,    36,
      36,    36,    40,    42,    44,    47,    44,    47,    36,    36,
      36,    44,    47,    44,    45,    52,    40,    42,    44,    47,
      36,    36,    36,    36,    36,    44,    47,    36,    44,    47,
      36,    44,    47,    44,    45,    36,    36,    44,    47,    44,
      47,    44,    47,    36,    36,    44,    47,    44,    47,    44,
      47,    36,    36,    44,    47,    44,    47,    36,    36,    44,
      47,    35,    44,    45,    51,    36,    36,    52,    45,    51,
      52,    61,    40,    42,    45,    51,    36,    36,    36,    44,
      45,    51,    52,    61,    44,    45,    51,    52,    61,    44,
      45,    51,    52,    61,    40,    42,    44,    45,    51,    52,
      61,    44,    45,    51,    52,    61,    36,    36,    36,    44,
      45,    51,    52,    61,    44,    45,    51,    52,    61,    44,
      45,    51,    52,    61,    36,    36,    44,    52,    44,    52,
      40,    42,    44,    47,    49,    45,    40,    42,    40,    42,
      44,    47,    49,    45,    45,    44,    45,    45,    23,    40,
      42,    44,    47,    44,    45,    45,    52,    19,    20,    40,
      42,    44,    47,    44,    47,    44,    47,    44,    45,    52,
      40,    42,    44,    47,    44,    47,    44,    47,    44,    47,
      44,    45,    44,    47,    44,    47,    44,    47,    44,    47,
      44,    47,    44,    47,    44,    47,    35,    44,    45,    51,
      52,    45,    51,    52,    61,    40,    42,    45,    51,    44,
      45,    51,    52,    61,    40,    42,    44,    45,    51,    52,
      61,    44,    45,    51,    52,    61,    44,    45,    51,    52,
      61,    44,    45,    51,    52,    61,    40,    42
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    87,    88,    89,    89,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      91,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    93,    94,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    96,    96,    97,    97,    97,    97,    97,
      97,    97,    97,    98,    99,    99,    99,    99,    99,    99,
      99,    99,   100,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   102,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   104,   105,
     105,   105,   105,   105,   105,   106,   107,   107,   107,   107,
     108,   109,   109,   109,   109,   109,   109,   109,   109,   110,
     110,   111,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   113,   113,   114,   115,   115,   115,   115,   115,
     115,   115,   115,   116,   117,   117,   117,   117,   117,   117,
     117,   117,   118,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   120,   121,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   123,   124,   124,   124,   124,
     124,   125,   125,   125,   125,   125,   126,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   128,   128,   128,   128,   128,   129,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   131,   131,   131,
     131
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     5,     5,     5,     5,     5,     5,     4,     4,     4,
       4,     4,     4,     5,     2,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     6,     6,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     5,     5,     2,     5,     5,     5,     5,     5,     4,
       4,     4,     4,     2,     5,     5,     5,     5,     4,     4,
       4,     4,     2,     5,     5,     3,     5,     5,     5,     5,
       3,     4,     4,     2,     4,     4,     4,     4,     2,     2,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     2,     3,
       5,     5,     2,     4,     4,     2,     5,     5,     4,     4,
       2,     5,     5,     5,     5,     4,     4,     4,     4,     5,
       5,     2,     5,     5,     5,     5,     3,     4,     4,     4,
       4,     2,     5,     5,     2,     5,     5,     5,     5,     4,
       4,     4,     4,     2,     5,     5,     5,     5,     4,     4,
       4,     4,     2,     5,     5,     5,     5,     5,     5,     4,
       4,     4,     4,     4,     4,     1,     2,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     1,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     2,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     5,     5,     5,     5,     5,     2,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     5,     5,     5,
       5
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 195 "parser.y"
                  {}
#line 1963 "parser.cpp"
    break;

  case 3:
#line 199 "parser.y"
                                    {}
#line 1969 "parser.cpp"
    break;

  case 4:
#line 200 "parser.y"
                                    {}
#line 1975 "parser.cpp"
    break;

  case 5:
#line 204 "parser.y"
                  {   }
#line 1981 "parser.cpp"
    break;

  case 6:
#line 205 "parser.y"
                  {   }
#line 1987 "parser.cpp"
    break;

  case 7:
#line 206 "parser.y"
                  {   }
#line 1993 "parser.cpp"
    break;

  case 8:
#line 207 "parser.y"
                  {   }
#line 1999 "parser.cpp"
    break;

  case 9:
#line 208 "parser.y"
                  {   }
#line 2005 "parser.cpp"
    break;

  case 10:
#line 209 "parser.y"
                  {   }
#line 2011 "parser.cpp"
    break;

  case 11:
#line 210 "parser.y"
                  {   }
#line 2017 "parser.cpp"
    break;

  case 12:
#line 211 "parser.y"
                  {   }
#line 2023 "parser.cpp"
    break;

  case 13:
#line 212 "parser.y"
                  {   }
#line 2029 "parser.cpp"
    break;

  case 14:
#line 213 "parser.y"
                  {   }
#line 2035 "parser.cpp"
    break;

  case 15:
#line 214 "parser.y"
                  {   }
#line 2041 "parser.cpp"
    break;

  case 16:
#line 215 "parser.y"
                  {   }
#line 2047 "parser.cpp"
    break;

  case 17:
#line 216 "parser.y"
                  {   }
#line 2053 "parser.cpp"
    break;

  case 18:
#line 217 "parser.y"
                  {   }
#line 2059 "parser.cpp"
    break;

  case 19:
#line 218 "parser.y"
                  {   }
#line 2065 "parser.cpp"
    break;

  case 20:
#line 219 "parser.y"
                  {   }
#line 2071 "parser.cpp"
    break;

  case 21:
#line 220 "parser.y"
                  {   }
#line 2077 "parser.cpp"
    break;

  case 22:
#line 221 "parser.y"
                  {   }
#line 2083 "parser.cpp"
    break;

  case 23:
#line 222 "parser.y"
                  {   }
#line 2089 "parser.cpp"
    break;

  case 24:
#line 223 "parser.y"
                  {   }
#line 2095 "parser.cpp"
    break;

  case 25:
#line 224 "parser.y"
                  {   }
#line 2101 "parser.cpp"
    break;

  case 26:
#line 225 "parser.y"
                  {   }
#line 2107 "parser.cpp"
    break;

  case 27:
#line 226 "parser.y"
                  {   }
#line 2113 "parser.cpp"
    break;

  case 28:
#line 227 "parser.y"
                  {   }
#line 2119 "parser.cpp"
    break;

  case 29:
#line 228 "parser.y"
                    {   }
#line 2125 "parser.cpp"
    break;

  case 30:
#line 232 "parser.y"
                         {(yyvsp[0].mdisk)->mostrardatos((yyvsp[0].mdisk));}
#line 2131 "parser.cpp"
    break;

  case 31:
#line 237 "parser.y"
                                                       { int tam=atoi((yyvsp[0].TEXT));  (yyvsp[-4].mdisk)->size=tam; (yyval.mdisk)=(yyvsp[-4].mdisk); }
#line 2137 "parser.cpp"
    break;

  case 32:
#line 238 "parser.y"
                                                     { int tam=atoi((yyvsp[0].TEXT));  (yyvsp[-4].mdisk)->size=tam; (yyval.mdisk)=(yyvsp[-4].mdisk); }
#line 2143 "parser.cpp"
    break;

  case 33:
#line 239 "parser.y"
                                                     { (yyvsp[-4].mdisk)->path=(yyvsp[0].TEXT); (yyval.mdisk)=(yyvsp[-4].mdisk); }
#line 2149 "parser.cpp"
    break;

  case 34:
#line 240 "parser.y"
                                                   { (yyvsp[-4].mdisk)->path=(yyvsp[0].TEXT); (yyval.mdisk)=(yyvsp[-4].mdisk); }
#line 2155 "parser.cpp"
    break;

  case 35:
#line 241 "parser.y"
                                                    { (yyvsp[-4].mdisk)->unit=(yyvsp[0].TEXT); (yyval.mdisk)=(yyvsp[-4].mdisk); }
#line 2161 "parser.cpp"
    break;

  case 36:
#line 242 "parser.y"
                                                           { (yyvsp[-4].mdisk)->fit=(yyvsp[0].TEXT); (yyval.mdisk)=(yyvsp[-4].mdisk); }
#line 2167 "parser.cpp"
    break;

  case 37:
#line 244 "parser.y"
                                         {obmkdisk *disco=new obmkdisk(); int tam=atoi((yyvsp[0].TEXT)); disco->size=tam; (yyval.mdisk)=disco; }
#line 2173 "parser.cpp"
    break;

  case 38:
#line 245 "parser.y"
                                       {obmkdisk *disco=new obmkdisk(); int tam=atoi((yyvsp[0].TEXT)); disco->size=tam; (yyval.mdisk)=disco; }
#line 2179 "parser.cpp"
    break;

  case 39:
#line 246 "parser.y"
                                       {obmkdisk *disco=new obmkdisk(); disco->path=(yyvsp[0].TEXT); (yyval.mdisk)=disco; }
#line 2185 "parser.cpp"
    break;

  case 40:
#line 247 "parser.y"
                                     {obmkdisk *disco=new obmkdisk(); disco->path=(yyvsp[0].TEXT); (yyval.mdisk)=disco; }
#line 2191 "parser.cpp"
    break;

  case 41:
#line 248 "parser.y"
                                       {obmkdisk *disco=new obmkdisk(); disco->unit=(yyvsp[0].TEXT); (yyval.mdisk)=disco; }
#line 2197 "parser.cpp"
    break;

  case 42:
#line 249 "parser.y"
                                                {obmkdisk *disco=new obmkdisk(); disco->fit=(yyvsp[0].TEXT); (yyval.mdisk)=disco; }
#line 2203 "parser.cpp"
    break;

  case 43:
#line 253 "parser.y"
                                             {obmrdisk *disco=new obmrdisk(); disco->mostrardatos((yyvsp[0].TEXT)); }
#line 2209 "parser.cpp"
    break;

  case 44:
#line 257 "parser.y"
                       { (yyvsp[0].fdisk)->mostrardatos((yyvsp[0].fdisk)); }
#line 2215 "parser.cpp"
    break;

  case 45:
#line 261 "parser.y"
                                                   { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-4].fdisk)->size=tam;  (yyval.fdisk)=(yyvsp[-4].fdisk);  }
#line 2221 "parser.cpp"
    break;

  case 46:
#line 262 "parser.y"
                                                     { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-4].fdisk)->size=tam;  (yyval.fdisk)=(yyvsp[-4].fdisk);  }
#line 2227 "parser.cpp"
    break;

  case 47:
#line 263 "parser.y"
                                                  { (yyvsp[-4].fdisk)->unit=(yyvsp[0].TEXT);  (yyval.fdisk)=(yyvsp[-4].fdisk); }
#line 2233 "parser.cpp"
    break;

  case 48:
#line 264 "parser.y"
                                                    { (yyvsp[-4].fdisk)->path=(yyvsp[0].TEXT);  (yyval.fdisk)=(yyvsp[-4].fdisk); }
#line 2239 "parser.cpp"
    break;

  case 49:
#line 265 "parser.y"
                                                  { (yyvsp[-4].fdisk)->path=(yyvsp[0].TEXT);  (yyval.fdisk)=(yyvsp[-4].fdisk); }
#line 2245 "parser.cpp"
    break;

  case 50:
#line 266 "parser.y"
                                                           { (yyvsp[-4].fdisk)->type=(yyvsp[0].TEXT);  (yyval.fdisk)=(yyvsp[-4].fdisk); }
#line 2251 "parser.cpp"
    break;

  case 51:
#line 267 "parser.y"
                                                          { (yyvsp[-4].fdisk)->fit=(yyvsp[0].TEXT);  (yyval.fdisk)=(yyvsp[-4].fdisk); }
#line 2257 "parser.cpp"
    break;

  case 52:
#line 268 "parser.y"
                                                    { (yyvsp[-4].fdisk)->name=(yyvsp[0].TEXT);   (yyval.fdisk)=(yyvsp[-4].fdisk);  }
#line 2263 "parser.cpp"
    break;

  case 53:
#line 269 "parser.y"
                                                           { (yyvsp[-4].fdisk)->name=(yyvsp[0].TEXT);  (yyval.fdisk)=(yyvsp[-4].fdisk); }
#line 2269 "parser.cpp"
    break;

  case 54:
#line 270 "parser.y"
                                                             { (yyvsp[-4].fdisk)->delet=(yyvsp[0].TEXT);  (yyval.fdisk)=(yyvsp[-4].fdisk); }
#line 2275 "parser.cpp"
    break;

  case 55:
#line 271 "parser.y"
                                                     { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-4].fdisk)->add_type=true; (yyvsp[-4].fdisk)->add_size=tam; (yyval.fdisk)=(yyvsp[-4].fdisk); }
#line 2281 "parser.cpp"
    break;

  case 56:
#line 272 "parser.y"
                                                   { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-4].fdisk)->add_type=true; (yyvsp[-4].fdisk)->add_size=tam; (yyval.fdisk)=(yyvsp[-4].fdisk); }
#line 2287 "parser.cpp"
    break;

  case 57:
#line 273 "parser.y"
                                                                   { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-5].fdisk)->add_type=false; (yyvsp[-5].fdisk)->add_size=tam; (yyval.fdisk)=(yyvsp[-5].fdisk); }
#line 2293 "parser.cpp"
    break;

  case 58:
#line 274 "parser.y"
                                                                { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-5].fdisk)->add_type=false; (yyvsp[-5].fdisk)->add_size=tam; (yyval.fdisk)=(yyvsp[-5].fdisk); }
#line 2299 "parser.cpp"
    break;

  case 59:
#line 276 "parser.y"
                                       { int tam=atoi((yyvsp[0].TEXT)); obfdisk *disco=new obfdisk(); disco->size=tam;     (yyval.fdisk)=disco; }
#line 2305 "parser.cpp"
    break;

  case 60:
#line 277 "parser.y"
                                         { int tam=atoi((yyvsp[0].TEXT)); obfdisk *disco=new obfdisk(); disco->size=tam;      (yyval.fdisk)=disco; }
#line 2311 "parser.cpp"
    break;

  case 61:
#line 278 "parser.y"
                                      { obfdisk *disco=new obfdisk(); disco->unit=(yyvsp[0].TEXT);      (yyval.fdisk)=disco;}
#line 2317 "parser.cpp"
    break;

  case 62:
#line 279 "parser.y"
                                        { obfdisk *disco=new obfdisk(); disco->path=(yyvsp[0].TEXT);      (yyval.fdisk)=disco; }
#line 2323 "parser.cpp"
    break;

  case 63:
#line 280 "parser.y"
                                      { obfdisk *disco=new obfdisk(); disco->path=(yyvsp[0].TEXT);      (yyval.fdisk)=disco;  }
#line 2329 "parser.cpp"
    break;

  case 64:
#line 281 "parser.y"
                                               { obfdisk *disco=new obfdisk(); disco->type=(yyvsp[0].TEXT);      (yyval.fdisk)=disco; }
#line 2335 "parser.cpp"
    break;

  case 65:
#line 282 "parser.y"
                                              { obfdisk *disco=new obfdisk(); disco->fit=(yyvsp[0].TEXT);     (yyval.fdisk)=disco; }
#line 2341 "parser.cpp"
    break;

  case 66:
#line 283 "parser.y"
                                        { obfdisk *disco=new obfdisk(); disco->name=(yyvsp[0].TEXT);      (yyval.fdisk)=disco; }
#line 2347 "parser.cpp"
    break;

  case 67:
#line 284 "parser.y"
                                               { obfdisk *disco=new obfdisk(); disco->name=(yyvsp[0].TEXT);      (yyval.fdisk)=disco; }
#line 2353 "parser.cpp"
    break;

  case 68:
#line 285 "parser.y"
                                                 { obfdisk *disco=new obfdisk(); disco->delet=(yyvsp[0].TEXT);      (yyval.fdisk)=disco; }
#line 2359 "parser.cpp"
    break;

  case 69:
#line 286 "parser.y"
                                         { int tam=atoi((yyvsp[0].TEXT)); obfdisk *disco=new obfdisk(); disco->add_type=true; disco->add_size=tam; (yyval.fdisk)=disco; }
#line 2365 "parser.cpp"
    break;

  case 70:
#line 287 "parser.y"
                                       { int tam=atoi((yyvsp[0].TEXT)); obfdisk *disco=new obfdisk(); disco->add_type=true; disco->add_size=tam; (yyval.fdisk)=disco; }
#line 2371 "parser.cpp"
    break;

  case 71:
#line 288 "parser.y"
                                                       { int tam=atoi((yyvsp[0].TEXT)); obfdisk *disco=new obfdisk(); disco->add_type=false; disco->add_size=tam; (yyval.fdisk)=disco; }
#line 2377 "parser.cpp"
    break;

  case 72:
#line 289 "parser.y"
                                                     { int tam=atoi((yyvsp[0].TEXT)); obfdisk *disco=new obfdisk(); disco->add_type=false; disco->add_size=tam; (yyval.fdisk)=disco; }
#line 2383 "parser.cpp"
    break;

  case 73:
#line 293 "parser.y"
                       { (yyvsp[0].montar)->mostrardatos((yyvsp[0].montar));}
#line 2389 "parser.cpp"
    break;

  case 74:
#line 294 "parser.y"
                                              { obmount *disco = new obmount(); disco->id=(yyvsp[0].TEXT);  disco->DesmontarMount(disco); }
#line 2395 "parser.cpp"
    break;

  case 75:
#line 298 "parser.y"
                                                 { (yyvsp[-4].montar)->path=(yyvsp[0].TEXT); (yyval.montar)=(yyvsp[-4].montar); }
#line 2401 "parser.cpp"
    break;

  case 76:
#line 299 "parser.y"
                                                 { (yyvsp[-4].montar)->path=(yyvsp[0].TEXT); (yyval.montar)=(yyvsp[-4].montar); }
#line 2407 "parser.cpp"
    break;

  case 77:
#line 300 "parser.y"
                                                          { (yyvsp[-4].montar)->name=(yyvsp[0].TEXT); (yyval.montar)=(yyvsp[-4].montar);  }
#line 2413 "parser.cpp"
    break;

  case 78:
#line 301 "parser.y"
                                                   { (yyvsp[-4].montar)->name=(yyvsp[0].TEXT); (yyval.montar)=(yyvsp[-4].montar);  }
#line 2419 "parser.cpp"
    break;

  case 79:
#line 303 "parser.y"
                                       { obmount *disco = new obmount(); disco->path=(yyvsp[0].TEXT); (yyval.montar)=disco; }
#line 2425 "parser.cpp"
    break;

  case 80:
#line 304 "parser.y"
                                     { obmount *disco = new obmount(); disco->path=(yyvsp[0].TEXT); (yyval.montar)=disco; }
#line 2431 "parser.cpp"
    break;

  case 81:
#line 305 "parser.y"
                                              { obmount *disco = new obmount(); disco->name=(yyvsp[0].TEXT); (yyval.montar)=disco;  }
#line 2437 "parser.cpp"
    break;

  case 82:
#line 306 "parser.y"
                                       { obmount *disco = new obmount(); disco->name=(yyvsp[0].TEXT); (yyval.montar)=disco;  }
#line 2443 "parser.cpp"
    break;

  case 83:
#line 310 "parser.y"
                     { (yyvsp[0].mkfs)->mostrardatos((yyvsp[0].mkfs)); }
#line 2449 "parser.cpp"
    break;

  case 84:
#line 315 "parser.y"
                                                            { (yyvsp[-4].mkfs)->id=(yyvsp[0].TEXT); (yyval.mkfs)=(yyvsp[-4].mkfs); }
#line 2455 "parser.cpp"
    break;

  case 85:
#line 316 "parser.y"
                                                            { (yyvsp[-4].mkfs)->type=(yyvsp[0].TEXT); (yyval.mkfs)=(yyvsp[-4].mkfs); }
#line 2461 "parser.cpp"
    break;

  case 86:
#line 317 "parser.y"
                                                        { (yyvsp[-4].mkfs)->fs=(yyvsp[0].TEXT); (yyval.mkfs)=(yyvsp[-4].mkfs); }
#line 2467 "parser.cpp"
    break;

  case 87:
#line 318 "parser.y"
                                                        { (yyvsp[-4].mkfs)->fs=(yyvsp[0].TEXT); (yyval.mkfs)=(yyvsp[-4].mkfs); }
#line 2473 "parser.cpp"
    break;

  case 88:
#line 320 "parser.y"
                                                { obmkfs *disco = new obmkfs(); disco->id=(yyvsp[0].TEXT); (yyval.mkfs)=disco; }
#line 2479 "parser.cpp"
    break;

  case 89:
#line 321 "parser.y"
                                                { obmkfs *disco = new obmkfs(); disco->type=(yyvsp[0].TEXT); (yyval.mkfs)=disco; }
#line 2485 "parser.cpp"
    break;

  case 90:
#line 322 "parser.y"
                                           { obmkfs *disco = new obmkfs(); disco->fs=(yyvsp[0].TEXT); (yyval.mkfs)=disco; }
#line 2491 "parser.cpp"
    break;

  case 91:
#line 323 "parser.y"
                                           { obmkfs *disco = new obmkfs(); disco->fs=(yyvsp[0].TEXT); (yyval.mkfs)=disco; }
#line 2497 "parser.cpp"
    break;

  case 92:
#line 327 "parser.y"
                        {(yyvsp[0].touch)->mostrardatos((yyvsp[0].touch)); }
#line 2503 "parser.cpp"
    break;

  case 93:
#line 331 "parser.y"
                                                    { (yyvsp[-4].touch)->path=(yyvsp[0].TEXT); (yyval.touch)=(yyvsp[-4].touch); }
#line 2509 "parser.cpp"
    break;

  case 94:
#line 332 "parser.y"
                                                    { (yyvsp[-4].touch)->path=(yyvsp[0].TEXT); (yyval.touch)=(yyvsp[-4].touch); }
#line 2515 "parser.cpp"
    break;

  case 95:
#line 333 "parser.y"
                                                    { (yyvsp[-2].touch)->r=true; (yyval.touch)=(yyvsp[-2].touch); }
#line 2521 "parser.cpp"
    break;

  case 96:
#line 334 "parser.y"
                                                    { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-4].touch)->size=tam; (yyval.touch)=(yyvsp[-4].touch); }
#line 2527 "parser.cpp"
    break;

  case 97:
#line 335 "parser.y"
                                                    { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-4].touch)->size=tam; (yyval.touch)=(yyvsp[-4].touch); }
#line 2533 "parser.cpp"
    break;

  case 98:
#line 336 "parser.y"
                                                    { (yyvsp[-4].touch)->cont=(yyvsp[0].TEXT); (yyval.touch)=(yyvsp[-4].touch); }
#line 2539 "parser.cpp"
    break;

  case 99:
#line 337 "parser.y"
                                                    { (yyvsp[-4].touch)->cont=(yyvsp[0].TEXT); (yyval.touch)=(yyvsp[-4].touch); }
#line 2545 "parser.cpp"
    break;

  case 100:
#line 338 "parser.y"
                                                    { (yyvsp[-2].touch)->stdin=true; (yyval.touch)=(yyvsp[-2].touch);}
#line 2551 "parser.cpp"
    break;

  case 101:
#line 340 "parser.y"
                                        { obtouch *disco = new obtouch();  disco->path=(yyvsp[0].TEXT); (yyval.touch)=disco; }
#line 2557 "parser.cpp"
    break;

  case 102:
#line 341 "parser.y"
                                        { obtouch *disco = new obtouch();  disco->path=(yyvsp[0].TEXT); (yyval.touch)=disco; }
#line 2563 "parser.cpp"
    break;

  case 103:
#line 342 "parser.y"
                                        { obtouch *disco = new obtouch();  disco->r=true; (yyval.touch)=disco; }
#line 2569 "parser.cpp"
    break;

  case 104:
#line 343 "parser.y"
                                        { int tam=atoi((yyvsp[0].TEXT)); obtouch *disco = new obtouch();  disco->size=tam; (yyval.touch)=disco; }
#line 2575 "parser.cpp"
    break;

  case 105:
#line 344 "parser.y"
                                        { int tam=atoi((yyvsp[0].TEXT)); obtouch *disco = new obtouch();  disco->size=tam; (yyval.touch)=disco; }
#line 2581 "parser.cpp"
    break;

  case 106:
#line 345 "parser.y"
                                        { obtouch *disco = new obtouch();  disco->cont=(yyvsp[0].TEXT); (yyval.touch)=disco; }
#line 2587 "parser.cpp"
    break;

  case 107:
#line 346 "parser.y"
                                        { obtouch *disco = new obtouch();  disco->cont=(yyvsp[0].TEXT); (yyval.touch)=disco; }
#line 2593 "parser.cpp"
    break;

  case 108:
#line 347 "parser.y"
                                        { obtouch *disco = new obtouch();  disco->stdin=true; (yyval.touch)=disco; }
#line 2599 "parser.cpp"
    break;

  case 109:
#line 351 "parser.y"
                    {(yyvsp[0].rep)->mostrardatos((yyvsp[0].rep)); }
#line 2605 "parser.cpp"
    break;

  case 110:
#line 355 "parser.y"
                                                   { (yyvsp[-4].rep)->id=(yyvsp[0].TEXT); (yyval.rep)=(yyvsp[-4].rep); }
#line 2611 "parser.cpp"
    break;

  case 111:
#line 356 "parser.y"
                                                 { (yyvsp[-4].rep)->path=(yyvsp[0].TEXT); (yyval.rep)=(yyvsp[-4].rep); }
#line 2617 "parser.cpp"
    break;

  case 112:
#line 357 "parser.y"
                                               { (yyvsp[-4].rep)->path=(yyvsp[0].TEXT); (yyval.rep)=(yyvsp[-4].rep); }
#line 2623 "parser.cpp"
    break;

  case 113:
#line 358 "parser.y"
                                                        { (yyvsp[-4].rep)->name=(yyvsp[0].TEXT); (yyval.rep)=(yyvsp[-4].rep);  }
#line 2629 "parser.cpp"
    break;

  case 114:
#line 359 "parser.y"
                                                 {int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-4].rep)->name=(yyvsp[0].TEXT); (yyval.rep)=(yyvsp[-4].rep);  }
#line 2635 "parser.cpp"
    break;

  case 115:
#line 360 "parser.y"
                                                   { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-4].rep)->root=tam; (yyval.rep)=(yyvsp[-4].rep);  }
#line 2641 "parser.cpp"
    break;

  case 116:
#line 361 "parser.y"
                                                 { int tam=atoi((yyvsp[0].TEXT)); (yyvsp[-4].rep)->root=tam; (yyval.rep)=(yyvsp[-4].rep);  }
#line 2647 "parser.cpp"
    break;

  case 117:
#line 362 "parser.y"
                                                 { (yyvsp[-4].rep)->ruta_file=(yyvsp[0].TEXT); (yyval.rep)=(yyvsp[-4].rep); }
#line 2653 "parser.cpp"
    break;

  case 118:
#line 363 "parser.y"
                                               { (yyvsp[-4].rep)->ruta_file=(yyvsp[0].TEXT); (yyval.rep)=(yyvsp[-4].rep); }
#line 2659 "parser.cpp"
    break;

  case 119:
#line 365 "parser.y"
                                         { obrep *disco = new obrep(); disco->id=(yyvsp[0].TEXT); (yyval.rep)=disco; }
#line 2665 "parser.cpp"
    break;

  case 120:
#line 366 "parser.y"
                                       { obrep *disco = new obrep(); disco->path=(yyvsp[0].TEXT); (yyval.rep)=disco; }
#line 2671 "parser.cpp"
    break;

  case 121:
#line 367 "parser.y"
                                     { obrep *disco = new obrep(); disco->path=(yyvsp[0].TEXT); (yyval.rep)=disco; }
#line 2677 "parser.cpp"
    break;

  case 122:
#line 368 "parser.y"
                                              { obrep *disco = new obrep(); disco->name=(yyvsp[0].TEXT); (yyval.rep)=disco;  }
#line 2683 "parser.cpp"
    break;

  case 123:
#line 369 "parser.y"
                                       { obrep *disco = new obrep(); disco->name=(yyvsp[0].TEXT); (yyval.rep)=disco;  }
#line 2689 "parser.cpp"
    break;

  case 124:
#line 370 "parser.y"
                                         { int tam=atoi((yyvsp[0].TEXT)); obrep *disco = new obrep(); disco->root=tam; (yyval.rep)=disco; }
#line 2695 "parser.cpp"
    break;

  case 125:
#line 371 "parser.y"
                                       { int tam=atoi((yyvsp[0].TEXT)); obrep *disco = new obrep(); disco->root=tam; (yyval.rep)=disco; }
#line 2701 "parser.cpp"
    break;

  case 126:
#line 372 "parser.y"
                                       { obrep *disco = new obrep(); disco->ruta_file=(yyvsp[0].TEXT); (yyval.rep)=disco; }
#line 2707 "parser.cpp"
    break;

  case 127:
#line 373 "parser.y"
                                     { obrep *disco = new obrep(); disco->ruta_file=(yyvsp[0].TEXT); (yyval.rep)=disco; }
#line 2713 "parser.cpp"
    break;

  case 128:
#line 378 "parser.y"
                          {(yyvsp[0].mkdir)->mostrardatos((yyvsp[0].mkdir)); }
#line 2719 "parser.cpp"
    break;

  case 129:
#line 382 "parser.y"
                                               { (yyvsp[-2].mkdir)->p_entrada=(yyvsp[0].TEXT); (yyvsp[-2].mkdir)->r=true;  (yyval.mkdir)=(yyvsp[-2].mkdir); }
#line 2725 "parser.cpp"
    break;

  case 130:
#line 383 "parser.y"
                                                 { (yyvsp[-4].mkdir)->path=(yyvsp[0].TEXT); (yyval.mkdir)=(yyvsp[-4].mkdir); }
#line 2731 "parser.cpp"
    break;

  case 131:
#line 384 "parser.y"
                                               { (yyvsp[-4].mkdir)->path=(yyvsp[0].TEXT); (yyval.mkdir)=(yyvsp[-4].mkdir); }
#line 2737 "parser.cpp"
    break;

  case 132:
#line 386 "parser.y"
                         { obmkdir *disco = new obmkdir(); disco->p_entrada=(yyvsp[0].TEXT);  disco->r=true;  (yyval.mkdir)=disco; }
#line 2743 "parser.cpp"
    break;

  case 133:
#line 387 "parser.y"
                                     { obmkdir *disco = new obmkdir(); disco->path=(yyvsp[0].TEXT); (yyval.mkdir)=disco; }
#line 2749 "parser.cpp"
    break;

  case 134:
#line 388 "parser.y"
                                   { obmkdir *disco = new obmkdir(); disco->path=(yyvsp[0].TEXT); (yyval.mkdir)=disco; }
#line 2755 "parser.cpp"
    break;

  case 135:
#line 392 "parser.y"
                   { (yyvsp[0].cat)->mostrardatos((yyvsp[0].cat)); }
#line 2761 "parser.cpp"
    break;

  case 136:
#line 396 "parser.y"
                                                         { (yyvsp[-4].cat)->path.push_back((yyvsp[0].TEXT)); (yyval.cat)=(yyvsp[-4].cat); }
#line 2767 "parser.cpp"
    break;

  case 137:
#line 397 "parser.y"
                                                       { (yyvsp[-4].cat)->path.push_back((yyvsp[0].TEXT)); (yyval.cat)=(yyvsp[-4].cat); }
#line 2773 "parser.cpp"
    break;

  case 138:
#line 399 "parser.y"
                                               { obcat *disco = new obcat();  disco->path.clear(); disco->path.push_back((yyvsp[0].TEXT)); (yyval.cat)=disco; }
#line 2779 "parser.cpp"
    break;

  case 139:
#line 400 "parser.y"
                                             { obcat *disco = new obcat();  disco->path.clear(); disco->path.push_back((yyvsp[0].TEXT)); (yyval.cat)=disco; }
#line 2785 "parser.cpp"
    break;

  case 140:
#line 404 "parser.y"
                 { (yyvsp[0].ren)->mostrardatos((yyvsp[0].ren)); }
#line 2791 "parser.cpp"
    break;

  case 141:
#line 408 "parser.y"
                                               { (yyvsp[-4].ren)->path=(yyvsp[0].TEXT); (yyval.ren)=(yyvsp[-4].ren); }
#line 2797 "parser.cpp"
    break;

  case 142:
#line 409 "parser.y"
                                             { (yyvsp[-4].ren)->path=(yyvsp[0].TEXT); (yyval.ren)=(yyvsp[-4].ren); }
#line 2803 "parser.cpp"
    break;

  case 143:
#line 410 "parser.y"
                                                      { (yyvsp[-4].ren)->name=(yyvsp[0].TEXT); (yyval.ren)=(yyvsp[-4].ren);  }
#line 2809 "parser.cpp"
    break;

  case 144:
#line 411 "parser.y"
                                               { (yyvsp[-4].ren)->name=(yyvsp[0].TEXT); (yyval.ren)=(yyvsp[-4].ren);  }
#line 2815 "parser.cpp"
    break;

  case 145:
#line 413 "parser.y"
                                     { obren *disco = new obren(); disco->path=(yyvsp[0].TEXT); (yyval.ren)=disco; }
#line 2821 "parser.cpp"
    break;

  case 146:
#line 414 "parser.y"
                                   { obren *disco = new obren(); disco->path=(yyvsp[0].TEXT); (yyval.ren)=disco; }
#line 2827 "parser.cpp"
    break;

  case 147:
#line 415 "parser.y"
                                            { obren *disco = new obren(); disco->name=(yyvsp[0].TEXT); (yyval.ren)=disco;  }
#line 2833 "parser.cpp"
    break;

  case 148:
#line 416 "parser.y"
                                     { obren *disco = new obren(); disco->name=(yyvsp[0].TEXT); (yyval.ren)=disco;  }
#line 2839 "parser.cpp"
    break;

  case 149:
#line 421 "parser.y"
                                           { obrm *disco = new obrm(); disco->path=(yyvsp[0].TEXT); disco->mostrardatos(disco);}
#line 2845 "parser.cpp"
    break;

  case 150:
#line 422 "parser.y"
                                         { obrm *disco = new obrm(); disco->path=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 2851 "parser.cpp"
    break;

  case 151:
#line 426 "parser.y"
                     { (yyvsp[0].edit)->mostrardatos((yyvsp[0].edit)); }
#line 2857 "parser.cpp"
    break;

  case 152:
#line 430 "parser.y"
                                                     { (yyvsp[-4].edit)->path=(yyvsp[0].TEXT); (yyval.edit)=(yyvsp[-4].edit); }
#line 2863 "parser.cpp"
    break;

  case 153:
#line 431 "parser.y"
                                                   { (yyvsp[-4].edit)->path=(yyvsp[0].TEXT); (yyval.edit)=(yyvsp[-4].edit); }
#line 2869 "parser.cpp"
    break;

  case 154:
#line 432 "parser.y"
                                                   { (yyvsp[-4].edit)->cont=(yyvsp[0].TEXT); (yyval.edit)=(yyvsp[-4].edit); }
#line 2875 "parser.cpp"
    break;

  case 155:
#line 433 "parser.y"
                                                   { (yyvsp[-4].edit)->cont=(yyvsp[0].TEXT); (yyval.edit)=(yyvsp[-4].edit); }
#line 2881 "parser.cpp"
    break;

  case 156:
#line 434 "parser.y"
                                                   { (yyvsp[-2].edit)->stdin=true; (yyval.edit)=(yyvsp[-2].edit);}
#line 2887 "parser.cpp"
    break;

  case 157:
#line 436 "parser.y"
                                        { obedit *disco = new obedit();  disco->path=(yyvsp[0].TEXT); (yyval.edit)=disco; }
#line 2893 "parser.cpp"
    break;

  case 158:
#line 437 "parser.y"
                                        { obedit *disco = new obedit();  disco->path=(yyvsp[0].TEXT); (yyval.edit)=disco; }
#line 2899 "parser.cpp"
    break;

  case 159:
#line 438 "parser.y"
                                        { obedit *disco = new obedit();  disco->cont=(yyvsp[0].TEXT); (yyval.edit)=disco; }
#line 2905 "parser.cpp"
    break;

  case 160:
#line 439 "parser.y"
                                        { obedit *disco = new obedit();  disco->cont=(yyvsp[0].TEXT); (yyval.edit)=disco; }
#line 2911 "parser.cpp"
    break;

  case 161:
#line 440 "parser.y"
                                        { obedit *disco = new obedit();  disco->stdin=true; (yyval.edit)=disco; }
#line 2917 "parser.cpp"
    break;

  case 162:
#line 444 "parser.y"
                                            { obexec *disco = new obexec();  disco->path=(yyvsp[0].TEXT); disco->exec(); }
#line 2923 "parser.cpp"
    break;

  case 163:
#line 445 "parser.y"
                                            { obexec *disco = new obexec();  disco->path=(yyvsp[0].TEXT); disco->exec();  }
#line 2929 "parser.cpp"
    break;

  case 164:
#line 450 "parser.y"
                 { (yyvsp[0].copy)->mostrardatos((yyvsp[0].copy)); }
#line 2935 "parser.cpp"
    break;

  case 165:
#line 454 "parser.y"
                                                   { (yyvsp[-4].copy)->path=(yyvsp[0].TEXT); (yyval.copy)=(yyvsp[-4].copy); }
#line 2941 "parser.cpp"
    break;

  case 166:
#line 455 "parser.y"
                                                 { (yyvsp[-4].copy)->path=(yyvsp[0].TEXT); (yyval.copy)=(yyvsp[-4].copy); }
#line 2947 "parser.cpp"
    break;

  case 167:
#line 456 "parser.y"
                                                    { (yyvsp[-4].copy)->destino=(yyvsp[0].TEXT); (yyval.copy)=(yyvsp[-4].copy); }
#line 2953 "parser.cpp"
    break;

  case 168:
#line 457 "parser.y"
                                                 { (yyvsp[-4].copy)->destino=(yyvsp[0].TEXT); (yyval.copy)=(yyvsp[-4].copy); }
#line 2959 "parser.cpp"
    break;

  case 169:
#line 459 "parser.y"
                                        { obcp *disco = new obcp();  disco->path=(yyvsp[0].TEXT); (yyval.copy)=disco; }
#line 2965 "parser.cpp"
    break;

  case 170:
#line 460 "parser.y"
                                        { obcp *disco = new obcp();  disco->path=(yyvsp[0].TEXT); (yyval.copy)=disco; }
#line 2971 "parser.cpp"
    break;

  case 171:
#line 461 "parser.y"
                                        { obcp *disco = new obcp();  disco->destino=(yyvsp[0].TEXT); (yyval.copy)=disco; }
#line 2977 "parser.cpp"
    break;

  case 172:
#line 462 "parser.y"
                                        { obcp *disco = new obcp();  disco->destino=(yyvsp[0].TEXT); (yyval.copy)=disco; }
#line 2983 "parser.cpp"
    break;

  case 173:
#line 467 "parser.y"
               { (yyvsp[0].mv)->mostrardatos((yyvsp[0].mv)); }
#line 2989 "parser.cpp"
    break;

  case 174:
#line 471 "parser.y"
                                                 { (yyvsp[-4].mv)->path=(yyvsp[0].TEXT); (yyval.mv)=(yyvsp[-4].mv); }
#line 2995 "parser.cpp"
    break;

  case 175:
#line 472 "parser.y"
                                               { (yyvsp[-4].mv)->path=(yyvsp[0].TEXT); (yyval.mv)=(yyvsp[-4].mv); }
#line 3001 "parser.cpp"
    break;

  case 176:
#line 473 "parser.y"
                                                 { (yyvsp[-4].mv)->destino=(yyvsp[0].TEXT); (yyval.mv)=(yyvsp[-4].mv); }
#line 3007 "parser.cpp"
    break;

  case 177:
#line 474 "parser.y"
                                               { (yyvsp[-4].mv)->destino=(yyvsp[0].TEXT); (yyval.mv)=(yyvsp[-4].mv); }
#line 3013 "parser.cpp"
    break;

  case 178:
#line 476 "parser.y"
                                      { obmv *disco = new obmv();  disco->path=(yyvsp[0].TEXT); (yyval.mv)=disco; }
#line 3019 "parser.cpp"
    break;

  case 179:
#line 477 "parser.y"
                                      { obmv *disco = new obmv();  disco->path=(yyvsp[0].TEXT); (yyval.mv)=disco; }
#line 3025 "parser.cpp"
    break;

  case 180:
#line 478 "parser.y"
                                      { obmv *disco = new obmv();  disco->destino=(yyvsp[0].TEXT); (yyval.mv)=disco; }
#line 3031 "parser.cpp"
    break;

  case 181:
#line 479 "parser.y"
                                      { obmv *disco = new obmv();  disco->destino=(yyvsp[0].TEXT); (yyval.mv)=disco; }
#line 3037 "parser.cpp"
    break;

  case 182:
#line 483 "parser.y"
                   { (yyvsp[0].find)->mostrardatos((yyvsp[0].find)); }
#line 3043 "parser.cpp"
    break;

  case 183:
#line 487 "parser.y"
                                                            { (yyvsp[-4].find)->path=(yyvsp[0].TEXT); (yyval.find)=(yyvsp[-4].find); }
#line 3049 "parser.cpp"
    break;

  case 184:
#line 488 "parser.y"
                                                            { (yyvsp[-4].find)->path=(yyvsp[0].TEXT); (yyval.find)=(yyvsp[-4].find); }
#line 3055 "parser.cpp"
    break;

  case 185:
#line 489 "parser.y"
                                                            { (yyvsp[-4].find)->caracter=(yyvsp[0].TEXT); (yyvsp[-4].find)->name=(yyvsp[0].TEXT); (yyval.find)=(yyvsp[-4].find); }
#line 3061 "parser.cpp"
    break;

  case 186:
#line 490 "parser.y"
                                                            { (yyvsp[-4].find)->caracter=(yyvsp[0].TEXT); (yyvsp[-4].find)->name=(yyvsp[0].TEXT); (yyval.find)=(yyvsp[-4].find); }
#line 3067 "parser.cpp"
    break;

  case 187:
#line 491 "parser.y"
                                                            { (yyvsp[-4].find)->caracter=(yyvsp[0].TEXT); (yyvsp[-4].find)->name=(yyvsp[0].TEXT); (yyval.find)=(yyvsp[-4].find); }
#line 3073 "parser.cpp"
    break;

  case 188:
#line 492 "parser.y"
                                                            { (yyvsp[-4].find)->caracter=(yyvsp[0].TEXT); (yyvsp[-4].find)->name=(yyvsp[0].TEXT); (yyval.find)=(yyvsp[-4].find); }
#line 3079 "parser.cpp"
    break;

  case 189:
#line 494 "parser.y"
                                                                { obfind *disco = new obfind();  disco->path=(yyvsp[0].TEXT); (yyval.find)=disco; }
#line 3085 "parser.cpp"
    break;

  case 190:
#line 495 "parser.y"
                                                                { obfind *disco = new obfind();  disco->path=(yyvsp[0].TEXT); (yyval.find)=disco; }
#line 3091 "parser.cpp"
    break;

  case 191:
#line 496 "parser.y"
                                                  { obfind *disco = new obfind();  disco->caracter=(yyvsp[0].TEXT); disco->name=(yyvsp[0].TEXT); (yyval.find)=disco; }
#line 3097 "parser.cpp"
    break;

  case 192:
#line 497 "parser.y"
                                                  { obfind *disco = new obfind();  disco->caracter=(yyvsp[0].TEXT); disco->name=(yyvsp[0].TEXT); (yyval.find)=disco; }
#line 3103 "parser.cpp"
    break;

  case 193:
#line 498 "parser.y"
                                           { obfind *disco = new obfind();  disco->caracter=(yyvsp[0].TEXT); disco->name=(yyvsp[0].TEXT); (yyval.find)=disco; }
#line 3109 "parser.cpp"
    break;

  case 194:
#line 499 "parser.y"
                                                    { obfind *disco = new obfind();  disco->caracter=(yyvsp[0].TEXT); disco->name=(yyvsp[0].TEXT); (yyval.find)=disco; }
#line 3115 "parser.cpp"
    break;

  case 195:
#line 503 "parser.y"
            {  system("read -p 'Presione Enter para continuar...' var"); }
#line 3121 "parser.cpp"
    break;

  case 196:
#line 507 "parser.y"
                        { (yyvsp[0].login)->mostrardatos((yyvsp[0].login));  }
#line 3127 "parser.cpp"
    break;

  case 197:
#line 511 "parser.y"
                                                            { (yyvsp[-4].login)->usuario=(yyvsp[0].TEXT); (yyval.login)=(yyvsp[-4].login); }
#line 3133 "parser.cpp"
    break;

  case 198:
#line 512 "parser.y"
                                                            { (yyvsp[-4].login)->usuario=(yyvsp[0].TEXT); (yyval.login)=(yyvsp[-4].login); }
#line 3139 "parser.cpp"
    break;

  case 199:
#line 513 "parser.y"
                                                            { (yyvsp[-4].login)->usuario=(yyvsp[0].TEXT); (yyval.login)=(yyvsp[-4].login); }
#line 3145 "parser.cpp"
    break;

  case 200:
#line 514 "parser.y"
                                                            { (yyvsp[-4].login)->usuario=(yyvsp[0].TEXT); (yyval.login)=(yyvsp[-4].login); }
#line 3151 "parser.cpp"
    break;

  case 201:
#line 515 "parser.y"
                                                            { (yyvsp[-4].login)->pasword=(yyvsp[0].TEXT); (yyval.login)=(yyvsp[-4].login); }
#line 3157 "parser.cpp"
    break;

  case 202:
#line 516 "parser.y"
                                                            { (yyvsp[-4].login)->pasword=(yyvsp[0].TEXT); (yyval.login)=(yyvsp[-4].login); }
#line 3163 "parser.cpp"
    break;

  case 203:
#line 517 "parser.y"
                                                            { (yyvsp[-4].login)->pasword=(yyvsp[0].TEXT); (yyval.login)=(yyvsp[-4].login); }
#line 3169 "parser.cpp"
    break;

  case 204:
#line 518 "parser.y"
                                                            { (yyvsp[-4].login)->pasword=(yyvsp[0].TEXT); (yyval.login)=(yyvsp[-4].login); }
#line 3175 "parser.cpp"
    break;

  case 205:
#line 519 "parser.y"
                                                            { (yyvsp[-4].login)->id_particion=(yyvsp[0].TEXT); (yyval.login)=(yyvsp[-4].login); }
#line 3181 "parser.cpp"
    break;

  case 206:
#line 521 "parser.y"
                                                    { oblogin *disco = new oblogin(); disco->usuario=(yyvsp[0].TEXT); (yyval.login)=disco;}
#line 3187 "parser.cpp"
    break;

  case 207:
#line 522 "parser.y"
                                                    { oblogin *disco = new oblogin(); disco->usuario=(yyvsp[0].TEXT); (yyval.login)=disco;}
#line 3193 "parser.cpp"
    break;

  case 208:
#line 523 "parser.y"
                                                    { oblogin *disco = new oblogin(); disco->usuario=(yyvsp[0].TEXT); (yyval.login)=disco;}
#line 3199 "parser.cpp"
    break;

  case 209:
#line 524 "parser.y"
                                                    { oblogin *disco = new oblogin(); disco->usuario=(yyvsp[0].TEXT); (yyval.login)=disco;}
#line 3205 "parser.cpp"
    break;

  case 210:
#line 525 "parser.y"
                                                    { oblogin *disco = new oblogin(); disco->pasword=(yyvsp[0].TEXT); (yyval.login)=disco;}
#line 3211 "parser.cpp"
    break;

  case 211:
#line 526 "parser.y"
                                                    { oblogin *disco = new oblogin(); disco->pasword=(yyvsp[0].TEXT); (yyval.login)=disco; }
#line 3217 "parser.cpp"
    break;

  case 212:
#line 527 "parser.y"
                                                    { oblogin *disco = new oblogin(); disco->pasword=(yyvsp[0].TEXT); (yyval.login)=disco;}
#line 3223 "parser.cpp"
    break;

  case 213:
#line 528 "parser.y"
                                                    { oblogin *disco = new oblogin(); disco->pasword=(yyvsp[0].TEXT); (yyval.login)=disco; }
#line 3229 "parser.cpp"
    break;

  case 214:
#line 529 "parser.y"
                                                    { oblogin *disco = new oblogin(); disco->id_particion=(yyvsp[0].TEXT); (yyval.login)=disco; }
#line 3235 "parser.cpp"
    break;

  case 215:
#line 533 "parser.y"
          { oblogin *disco = new oblogin(); disco->logout();  }
#line 3241 "parser.cpp"
    break;

  case 216:
#line 537 "parser.y"
                                                        { obmkgrp *disco = new obmkgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3247 "parser.cpp"
    break;

  case 217:
#line 538 "parser.y"
                                                        { obmkgrp *disco = new obmkgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3253 "parser.cpp"
    break;

  case 218:
#line 539 "parser.y"
                                                        { obmkgrp *disco = new obmkgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3259 "parser.cpp"
    break;

  case 219:
#line 540 "parser.y"
                                                        { obmkgrp *disco = new obmkgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3265 "parser.cpp"
    break;

  case 220:
#line 541 "parser.y"
                                                        { obmkgrp *disco = new obmkgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3271 "parser.cpp"
    break;

  case 221:
#line 545 "parser.y"
                                                        { obrmgrp *disco = new obrmgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3277 "parser.cpp"
    break;

  case 222:
#line 546 "parser.y"
                                                        { obrmgrp *disco = new obrmgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3283 "parser.cpp"
    break;

  case 223:
#line 547 "parser.y"
                                                        { obrmgrp *disco = new obrmgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3289 "parser.cpp"
    break;

  case 224:
#line 548 "parser.y"
                                                        { obrmgrp *disco = new obrmgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3295 "parser.cpp"
    break;

  case 225:
#line 549 "parser.y"
                                                        { obrmgrp *disco = new obrmgrp(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3301 "parser.cpp"
    break;

  case 226:
#line 553 "parser.y"
                       { (yyvsp[0].mkusr)->mostrardatos((yyvsp[0].mkusr)); }
#line 3307 "parser.cpp"
    break;

  case 227:
#line 556 "parser.y"
                                                            { (yyvsp[-4].mkusr)->name=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3313 "parser.cpp"
    break;

  case 228:
#line 557 "parser.y"
                                                            { (yyvsp[-4].mkusr)->name=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3319 "parser.cpp"
    break;

  case 229:
#line 558 "parser.y"
                                                            { (yyvsp[-4].mkusr)->name=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3325 "parser.cpp"
    break;

  case 230:
#line 559 "parser.y"
                                                            { (yyvsp[-4].mkusr)->name=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3331 "parser.cpp"
    break;

  case 231:
#line 560 "parser.y"
                                                            { (yyvsp[-4].mkusr)->name=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3337 "parser.cpp"
    break;

  case 232:
#line 561 "parser.y"
                                                           { (yyvsp[-4].mkusr)->password=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3343 "parser.cpp"
    break;

  case 233:
#line 562 "parser.y"
                                                           { (yyvsp[-4].mkusr)->password=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3349 "parser.cpp"
    break;

  case 234:
#line 563 "parser.y"
                                                           { (yyvsp[-4].mkusr)->password=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3355 "parser.cpp"
    break;

  case 235:
#line 564 "parser.y"
                                                           { (yyvsp[-4].mkusr)->password=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3361 "parser.cpp"
    break;

  case 236:
#line 565 "parser.y"
                                                           { (yyvsp[-4].mkusr)->password=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3367 "parser.cpp"
    break;

  case 237:
#line 566 "parser.y"
                                                           { (yyvsp[-4].mkusr)->password=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3373 "parser.cpp"
    break;

  case 238:
#line 567 "parser.y"
                                                           { (yyvsp[-4].mkusr)->password=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3379 "parser.cpp"
    break;

  case 239:
#line 568 "parser.y"
                                                           { (yyvsp[-4].mkusr)->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3385 "parser.cpp"
    break;

  case 240:
#line 569 "parser.y"
                                                           { (yyvsp[-4].mkusr)->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3391 "parser.cpp"
    break;

  case 241:
#line 570 "parser.y"
                                                           { (yyvsp[-4].mkusr)->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3397 "parser.cpp"
    break;

  case 242:
#line 571 "parser.y"
                                                           { (yyvsp[-4].mkusr)->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3403 "parser.cpp"
    break;

  case 243:
#line 572 "parser.y"
                                                           { (yyvsp[-4].mkusr)->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=(yyvsp[-4].mkusr); }
#line 3409 "parser.cpp"
    break;

  case 244:
#line 574 "parser.y"
                                                 { obmkusr *disco = new obmkusr(); disco->name=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3415 "parser.cpp"
    break;

  case 245:
#line 575 "parser.y"
                                                 { obmkusr *disco = new obmkusr(); disco->name=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3421 "parser.cpp"
    break;

  case 246:
#line 576 "parser.y"
                                                 { obmkusr *disco = new obmkusr(); disco->name=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3427 "parser.cpp"
    break;

  case 247:
#line 577 "parser.y"
                                                 { obmkusr *disco = new obmkusr(); disco->name=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3433 "parser.cpp"
    break;

  case 248:
#line 578 "parser.y"
                                                 { obmkusr *disco = new obmkusr(); disco->name=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3439 "parser.cpp"
    break;

  case 249:
#line 579 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->password=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3445 "parser.cpp"
    break;

  case 250:
#line 580 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->password=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3451 "parser.cpp"
    break;

  case 251:
#line 581 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->password=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3457 "parser.cpp"
    break;

  case 252:
#line 582 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->password=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3463 "parser.cpp"
    break;

  case 253:
#line 583 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->password=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3469 "parser.cpp"
    break;

  case 254:
#line 584 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->password=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3475 "parser.cpp"
    break;

  case 255:
#line 585 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->password=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3481 "parser.cpp"
    break;

  case 256:
#line 586 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3487 "parser.cpp"
    break;

  case 257:
#line 587 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3493 "parser.cpp"
    break;

  case 258:
#line 588 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3499 "parser.cpp"
    break;

  case 259:
#line 589 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3505 "parser.cpp"
    break;

  case 260:
#line 590 "parser.y"
                                                { obmkusr *disco = new obmkusr(); disco->grupo=(yyvsp[0].TEXT); (yyval.mkusr)=disco; }
#line 3511 "parser.cpp"
    break;

  case 261:
#line 594 "parser.y"
                                                        { obrmusr *disco = new obrmusr(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3517 "parser.cpp"
    break;

  case 262:
#line 595 "parser.y"
                                                        { obrmusr *disco = new obrmusr(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3523 "parser.cpp"
    break;

  case 263:
#line 596 "parser.y"
                                                        { obrmusr *disco = new obrmusr(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco);}
#line 3529 "parser.cpp"
    break;

  case 264:
#line 597 "parser.y"
                                                        { obrmusr *disco = new obrmusr(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3535 "parser.cpp"
    break;

  case 265:
#line 598 "parser.y"
                                                        { obrmusr *disco = new obrmusr(); disco->name=(yyvsp[0].TEXT); disco->mostrardatos(disco); }
#line 3541 "parser.cpp"
    break;

  case 266:
#line 602 "parser.y"
                     { (yyvsp[0].chgrp)->mostrardatos((yyvsp[0].chgrp)); }
#line 3547 "parser.cpp"
    break;

  case 267:
#line 605 "parser.y"
                                                          { (yyvsp[-4].chgrp)->name=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3553 "parser.cpp"
    break;

  case 268:
#line 606 "parser.y"
                                                          { (yyvsp[-4].chgrp)->name=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3559 "parser.cpp"
    break;

  case 269:
#line 607 "parser.y"
                                                          { (yyvsp[-4].chgrp)->name=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3565 "parser.cpp"
    break;

  case 270:
#line 608 "parser.y"
                                                          { (yyvsp[-4].chgrp)->name=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3571 "parser.cpp"
    break;

  case 271:
#line 609 "parser.y"
                                                          { (yyvsp[-4].chgrp)->name=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3577 "parser.cpp"
    break;

  case 272:
#line 610 "parser.y"
                                                         { (yyvsp[-4].chgrp)->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3583 "parser.cpp"
    break;

  case 273:
#line 611 "parser.y"
                                                         { (yyvsp[-4].chgrp)->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3589 "parser.cpp"
    break;

  case 274:
#line 612 "parser.y"
                                                         { (yyvsp[-4].chgrp)->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3595 "parser.cpp"
    break;

  case 275:
#line 613 "parser.y"
                                                         { (yyvsp[-4].chgrp)->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3601 "parser.cpp"
    break;

  case 276:
#line 614 "parser.y"
                                                         { (yyvsp[-4].chgrp)->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=(yyvsp[-4].chgrp); }
#line 3607 "parser.cpp"
    break;

  case 277:
#line 616 "parser.y"
                                               { obchgrp *disco = new obchgrp(); disco->name=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3613 "parser.cpp"
    break;

  case 278:
#line 617 "parser.y"
                                               { obchgrp *disco = new obchgrp(); disco->name=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3619 "parser.cpp"
    break;

  case 279:
#line 618 "parser.y"
                                               { obchgrp *disco = new obchgrp(); disco->name=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3625 "parser.cpp"
    break;

  case 280:
#line 619 "parser.y"
                                               { obchgrp *disco = new obchgrp(); disco->name=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3631 "parser.cpp"
    break;

  case 281:
#line 620 "parser.y"
                                               { obchgrp *disco = new obchgrp(); disco->name=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3637 "parser.cpp"
    break;

  case 282:
#line 621 "parser.y"
                                              { obchgrp *disco = new obchgrp(); disco->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3643 "parser.cpp"
    break;

  case 283:
#line 622 "parser.y"
                                              { obchgrp *disco = new obchgrp(); disco->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3649 "parser.cpp"
    break;

  case 284:
#line 623 "parser.y"
                                              { obchgrp *disco = new obchgrp(); disco->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3655 "parser.cpp"
    break;

  case 285:
#line 624 "parser.y"
                                              { obchgrp *disco = new obchgrp(); disco->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3661 "parser.cpp"
    break;

  case 286:
#line 625 "parser.y"
                                              { obchgrp *disco = new obchgrp(); disco->grupo=(yyvsp[0].TEXT); (yyval.chgrp)=disco; }
#line 3667 "parser.cpp"
    break;

  case 287:
#line 629 "parser.y"
                                               { obrecovery *disco = new obrecovery(); disco->id=(yyvsp[0].TEXT); disco->mostrardatos(disco); disco->recovery(); }
#line 3673 "parser.cpp"
    break;

  case 288:
#line 630 "parser.y"
                                               { obrecovery *disco = new obrecovery(); disco->id=(yyvsp[0].TEXT); disco->mostrardatos(disco); disco->recovery(); }
#line 3679 "parser.cpp"
    break;

  case 289:
#line 631 "parser.y"
                                           { obrecovery *disco = new obrecovery(); disco->id=(yyvsp[0].TEXT); disco->mostrardatos(disco); disco->simulateloss(); }
#line 3685 "parser.cpp"
    break;

  case 290:
#line 632 "parser.y"
                                           { obrecovery *disco = new obrecovery(); disco->id=(yyvsp[0].TEXT); disco->mostrardatos(disco); disco->simulateloss(); }
#line 3691 "parser.cpp"
    break;


#line 3695 "parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
