/* A Bison parser, made from parser.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	QSTRING	257
# define	ID	258
# define	COMMENT2	259
# define	FSM	260
# define	ENDF	261
# define	STATES	262
# define	WATCH	263
# define	ENDW	264
# define	STATE	265
# define	ENDS	266
# define	INITIALIZE	267
# define	ENDI	268
# define	IF	269
# define	ELSEIF	270
# define	ELSE	271
# define	ENDIF	272
# define	IFONCE	273
# define	ENDIFONCE	274
# define	NUMBER	275
# define	AND	276
# define	OR	277
# define	NOT	278
# define	TEAM	279
# define	SHIPS	280
# define	TEAMSHIPS	281
# define	SHIPSPOINT	282
# define	TEAMSPOINT	283
# define	VOLUMEPOINT	284
# define	PATH	285
# define	POINT	286
# define	VOLUME	287
# define	THISTEAM	288
# define	THISTEAMSHIPS	289
# define	THISTEAMSPOINT	290
# define	JUMP	291
# define	TRUE	292
# define	FALSE	293
# define	LT	294
# define	LTE	295
# define	EQ	296
# define	NEQ	297
# define	GTE	298
# define	GT	299
# define	FSMCREATE	300
# define	LOCALIZATION	301
# define	ENDL	302
# define	LSTRING	303
# define	BANG	304
# define	UMINUS	305

#line 1 "parser.y"


#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "Types.h"
#include "kas2c.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

int parseLevel = LEVEL_LEVEL;

int ifOnceIndex;  // up to 2^32 IFONCE statements per WATCH or INIT routine

extern FILE *yyout;
extern char *yytext;

extern int lineNum;
extern char curFilename[];


#line 28 "parser.y"
#ifndef YYSTYPE
typedef union {
    char    *string;    /* string buffer */
    int     number;     /* numeric value */
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		202
#define	YYFLAG		-32768
#define	YYNTBASE	62

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 305 ? yytranslate[x] : 126)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    61,     2,     2,     2,     2,     2,     2,
      59,    60,    52,    51,    58,    50,     2,    53,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    57,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    56,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    54,    55
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     5,     6,    10,    12,    14,    15,    18,    21,
      25,    27,    30,    31,    34,    41,    44,    46,    49,    50,
      51,    56,    58,    62,    63,    67,    69,    71,    72,    76,
      78,    80,    81,    84,    86,    89,    93,    95,    97,    99,
     100,   109,   114,   115,   116,   120,   121,   122,   123,   132,
     133,   134,   142,   145,   146,   147,   155,   156,   161,   162,
     167,   168,   173,   174,   179,   180,   185,   186,   191,   192,
     197,   198,   203,   204,   209,   210,   215,   216,   221,   222,
     227,   228,   232,   233,   237,   238,   242,   243,   248,   250,
     251,   257,   259,   261,   262,   264,   265,   270,   272,   274,
     278,   280,   284,   288,   292,   296,   300,   304,   308,   312,
     314,   316,   320,   322,   323,   326,   331,   334,   336
};
static const short yyrhs[] =
{
      63,    70,    77,    80,     0,     0,    64,    66,    65,     0,
      47,     0,    48,     0,     0,    66,    67,     0,    68,    69,
       0,    49,    56,     4,     0,     3,     0,    69,     3,     0,
       0,    70,    71,     0,    72,    74,    77,    80,   122,    73,
       0,     6,     4,     0,     7,     0,     7,     4,     0,     0,
       0,     8,    75,    76,    57,     0,     4,     0,    76,    58,
       4,     0,     0,    78,    83,    79,     0,    13,     0,    14,
       0,     0,    81,    83,    82,     0,     9,     0,    10,     0,
       0,    83,    84,     0,    57,     0,   100,    57,     0,    37,
       4,    57,     0,    87,     0,    96,     0,    85,     0,     0,
      46,    59,     4,    86,    58,   121,    60,    57,     0,    93,
      90,    88,    18,     0,     0,     0,    17,    89,    83,     0,
       0,     0,     0,    90,    16,    59,    91,   100,    60,    92,
      83,     0,     0,     0,    15,    59,    94,   100,    60,    95,
      83,     0,    97,    20,     0,     0,     0,    19,    59,    98,
     100,    60,    99,    83,     0,     0,   100,    51,   101,   100,
       0,     0,   100,    50,   102,   100,     0,     0,   100,    52,
     103,   100,     0,     0,   100,    53,   104,   100,     0,     0,
     100,    22,   105,   100,     0,     0,   100,    23,   106,   100,
       0,     0,   100,    42,   107,   100,     0,     0,   100,    43,
     108,   100,     0,     0,   100,    45,   109,   100,     0,     0,
     100,    44,   110,   100,     0,     0,   100,    40,   111,   100,
       0,     0,   100,    41,   112,   100,     0,     0,    50,   113,
     100,     0,     0,    61,   114,   100,     0,     0,    24,   115,
     100,     0,     0,    59,   116,   100,    60,     0,    21,     0,
       0,     4,   117,    59,   118,    60,     0,    38,     0,    39,
       0,     0,   120,     0,     0,   118,    58,   119,   120,     0,
     100,     0,     3,     0,    49,    56,     4,     0,   121,     0,
      26,    56,     4,     0,    31,    56,     4,     0,    32,    56,
       4,     0,    33,    56,     4,     0,    27,    56,     4,     0,
      28,    56,     4,     0,    29,    56,     4,     0,    30,    56,
       4,     0,    35,     0,    36,     0,    25,    56,     4,     0,
      34,     0,     0,   122,   123,     0,   124,    77,    80,   125,
       0,    11,     4,     0,    12,     0,    12,     4,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,    48,    51,    52,    55,    58,    61,    62,    69,    72,
      75,    76,    79,    80,    83,    86,    89,    90,    93,    94,
      94,    97,    98,   101,   102,   105,   108,   111,   112,   115,
     118,   126,   127,   130,   131,   132,   133,   134,   136,   140,
     140,   147,   150,   151,   151,   154,   155,   155,   155,   158,
     158,   158,   161,   164,   164,   164,   168,   168,   169,   169,
     170,   170,   171,   171,   172,   172,   173,   173,   174,   174,
     175,   175,   176,   176,   177,   177,   178,   178,   179,   179,
     180,   180,   181,   181,   182,   182,   183,   183,   184,   185,
     185,   186,   187,   190,   191,   192,   192,   195,   196,   198,
     200,   205,   207,   209,   211,   213,   215,   217,   219,   221,
     223,   227,   229,   233,   234,   237,   240,   243,   244
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "QSTRING", "ID", "COMMENT2", "FSM", "ENDF", 
  "STATES", "WATCH", "ENDW", "STATE", "ENDS", "INITIALIZE", "ENDI", "IF", 
  "ELSEIF", "ELSE", "ENDIF", "IFONCE", "ENDIFONCE", "NUMBER", "AND", "OR", 
  "NOT", "TEAM", "SHIPS", "TEAMSHIPS", "SHIPSPOINT", "TEAMSPOINT", 
  "VOLUMEPOINT", "PATH", "POINT", "VOLUME", "THISTEAM", "THISTEAMSHIPS", 
  "THISTEAMSPOINT", "JUMP", "TRUE", "FALSE", "LT", "LTE", "EQ", "NEQ", 
  "GTE", "GT", "FSMCREATE", "LOCALIZATION", "ENDL", "LSTRING", "'-'", 
  "'+'", "'*'", "'/'", "BANG", "UMINUS", "'_'", "';'", "','", "'('", 
  "')'", "'!'", "level", "localization", "localization_start", 
  "localization_end", "lstring_defs", "lstring_def", "lstring_def_name", 
  "lstring_value_list", "fsms", "fsm", "fsm_start", "fsm_end", 
  "state_list", "@1", "state_id_list", "initialize_block", 
  "initialize_start", "initialize_end", "watch_block", "watch_start", 
  "watch_end", "statements", "statement", "fsmcreate", "@2", 
  "ifstatement", "else", "@3", "elseif", "@4", "@5", "ifcondition", "@6", 
  "@7", "ifoncestatement", "ifoncecondition", "@8", "@9", "expression", 
  "@10", "@11", "@12", "@13", "@14", "@15", "@16", "@17", "@18", "@19", 
  "@20", "@21", "@22", "@23", "@24", "@25", "@26", "param_list", "@27", 
  "param", "paramteam", "states", "state", "state_start", "state_end", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    62,    63,    63,    64,    65,    66,    66,    67,    68,
      69,    69,    70,    70,    71,    72,    73,    73,    74,    75,
      74,    76,    76,    77,    77,    78,    79,    80,    80,    81,
      82,    83,    83,    84,    84,    84,    84,    84,    84,    86,
      85,    87,    88,    89,    88,    90,    91,    92,    90,    94,
      95,    93,    96,    98,    99,    97,   101,   100,   102,   100,
     103,   100,   104,   100,   105,   100,   106,   100,   107,   100,
     108,   100,   109,   100,   110,   100,   111,   100,   112,   100,
     113,   100,   114,   100,   115,   100,   116,   100,   100,   117,
     100,   100,   100,   118,   118,   119,   118,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   121,   121,   122,   122,   123,   124,   125,   125
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     4,     0,     3,     1,     1,     0,     2,     2,     3,
       1,     2,     0,     2,     6,     2,     1,     2,     0,     0,
       4,     1,     3,     0,     3,     1,     1,     0,     3,     1,
       1,     0,     2,     1,     2,     3,     1,     1,     1,     0,
       8,     4,     0,     0,     3,     0,     0,     0,     8,     0,
       0,     7,     2,     0,     0,     7,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     3,     0,     3,     0,     3,     0,     4,     1,     0,
       5,     1,     1,     0,     1,     0,     4,     1,     1,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     3,     1,     0,     2,     4,     2,     1,     2
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       2,     4,    12,     6,    23,     0,     0,    25,    13,    18,
      27,    31,     5,     0,     3,     7,     0,    15,    19,    23,
      29,     1,    31,     0,     0,    10,     8,     0,    27,     0,
      89,    26,     0,     0,    88,    84,     0,    91,    92,     0,
      80,    33,    86,    82,    24,    32,    38,    36,    45,    37,
       0,     0,     9,    11,    21,     0,   113,    30,    28,     0,
      49,    53,     0,     0,     0,     0,     0,     0,    42,    52,
      64,    66,    76,    78,    68,    70,    74,    72,    58,    56,
      60,    62,    34,    20,     0,     0,    93,     0,     0,    85,
      35,    39,    81,     0,    83,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,    16,     0,    14,   114,    23,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,   109,   110,     0,
      97,     0,    94,   100,     0,     0,     0,    87,    46,    31,
      41,    65,    67,    77,    79,    69,    71,    75,    73,    59,
      57,    61,    63,    17,   116,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,    90,    50,    54,
       0,     0,    44,     0,   111,   101,   105,   106,   107,   108,
     102,   103,   104,    99,     0,    31,    31,     0,     0,   117,
     115,    96,    51,    55,     0,    47,   118,    40,    31,    48,
       0,     0,     0
};

static const short yydefgoto[] =
{
     200,     2,     3,    14,     5,    15,    16,    26,     4,     8,
       9,   113,    19,    27,    55,    10,    11,    44,    21,    22,
      58,    23,    45,    46,   136,    47,    97,   139,    68,   171,
     198,    48,    87,   185,    49,    50,    88,   186,    51,   107,
     106,   108,   109,    98,    99,   102,   103,   105,   104,   100,
     101,    65,    67,    62,    66,    59,   131,   184,   132,   133,
      85,   114,   115,   190
};

static const short yypact[] =
{
     -43,-32768,-32768,-32768,     9,   -18,    20,-32768,-32768,    27,
      28,-32768,-32768,    10,-32768,-32768,    74,-32768,-32768,    58,
  -32768,-32768,-32768,    68,    76,-32768,    78,    80,    28,   111,
  -32768,-32768,    26,    32,-32768,-32768,    82,-32768,-32768,    34,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
      75,   259,-32768,-32768,-32768,   -41,-32768,-32768,-32768,    37,
  -32768,-32768,   116,    40,    90,   116,   116,   116,    35,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,    94,    18,    29,   116,   116,-32768,
  -32768,-32768,-32768,   163,-32768,    41,-32768,    83,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
  -32768,    98,    99,-32768,-32768,    58,-32768,    48,    52,    53,
      54,    56,    57,    60,    63,    66,-32768,-32768,-32768,    67,
     283,   -24,-32768,-32768,   187,   211,    73,-32768,-32768,-32768,
  -32768,   -32,   -32,    23,    23,    23,    23,    23,    23,    17,
      17,-32768,-32768,-32768,-32768,    28,   129,   130,   132,   134,
     135,   137,   138,   139,   140,   142,-32768,-32768,-32768,-32768,
     -11,   116,   141,   146,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,    29,-32768,-32768,    87,   235,   147,
  -32768,-32768,   141,   141,    95,-32768,-32768,-32768,-32768,   141,
     153,   159,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   -16,-32768,-32768,   -27,-32768,
  -32768,   -22,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -60,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -17,    -1,
  -32768,-32768,-32768,-32768
};


#define	YYLAST		336


static const short yytable[] =
{
      29,    56,    89,    28,     1,    92,    93,    94,    72,    73,
      74,    75,    76,    77,   117,     6,    83,    84,    78,    79,
      80,    81,     7,   126,    17,   111,   130,   134,   135,   112,
      12,    13,   116,    30,   166,    18,   167,    20,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      34,    95,    96,    35,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    24,    37,    38,    80,
      81,     7,    30,    78,    79,    80,    81,    25,   129,    40,
      52,    53,    31,    32,    54,    60,    63,    33,    42,    34,
      43,    61,    35,    64,    91,    69,    86,    90,   110,   155,
     138,   140,   153,   154,   156,    36,    37,    38,   157,   158,
     159,   188,   160,   161,    39,    30,   162,   172,    40,   163,
      30,    57,   164,   165,   130,    41,    32,    42,   173,    43,
      33,   170,    34,   174,   175,    35,   176,    34,   177,   178,
      35,   179,   180,   181,   182,    30,   183,   194,    36,    37,
      38,   196,   197,   201,    37,    38,    32,    39,   189,   202,
      33,    40,    34,   192,   193,    35,    40,   191,    41,   187,
      42,     0,    43,     0,     0,    42,   199,    43,    36,    37,
      38,     0,     0,     0,     0,    70,    71,    39,     0,     0,
       0,    40,     0,     0,     0,     0,     0,     0,    41,     0,
      42,     0,    43,    72,    73,    74,    75,    76,    77,    70,
      71,     0,     0,    78,    79,    80,    81,     0,     0,     0,
       0,     0,     0,   137,     0,     0,     0,    72,    73,    74,
      75,    76,    77,    70,    71,     0,     0,    78,    79,    80,
      81,     0,     0,     0,     0,     0,     0,   168,     0,     0,
       0,    72,    73,    74,    75,    76,    77,    70,    71,     0,
       0,    78,    79,    80,    81,     0,     0,     0,     0,     0,
       0,   169,     0,     0,     0,    72,    73,    74,    75,    76,
      77,    70,    71,     0,     0,    78,    79,    80,    81,     0,
       0,     0,     0,     0,     0,   195,     0,     0,     0,    72,
      73,    74,    75,    76,    77,    70,    71,     0,     0,    78,
      79,    80,    81,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,    78,    79,    80,    81
};

static const short yycheck[] =
{
      22,    28,    62,    19,    47,    65,    66,    67,    40,    41,
      42,    43,    44,    45,    25,     6,    57,    58,    50,    51,
      52,    53,    13,    34,     4,     7,    86,    87,    88,    11,
      48,    49,     3,     4,    58,     8,    60,     9,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
      21,    16,    17,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    56,    38,    39,    52,
      53,    13,     4,    50,    51,    52,    53,     3,    49,    50,
       4,     3,    14,    15,     4,    59,     4,    19,    59,    21,
      61,    59,    24,    59,     4,    20,    59,    57,     4,   115,
      59,    18,     4,     4,    56,    37,    38,    39,    56,    56,
      56,   171,    56,    56,    46,     4,    56,   139,    50,    56,
       4,    10,    56,    56,   184,    57,    15,    59,   155,    61,
      19,    58,    21,     4,     4,    24,     4,    21,     4,     4,
      24,     4,     4,     4,     4,     4,     4,    60,    37,    38,
      39,     4,    57,     0,    38,    39,    15,    46,    12,     0,
      19,    50,    21,   185,   186,    24,    50,   184,    57,   170,
      59,    -1,    61,    -1,    -1,    59,   198,    61,    37,    38,
      39,    -1,    -1,    -1,    -1,    22,    23,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,    61,    40,    41,    42,    43,    44,    45,    22,
      23,    -1,    -1,    50,    51,    52,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    40,    41,    42,
      43,    44,    45,    22,    23,    -1,    -1,    50,    51,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    40,    41,    42,    43,    44,    45,    22,    23,    -1,
      -1,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    40,    41,    42,    43,    44,
      45,    22,    23,    -1,    -1,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    40,
      41,    42,    43,    44,    45,    22,    23,    -1,    -1,    50,
      51,    52,    53,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    44,    45,    -1,
      -1,    -1,    -1,    50,    51,    52,    53
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 4:
#line 55 "parser.y"
{ kasLocalizationStart(); }
    break;
case 5:
#line 58 "parser.y"
{ kasLocalizationEnd(); }
    break;
case 8:
#line 69 "parser.y"
{ kasLStringDefineEnd(); }
    break;
case 9:
#line 72 "parser.y"
{ kasLStringDefineStart(yyvsp[0].string); }
    break;
case 10:
#line 75 "parser.y"
{ kasLStringValue(yyvsp[0].string); }
    break;
case 11:
#line 76 "parser.y"
{ kasLStringValue(yyvsp[0].string); }
    break;
case 15:
#line 86 "parser.y"
{ kasFSMStart(yyvsp[0].string); }
    break;
case 16:
#line 89 "parser.y"
{ kasFSMEnd(""); }
    break;
case 17:
#line 90 "parser.y"
{ kasFSMEnd(yyvsp[0].string); }
    break;
case 18:
#line 93 "parser.y"
{ kasStateListClear(); kasStateListEnd(); }
    break;
case 19:
#line 94 "parser.y"
{ kasStateListClear(); }
    break;
case 20:
#line 94 "parser.y"
{ kasStateListEnd(); }
    break;
case 21:
#line 97 "parser.y"
{ kasStateListAdd(yyvsp[0].string); }
    break;
case 22:
#line 98 "parser.y"
{ kasStateListAdd(yyvsp[0].string); }
    break;
case 23:
#line 101 "parser.y"
{ kasInitializeStart(); kasInitializeEnd(); }
    break;
case 25:
#line 105 "parser.y"
{ kasInitializeStart(); }
    break;
case 26:
#line 108 "parser.y"
{ kasInitializeEnd(); }
    break;
case 27:
#line 111 "parser.y"
{ kasWatchStart(); kasWatchEnd(); }
    break;
case 29:
#line 115 "parser.y"
{ kasWatchStart(); }
    break;
case 30:
#line 118 "parser.y"
{ kasWatchEnd(); }
    break;
case 33:
#line 130 "parser.y"
{ fprintf(yyout, ";\n\t"); }
    break;
case 34:
#line 131 "parser.y"
{ fprintf(yyout, ";\n\t"); }
    break;
case 35:
#line 132 "parser.y"
{ kasJump(yyvsp[-1].string); fprintf(yyout, ";\n\treturn;\n\t"); }
    break;
case 39:
#line 140 "parser.y"
{ kasFSMCreateStart(yyvsp[0].string); }
    break;
case 40:
#line 140 "parser.y"
{ kasFSMCreateEnd(); }
    break;
case 43:
#line 151 "parser.y"
{ fprintf(yyout, "else\n\t{\n\t"); }
    break;
case 44:
#line 151 "parser.y"
{ fprintf(yyout, "}\n\t"); }
    break;
case 46:
#line 155 "parser.y"
{ fprintf(yyout, "else if ("); }
    break;
case 47:
#line 155 "parser.y"
{ fprintf(yyout, ")\n\t{\n\t"); }
    break;
case 48:
#line 155 "parser.y"
{ fprintf(yyout, "}\n\t"); }
    break;
case 49:
#line 158 "parser.y"
{ fprintf(yyout, "if ("); }
    break;
case 50:
#line 158 "parser.y"
{ fprintf(yyout, ")\n\t{\n\t"); }
    break;
case 51:
#line 158 "parser.y"
{ fprintf(yyout, "}\n\t"); }
    break;
case 53:
#line 164 "parser.y"
{ fprintf(yyout, "if (!kasfVarValueGet(\"_VIFONCE%03d\") && (", ifOnceIndex+1); }
    break;
case 54:
#line 164 "parser.y"
{ fprintf(yyout, ")) // \"ifonce\" #%d\n\t{\n\tkasfVarCreateSet(\"_VIFONCE%03d\", 1);\n\t", ifOnceIndex+1, ifOnceIndex+1); }
    break;
case 55:
#line 164 "parser.y"
{ fprintf(yyout, "}\n\t"); ++ifOnceIndex; }
    break;
case 56:
#line 168 "parser.y"
{ fprintf(yyout, " + "); }
    break;
case 58:
#line 169 "parser.y"
{ fprintf(yyout, " - "); }
    break;
case 60:
#line 170 "parser.y"
{ fprintf(yyout, " * "); }
    break;
case 62:
#line 171 "parser.y"
{ fprintf(yyout, " / "); }
    break;
case 64:
#line 172 "parser.y"
{ fprintf(yyout, " && "); }
    break;
case 66:
#line 173 "parser.y"
{ fprintf(yyout, " || "); }
    break;
case 68:
#line 174 "parser.y"
{ fprintf(yyout, " == "); }
    break;
case 70:
#line 175 "parser.y"
{ fprintf(yyout, " != "); }
    break;
case 72:
#line 176 "parser.y"
{ fprintf(yyout, " > "); }
    break;
case 74:
#line 177 "parser.y"
{ fprintf(yyout, " >= "); }
    break;
case 76:
#line 178 "parser.y"
{ fprintf(yyout, " < "); }
    break;
case 78:
#line 179 "parser.y"
{ fprintf(yyout, " <= "); }
    break;
case 80:
#line 180 "parser.y"
{ fprintf(yyout, "-"); }
    break;
case 82:
#line 181 "parser.y"
{ fprintf(yyout, "!"); }
    break;
case 84:
#line 182 "parser.y"
{ fprintf(yyout, "!"); }
    break;
case 86:
#line 183 "parser.y"
{ fprintf(yyout, "("); }
    break;
case 87:
#line 183 "parser.y"
{ fprintf(yyout, ")"); }
    break;
case 88:
#line 184 "parser.y"
{ fprintf(yyout, "%d", yyvsp[0].number); }
    break;
case 89:
#line 185 "parser.y"
{ kasFunctionStart(yyvsp[0].string); }
    break;
case 90:
#line 185 "parser.y"
{ kasFunctionEnd(); }
    break;
case 91:
#line 186 "parser.y"
{ fprintf(yyout, "1"); }
    break;
case 92:
#line 187 "parser.y"
{ fprintf(yyout, "0"); }
    break;
case 95:
#line 192 "parser.y"
{ fprintf(yyout, ", "); }
    break;
case 97:
#line 195 "parser.y"
{ kasFunctionParamNumber(); }
    break;
case 98:
#line 196 "parser.y"
{ fprintf(yyout, "\"%s\"", yyvsp[0].string);
                         kasFunctionParamCharPtr(); }
    break;
case 99:
#line 198 "parser.y"
{ kasLStringReference(yyvsp[0].string);
                              kasFunctionParamCharPtr(); }
    break;
case 101:
#line 205 "parser.y"
{ fprintf(yyout, "kasGrowSelectionPtr(\"%s\")", yyvsp[0].string);
                             kasFunctionParamSelectCommandPtr(); }
    break;
case 102:
#line 207 "parser.y"
{ fprintf(yyout, "kasPathPtr(\"%s\")", yyvsp[0].string);
                             kasFunctionParamPathPtr(); }
    break;
case 103:
#line 209 "parser.y"
{ fprintf(yyout, "kasVectorPtr(\"%s\")", yyvsp[0].string);
                              kasFunctionParamVectorPtr(); }
    break;
case 104:
#line 211 "parser.y"
{ fprintf(yyout, "kasVolumePtr(\"%s\")", yyvsp[0].string);
                              kasFunctionParamVolumePtr(); }
    break;
case 105:
#line 213 "parser.y"
{ fprintf(yyout, "kasAITeamShipsPtr(\"%s\")", yyvsp[0].string);
                                  kasFunctionParamSelectCommandPtr(); }
    break;
case 106:
#line 215 "parser.y"
{ fprintf(yyout, "kasShipsVectorPtr(\"%s\")", yyvsp[0].string);
                                   kasFunctionParamVectorPtr(); }
    break;
case 107:
#line 217 "parser.y"
{ fprintf(yyout, "kasTeamsVectorPtr(\"%s\")", yyvsp[0].string);
                                   kasFunctionParamVectorPtr(); }
    break;
case 108:
#line 219 "parser.y"
{ fprintf(yyout, "kasVolumeVectorPtr(\"%s\")", yyvsp[0].string);
                                   kasFunctionParamVectorPtr(); }
    break;
case 109:
#line 221 "parser.y"
{ fprintf(yyout, "(&kasThisTeamPtr->shipList)");
                        kasFunctionParamSelectCommandPtr(); }
    break;
case 110:
#line 223 "parser.y"
{ fprintf(yyout, "kasThisTeamsVectorPtr()");
                                kasFunctionParamVectorPtr();  }
    break;
case 111:
#line 227 "parser.y"
{ fprintf(yyout, "kasAITeamPtr(\"%s\")", yyvsp[0].string);
                             kasFunctionParamAITeamPtr(); }
    break;
case 112:
#line 229 "parser.y"
{ fprintf(yyout, "kasThisTeamPtr");
                        kasFunctionParamAITeamPtr(); }
    break;
case 116:
#line 240 "parser.y"
{ kasStateStart(yyvsp[0].string); }
    break;
case 117:
#line 243 "parser.y"
{ kasStateEnd(""); }
    break;
case 118:
#line 244 "parser.y"
{ kasStateEnd(yyvsp[0].string); }
    break;
}

#line 705 "/usr/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 247 "parser.y"


int lineNum = 1;
char curFilename[256]; // current input filename
char levelName[MAX_LEVEL_NAME_LENGTH+1]; // current input filename
FILE *yyhout;

int yyerror (char *s)
{
    if (!strcasecmp(s, "parse error"))
        fprintf(stderr, "%s(%d) error : at '%s', %s\n", curFilename, lineNum, yytext, stateHelpGet());
    else
        fprintf(stderr, "%s(%d) error : at '%s', %s (%s)\n", curFilename, lineNum, yytext, stateHelpGet(), s);
}

char *levelNameGet(void)
{
    return levelName;
}

int lineNumGet(void)
{
    return lineNum;
}

char *curFilenameGet()
{
    return curFilename;
}

main(int argc, char **argv)
{
    char infilename[256], outfilename[256], houtfilename[256];
    char *shortinfilename, tempfilename[256];
    extern FILE *yyin, *yyout;
    int i;

    if (argc >= 2 && !strcasecmp(argv[1], "-F"))
    {
        kasHeaders(0);
        exit(1);
    }
    else if (argc >= 2 && !strcasecmp(argv[1], "-FC"))
    {
        kasHeaders(1);
        exit(1);
    }
    else if (argc != 4)
    {
        fprintf(stderr, "\nKAS2C Version %s\n", KAS2C_VERSION);
        fprintf(stderr, "Copyright (C) 1998 Relic Entertainment Inc.  All rights reserved.\n");
        fprintf(stderr, "\n Usage:\n\n");
        fprintf(stderr, " KAS2C mission.kas mission.c mission.h   generate mission.c and mission.h\n");
        fprintf(stderr, " KAS2C -f                                list function headers (english style)\n");
        fprintf(stderr, " KAS2C -fc                               list function headers (C-style)\n");
        exit(1);
    }

    strcpy(infilename, argv[1]);

    strcpy(outfilename, argv[2]);

    strcpy(houtfilename, argv[3]);

    if (!(yyin = fopen(infilename, "r")))
    {
        fprintf(stderr, "%s: can't open\n", infilename);
        exit(1);
    }

    if (!(yyout = fopen(outfilename, "w")))
    {
        fprintf(stderr, "%s: can't open\n", outfilename);
        exit(1);
    }

    if (!(yyhout = fopen(houtfilename, "w")))
    {
        fprintf(stderr, "%s: can't open\n", houtfilename);
        exit(1);
    }

    // hack off leading paths for display of infilename
    shortinfilename = infilename+strlen(infilename);
    while (shortinfilename > infilename)
    {
        --shortinfilename;
        if (*shortinfilename == '\\')
        {
            ++shortinfilename;
            break;
        }
    }
    fprintf(stderr, "%s\n", shortinfilename);

    strcpy(tempfilename, shortinfilename);
    strcpy(infilename, tempfilename);

    strcpy(levelName, shortinfilename);
    i = 0;
    while (i < strlen(levelName) && levelName[i] != '.')
        ++i;
    if (i < strlen(levelName))
        levelName[i] = 0;  // knock off extension

    strcpy(curFilename, infilename);

    fprintf(yyout, "//\n");
    fprintf(yyout, "//  %s\n", outfilename);
    fprintf(yyout, "//\n");
    fprintf(yyout, "//  Finite state machine routines for \"%s\" mission\n", levelName);
    fprintf(yyout, "//\n");
    fprintf(yyout, "//  Copyright (C) 1998 Relic Entertainment Inc.\n");
    fprintf(yyout, "//  All rights reserved\n");
    fprintf(yyout, "//\n");
    fprintf(yyout, "//  This code was autogenerated from %s by KAS2C Version %s\n",
        infilename, KAS2C_VERSION);
    fprintf(yyout, "//\n\n\n");
    fprintf(yyout, "#include \"%s\"  // prototypes and #includes for exposed game functions\n", houtfilename);
    fprintf(yyout, "\n");
    fprintf(yyout, "extern AITeam *CurrentTeamP;\n");
    fprintf(yyout, "#define kasThisTeamPtr CurrentTeamP\n");
    fprintf(yyout, "\n");
    fprintf(yyout, "//  for run-time scoping of symbols (variables, timers, etc.)\n");
    fprintf(yyout, "extern sdword CurrentMissionScope;\n");
    fprintf(yyout, "extern char CurrentMissionScopeName[];\n");
    fprintf(yyout, "\n");

    fprintf(yyout, "//  for displaying localized strings\n");
    fprintf(yyout, "extern udword strCurLanguage;\n");
    fprintf(yyout, "\n");

    fprintf(yyhout, "//\n");
    fprintf(yyhout, "//  %s\n", houtfilename);
    fprintf(yyhout, "//\n");
    fprintf(yyhout, "//  Finite state machine for \"%s\" mission\n", levelName);
    fprintf(yyhout, "//\n");
    fprintf(yyhout, "//  Copyright (C) 1998 Relic Entertainment Inc.\n");
    fprintf(yyhout, "//  All rights reserved\n");
    fprintf(yyhout, "//\n");
    fprintf(yyhout, "//  This code was autogenerated from %s by KAS2C Version %s\n",
        infilename, KAS2C_VERSION);
    fprintf(yyhout, "//\n\n\n");
    fprintf(yyhout, "#ifndef __%s_H\n", levelName);
    fprintf(yyhout, "#define __%s_H\n\n\n", levelName);

    // add any other required #includes here
    fprintf(yyhout, "//\n");
    fprintf(yyhout, "//  types and exposed game functions\n");
    fprintf(yyhout, "//\n");
    fprintf(yyhout, "#include <string.h>\n");
    fprintf(yyhout, "#include \"Types.h\"\n");
    fprintf(yyhout, "#include \"Vector.h\"\n");
    fprintf(yyhout, "#include \"AITeam.h\"\n");
    fprintf(yyhout, "#include \"AIMoves.h\"\n");
    fprintf(yyhout, "#include \"CommandWrap.h\"\n");
    fprintf(yyhout, "#include \"Timer.h\"\n");
    fprintf(yyhout, "#include \"Volume.h\"\n");
    fprintf(yyhout, "#include \"Objectives.h\"\n");
    fprintf(yyhout, "#include \"Attributes.h\"\n");
    fprintf(yyhout, "#include \"TradeMgr.h\"\n");

    fprintf(yyhout, "\n\n");
    fprintf(yyhout, "//\n");
    fprintf(yyhout, "//  FSM prototypes\n");
    fprintf(yyhout, "//\n");

    ifOnceIndex = 0;  // reset counter for each source file parsed

    // normal yyin and yyout interaction
    yyparse();

    fprintf(yyhout, "\n\n#endif\n");

    fclose(yyin);
    fclose(yyout);
    fclose(yyhout);

    // to be done: unlink output files on error?
    if (yynerrs)
    {
        unlink(outfilename);
        unlink(houtfilename);
    }

}
