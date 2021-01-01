#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED

    /** comments in Pascal
        (* This is a multi-line comments
            and it will span multiple lines. *)

        { This is a single line comment in pascal }
    */

    #define MAXDIGIT    11
    #define MAXCHAR     20

    char curr_char, prev_char;
    int curr_line, curr_column;

    const char* keywords_list[] =
        {"PROGRAM", "BEGIN", "END", "CONST" , "VAR", "IF", "THEN", "ELSE",
        "WHILE", "DO", "READ", "WRITE", "AND", "OR", "NOT", "WRITELN", "READLN",
        "ROUND", "INTEGER" , "BOOLEAN" , "STRING", "REAL" , "CHAR", "TYPE",
        "PROCEDURE", "FUNCTION", "FOR", "CASE", "REPEAT", "UNTIL", "TRUE", "FALSE",
        "TO", "DOWNTO", "OF", "ARRAY", "MOD"};

    const char* specialTokens_list[] =
        {"SC", "PT",  "PLUS", "MINUS", "MULT", "DIV", "COMMA",
        "EQ", "ASSIGN", "INF",  "INFEQ", "SUP", "SUPEQ", "DIFF",
        "OP",  "CP", "OB", "CB", "AP", "CL"};

    const char* specialTokens_symb[] =
        {";", ".", "+", "-", "*", "/", "," , "=", ":=", "<", "<=",
        ">", ">=", "<>", "(", ")", "[", "]", "'", ":"};

    int keywords_list_size = sizeof(keywords_list) / sizeof(keywords_list[0]);
    int specialTokens_list_size = sizeof(specialTokens_list) / sizeof(specialTokens_list[0]);

    #define NBRKEYWORDS         keywords_list_size
    #define NBRSPECIALTOKENS    specialTokens_list_size

    void lexicalAnalysis();
    void WriteCurrTknToFile();
    char next_char();
    void getToken();
    void LexError(char*);
    void ignoreWhiteSpaces();
    bool isNumber();
    bool isWord();
    bool isSpecial();
    bool isChar();
    void String();

#endif // TOKENIZER_H_INCLUDED
