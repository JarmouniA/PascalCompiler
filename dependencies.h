#ifndef DEPENDENCIES_H_INCLUDED
#define DEPENDENCIES_H_INCLUDED

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    #include <stdbool.h>

    #define lenName     12
    #define lenValue    20
    #define lenIDValue    20 // MAXDIGIT=11     MAXCHAR=20

    #define MALLOC(ad, nb, type)\
        ad =(type*)malloc(nb*sizeof(type));\
        if (ad == NULL){\
            printf("Memory allocation is impossible");\
            exit(1);\
        }

    #define CALLOC(ad, nb, type)\
        ad =(type*)calloc(nb,sizeof(type));\
        if (ad == NULL){\
            printf("Memory Callocation is impossible");\
            exit(1);\
        }

    #define REALLOC(ad, NewSize, type)\
        ad = realloc(ad, NewSize * sizeof(type));\
        if (ad == NULL){\
            printf("Memory reallocation is impossible");\
            exit(1);\
        }

    FILE *prog_file, *out_file, *xml_file;
    char **Strings;
    int nbr_tokens, nbr_strs, nbr_IDs;

    typedef enum {
        // keywords tokens
        PROGRAM,  BEGIN,  END,  CONST,  VAR,  IF,  THEN, ELSE, WHILE,  DO,  READ,  WRITE,
        AND, OR, NOT, WRITELN, READLN, ROUND, INTEGER , BOOLEAN , STRING, REAL , CHAR,
        TYPE, PROCEDURE, FUNCTION, FOR, CASE, REPEAT, UNTIL, TRUE, FALSE, TO, DOWNTO, OF,
        ARRAY, MOD,
        // Symbols tokens
        SC,  PT,  PLUS,  MINUS,  MULT,  DIV,  COMMA,  EQ,  ASSIGN,  INF,  INFEQ,  SUP,
        SUPEQ, DIFF, OP, CP, OB, CB, AP, CL,
        // Other tokens
        ID, NUM, STRVAL, CHARVAL, EndOfFile
    } token_code;

    typedef struct {
        token_code code;
        char name[lenName];
        char value[lenValue];
        int line;
    } Token;

    Token curr_token, prev_token, *tokens;

    typedef enum {TPROG, TCONST, TTYPE, TVAR, TFCT, TPRCD} ID_type;

    typedef struct {
        char name[lenValue];
        ID_type type;
        char value[lenIDValue];
        int line;
        char block[lenValue];
    } ID_struct;

    ID_struct *IDs;// ,**IDs_blocks;

#endif // DEPENDENCIES_H_INCLUDED
