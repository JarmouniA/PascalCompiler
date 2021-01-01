/**
GRAMMAR for Pascal Language

PROGRAM     ::= program ID ; BLOCK .
BLOCK       ::= CONSTS TYPES VARS FUNCTS PRCDS INSTS
CONSTS      ::= const ID = {NUM | STRVAL | CHARVAL | BOOLVAL} ; | e
TYPES       ::= type ID = array [ NUM .. {ID | NUM} ] of TYPE ; | e
VARS        ::= var {DCLR | INIT} ; {DCLR | INIT ;} | e
DCLR        ::= ID { , ID} : TYPE
INIT        ::= ID : TYPE = {NUM | STRVAL | CHARVAL | BOOLVAL}
TYPE        ::= integer | boolean | string | real | char | ID
FUNCTS      ::= FUNCT ; {FUNCT ; | e} | e
FUNCT       ::= function ID ( DCLR {; DCLR} ) : TYPE ; BLOCK
PRCDS       ::= PRCD ; {PRCD ;} | e
PRCD        ::= procedure ID {( {var | e} DCLR {; DCLR} ) | e} ; BLOCK
INSTS       ::= begin INST {; INST} end
INST        ::= INSTS | ASSIGN | EXECUTE | IF | WHILE | FOR | CASE | REPEAT | WRITE | WRITELN | READ | READLN | e
ASSIGN      ::= VAR := EXPR
VAR         ::= ID {[ EXPR ] | e}
EXECUTE     ::= ID ( EXPR {, EXPR} )
IF          ::= if COND then INST {else INST | e}
WHILE       ::= while COND do INST
FOR         ::= for ID := EXPR {to | downto} NUM do INST
CASE        ::= case ID of NUM : INST {NUM : INST} {else INST | e} end
REPEAT      ::= repeat INST until COND
WRITE       ::= write ( EXPR {, EXPR} )
WRITELN     ::= writeln ( EXPR {, EXPR} )
READ        ::= read ( VAR {, VAR} )
READLN      ::= readln ( VAR {, VAR} )
COND        ::= {( | e} {EXPR | e} CONDOP EXPR {) | e} {{and | or} COND | e}
CONDOP      ::= = | <> | < | > | <= | >= | not
EXPR        ::= TERM { ADDOP TERM } | BOOLVAL | STRVAL | CHARVAL
ADDOP       ::= + | -
TERM        ::= FACTOR { MULOP FACTOR }
MULOP       ::= * | /
FACTOR      ::= ID | NUM | ( EXPR ) | ID [ EXPR ] | EXECUTE
ID          ::= letter {letter | digit}
NUM         ::= digit {digit}
STRVAL      ::= ' letter | digit {letter | digit} '
CHARVAL     ::= ' { letter | digit } '
BOOLVAL        ::= true | false
digit       ::= 0|..|9
letter      ::= a|b|..|z|A|..|Z
*/

#ifndef SYNTAX_ANALYZER_H_INCLUDED
#define SYNTAX_ANALYZER_H_INCLUDED

    int currTokenNbr;
    char curr_block[lenValue];
    extern void SemanticError(int, char*);
    extern bool isConstant(Token);
    void WriteCurrTokenToFile();
    Token next_token();
    Token previous_token();
    bool verifyToken(token_code);
    void SyntaxError(token_code, char*);
    void Program();
    void Block();
    void Consts();
    void Types();
    bool isType();
    bool isNewType();
    void Vars();
    void Dclr();
    void Init();
    void Functs();
    void Prcds();
    void Insts();
    bool Inst();
    void Assign_Execute();
    void If();
    void While();
    void For();
    void Case();
    void Repeat();
    void Write();
    void Read();
    void Cond();
    bool CondOp();
    bool Expr();
    bool AddOp();
    bool Term();
    bool MulOp();
    bool Factor();
    bool isBool();

#endif // SYNTAX_ANALYZER_H_INCLUDED
