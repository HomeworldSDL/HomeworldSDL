#ifndef BISON_PARSER_H
# define BISON_PARSER_H

#ifndef YYSTYPE
typedef union {
    char    *string;    /* string buffer */
    int     number;     /* numeric value */
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
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


extern YYSTYPE yylval;

#endif /* not BISON_PARSER_H */
