#include "dependencies.h"
#include "syntax_analyzer.h"

// Syntax Analysis: Parsing

bool verifyToken(token_code code_to_test){
    if ( (curr_token = next_token()).code != code_to_test ){
        previous_token();
        return false;
    }
    WriteCurrTokenToFile();
    return true;
}

Token previous_token(){
    return curr_token = tokens[--currTokenNbr];
}

Token next_token(){
    prev_token = curr_token;
    return curr_token = tokens[currTokenNbr++];
}

// Parse tree
void WriteCurrTokenToFile(){
    fprintf(xml_file,"{%s, %s}\n", curr_token.name, curr_token.value);
}

void SyntaxError(token_code code, char *message){
    printf("Syntax Error: Code %d\nLine %d: %s\n",
           code, curr_token.line, message);
}

void Program(){
    currTokenNbr = 0;
    MALLOC(IDs, 1, ID_struct);
    nbr_IDs = 1;
    fprintf(xml_file,"<PROGRAM>\n");
    bool r = verifyToken(PROGRAM);
    if(r){
        strcpy(IDs[nbr_IDs-1].name, curr_token.value);
        IDs[nbr_IDs-1].type = TPROG;
        IDs[nbr_IDs-1].line = curr_token.line;
        strcpy(IDs[nbr_IDs-1].block, "PROGRAM");
        REALLOC(IDs, ++nbr_IDs, ID_struct);
    }
    else
        SyntaxError(PROGRAM, "Program keyword is missing");
    if(!verifyToken(ID))
        SyntaxError(ID, "Program ID is missing");
    if(!verifyToken(SC))
        SyntaxError(SC, "SemiColon is missing");
    strcpy(curr_block, "PROGRAM");
    Block();
    if (!verifyToken(PT))
        SyntaxError(PT, "The end's Point is missing");
    fprintf(xml_file,"</PROGRAM>\n");
}

void Block(){
    fprintf(xml_file,"<BLOC>\n");
    Consts();
    Types();
    Vars();
    Functs();
    Prcds();
    Insts();
    fprintf(xml_file,"</BLOC>\n");
}

void Consts(){
    fprintf(xml_file,"<CONSTS>\n");
    if (verifyToken(CONST)){
        while(verifyToken(ID)){
            strcpy(IDs[nbr_IDs-1].name, curr_token.value);
            IDs[nbr_IDs-1].type = TCONST;
            IDs[nbr_IDs-1].line = curr_token.line;
            strcpy(IDs[nbr_IDs-1].block, curr_block);

            if(!verifyToken(EQ))
                SyntaxError(EQ, "EQUAL sign is missing");
            if(verifyToken(NUM) || isBool() || verifyToken(CHARVAL)){
                strcpy(IDs[nbr_IDs-1].value, curr_token.value);
            }
            else if (verifyToken(STRVAL)){
                strcpy(IDs[nbr_IDs-1].value, curr_token.value);
            }
            else
                SyntaxError(SC, "Constant's value is missing");
            REALLOC(IDs, ++nbr_IDs, ID_struct);
            if(!verifyToken(SC))
                SyntaxError(SC, "SemiColon is missing");
        }
    }
    fprintf(xml_file,"</CONSTS>\n");
}

void Types(){
    fprintf(xml_file,"<TYPES>\n");
    if (verifyToken(TYPE)){
        while(verifyToken(ID)){
            strcpy(IDs[nbr_IDs-1].name, curr_token.value);
            IDs[nbr_IDs-1].type = TTYPE;
            IDs[nbr_IDs-1].line = curr_token.line;
            strcpy(IDs[nbr_IDs-1].block, curr_block);
            REALLOC(IDs, ++nbr_IDs, ID_struct);

            if(!verifyToken(EQ))
                SyntaxError(EQ, "Equal sign is missing");
            if(!verifyToken(ARRAY))
                SyntaxError(ARRAY, "ARRAY keyword is missing");
            if(!verifyToken(OB))
                SyntaxError(OB, "Opening Bracket is missing");
            if(!verifyToken(NUM))
                SyntaxError(NUM, "NUM is missing");
            if(!verifyToken(PT)||!verifyToken(PT))
                SyntaxError(PT, ".. is missing");
            if(!verifyToken(ID)&&!verifyToken(NUM))
                SyntaxError(ID, "MaxArray is missing");
            if(!verifyToken(CB))
                SyntaxError(CB, "Closing Bracket is missing");
            if(!verifyToken(OF))
                SyntaxError(OF, "OF keyword is missing");
            if(!isType()&&!isNewType())
                SyntaxError(curr_token.code, "VAR type is missing");
            if(!verifyToken(SC))
                SyntaxError(SC, "SemiColon is missing");
        }
        if(curr_token.code==TYPE){
            if(!verifyToken(SC))
                SyntaxError(SC, "SemiColon is missing");
        }
    }
    fprintf(xml_file,"</TYPES>\n");
}

bool isType(){
    switch((curr_token = next_token()).code){
        case INTEGER : case BOOLEAN : case STRING :
        case REAL : case CHAR : break;
        default: previous_token(); return false;
    }
    WriteCurrTokenToFile();
    return true;
}

bool isNewType(){
    if(!verifyToken(ID))
        return false;
    for(int i = 0; i<=nbr_IDs-1;i++){
        if( (IDs[i].type==TTYPE) && (strcmp(IDs[i].name, curr_token.value)==0) )
            return true;
    }
    return false;
}

void Vars(){
    fprintf(xml_file,"<VARS>\n");
    if (verifyToken(VAR)){
        do{
            Dclr();
        }while(verifyToken(SC));
    }
    fprintf(xml_file,"</VARS>\n");
}

void Dclr(){
    int i = 0;
    do{
        verifyToken(VAR);
        if(verifyToken(ID)){
            i++;
            strcpy(IDs[nbr_IDs-1].name, curr_token.value);
            IDs[nbr_IDs-1].type = TVAR;
            IDs[nbr_IDs-1].line = curr_token.line;
            strcpy(IDs[nbr_IDs-1].block, curr_block);
            REALLOC(IDs, ++nbr_IDs, ID_struct);
        }
        else
            return;
    } while(verifyToken(COMMA));
    if (verifyToken(COMMA))
        SyntaxError(COMMA, "COMMA not needed");
    if(!verifyToken(CL))
        SyntaxError(CL, "COLON is missing");
    if(!isType()&&!isNewType())
        SyntaxError(curr_token.code, "Variable's type is missing");
    if (verifyToken(EQ) && (i==1) )
        Init();
    else if ( (curr_token.code==EQ) && i > 1 )
        SyntaxError(EQ, "Initializing more than one VAR in the same time is not allowed");
}

void Init(){
    if(verifyToken(NUM) || isBool() || verifyToken(CHARVAL) || verifyToken(STRVAL)){
        strcpy(IDs[nbr_IDs-2].value, curr_token.value);
    }
    else
        SyntaxError(curr_token.code, "Initialization value is missing");
}

void Functs(){
    fprintf(xml_file,"<FUNCTS>\n");
    while (verifyToken(FUNCTION)){
        if(!verifyToken(ID))
            SyntaxError(ID, "ID is missing");
        else{
            strcpy(IDs[nbr_IDs-1].name, curr_token.value);
            strcpy(curr_block, curr_token.value);
            IDs[nbr_IDs-1].type = TFCT;
            strcpy(IDs[nbr_IDs-1].block, curr_block);
            REALLOC(IDs, ++nbr_IDs, ID_struct);
        }
        if(!verifyToken(OP))
            SyntaxError(OP, "Opening parenthesis is missing");
        do{
            Dclr();
        }while(verifyToken(SC));
        if(!verifyToken(CP))
            SyntaxError(CP, "Closing parenthesis is missing");
        if(!verifyToken(CL))
            SyntaxError(CL, "COLON is missing");
        if(!isType()&&!isNewType())
            SyntaxError(curr_token.code, "Function's type is missing");
        if(!verifyToken(SC))
            SyntaxError(SC, "SemiColon is missing");
        Block();
        if(!verifyToken(SC))
            SyntaxError(SC, "SemiColon is missing");
    }
    fprintf(xml_file,"</FUNCTS>\n");
}


void Prcds(){
    fprintf(xml_file,"<PRCDS>\n");
    while (verifyToken(PROCEDURE)){
        if(!verifyToken(ID))
            SyntaxError(ID, "ID is missing");
        else{
            strcpy(IDs[nbr_IDs-1].name, curr_token.value);
            strcpy(curr_block, curr_token.value);
            IDs[nbr_IDs-1].type = TPRCD;
            IDs[nbr_IDs-1].line = curr_token.line;
            strcpy(IDs[nbr_IDs-1].block, curr_block);
            REALLOC(IDs, ++nbr_IDs, ID_struct);
        }
        if(verifyToken(OP)){
            do{
                Dclr();
            }while(verifyToken(SC));
            if(!verifyToken(CP))
                SyntaxError(CP, "Closing parenthesis is missing");
        }
        if(!verifyToken(SC))
            SyntaxError(SC, "SemiColon is missing");
        Block();
        if(!verifyToken(SC))
            SyntaxError(SC, "SemiColon is missing");
    }
    fprintf(xml_file,"</PRCDS>\n");
}

void Insts(){
    fprintf(xml_file,"<INSTS>\n");
    if (!verifyToken(BEGIN))
        SyntaxError(BEGIN, "BEGIN keyword is missing");
    while (Inst() && !verifyToken(END)){
        if(!verifyToken(SC))
            SyntaxError(SC, "SemiColon is missing");
    }
    if (curr_token.code!=END && !verifyToken(END))
        SyntaxError(END, "END keyword is missing");
    fprintf(xml_file,"</INSTS>\n");
}

bool Inst(){
    switch((curr_token = next_token()).code){
        case BEGIN  :
            previous_token();
            Insts();
            return true;
        case ID         :Assign_Execute();break;
        case IF         :If();break;
        case WHILE      :While();break;
        case FOR        :For();break;
        case CASE       :Case();break;
        case REPEAT     :Repeat();break;
        case WRITE      :Write();break;
        case WRITELN    :Write();break;
        case READ       :Read();break;
        case READLN     :Read();break;
        default:
            previous_token();
            return false;
    }
    fprintf(xml_file,"</INST>\n");
    return true;
}

void Assign_Execute(){
    fprintf(xml_file,"<INST>\n");
    WriteCurrTokenToFile();
     // verification of the 4th semantic rule
    if(isConstant(curr_token))
        SemanticError(curr_token.line, "The value of a const cannot be changed in the program");
    //Execute
    if(verifyToken(OP)){
        bool r, r2;
        do{
            r = Expr();
        }while((r2 = verifyToken(COMMA)));
        if(r2 && !r)
            SyntaxError(COMMA, "Comma not needed");
        if(!verifyToken(CP))
            SyntaxError(CP, "Closing parenthesis is missing");
        return;
    }
    // Assign
    if(verifyToken(OB)){
        Expr();
        if(!verifyToken(CB))
            SyntaxError(CB, "Closing bracket is missing");
    }
    if(!verifyToken(ASSIGN)){
        SyntaxError(ASSIGN, "Assign sign is missing");
        // verification of the 1st semantic rule
        if( verifyToken(CL) && (isType() || isNewType()) ){
            SemanticError(curr_token.line, "All the declarations must be in CONSTS, TYPES or VARS");
            return;
        }
    }
    if(!Expr())
        SyntaxError(curr_token.code, "The expression to be assigned is missing");
}

void If(){
    fprintf(xml_file,"<INST>\n");
    WriteCurrTokenToFile();
    Cond();
    verifyToken(THEN);
    Inst();
}

void While(){
    fprintf(xml_file,"<INST>\n");
    WriteCurrTokenToFile();
    Cond();
    verifyToken(DO);
    Inst();
}

void For(){
    fprintf(xml_file,"<INST>\n");
    WriteCurrTokenToFile();
    if(!verifyToken(ID))
        SyntaxError(ID, "ID is missing");
    if(!verifyToken(ASSIGN))
        SyntaxError(ASSIGN, "Assign sign is missing");
    if(!Expr())
        SyntaxError(curr_token.code, "Initial value is missing");
    if(!verifyToken(TO)&&!verifyToken(DOWNTO))
        SyntaxError(TO, "TO or DOWNTO keyword is missing");
    if(!Expr())
        SyntaxError(curr_token.code, "Final value is missing");
    if(!verifyToken(DO))
        SyntaxError(DO, "DO keyword is missing");
    Inst();
}

void Case(){
    fprintf(xml_file,"<INST>\n");
    WriteCurrTokenToFile();
    if(!verifyToken(ID))
        SyntaxError(ID, "ID is missing");
    if(!verifyToken(OF))
        SyntaxError(OF, "OF keyword is missing");
    if(!verifyToken(NUM))
        SyntaxError(NUM, "NUM is missing");
    do{
        if(!verifyToken(CL))
            SyntaxError(CL, "COLON is missing");
        Inst();
    }while(verifyToken(NUM));
    if(verifyToken(ELSE)){
        Inst();
    }
    if(!verifyToken(END))
        SyntaxError(END, "END keyword is missing");
}

void Repeat(){
    fprintf(xml_file,"<INST>\n");
    WriteCurrTokenToFile();
    Inst();
    if(!verifyToken(UNTIL))
        SyntaxError(UNTIL, "UNTIL keyword is missing");
    Cond();
}

void Cond(){
    bool r = verifyToken(OP);
    Expr();
    CondOp();
    if(!Expr())
        SyntaxError(curr_token.code, "Condition's right expression is missing");
    if(r && !verifyToken(CP))
        SyntaxError(CP, "Closing parenthesis is missing");
    if(verifyToken(AND) || verifyToken(OR))
        Cond();
}

 bool CondOp(){
     switch((curr_token = next_token()).code){
        case EQ: case PLUS: case MINUS: case MULT:
        case DIV: case INF: case INFEQ: case SUP:
        case SUPEQ: case DIFF: case NOT : break;
        default: return false;
    }
    if (curr_token.code==DIFF)
        fprintf(xml_file,"{%s, &#60;&#62;}\n", curr_token.name);
    else if (curr_token.code==INFEQ)
        fprintf(xml_file,"{%s, &#60;=}\n", curr_token.name);
    else if (curr_token.code==INF)
        fprintf(xml_file,"{%s, &#60;}\n", curr_token.name);
    else
        WriteCurrTokenToFile();
    return true;
 }

void Write(){
    bool r = true;
    fprintf(xml_file,"<INST>\n");
    WriteCurrTokenToFile();
    if(!verifyToken(OP))
        SyntaxError(OP, "Opening parenthesis is missing");
    do{
        r = Expr();
    }while(verifyToken(COMMA));
    if(!r)
        SyntaxError(COMMA, "Comma not needed");
    if(!verifyToken(CP))
        SyntaxError(CP, "Closing parenthesis is missing");
}

void Read(){
    bool r = true;
    fprintf(xml_file,"<INST>\n");
    WriteCurrTokenToFile();
    if(!verifyToken(OP))
        SyntaxError(OP, "Opening parenthesis is missing");
    verifyToken(ID);
    if(verifyToken(OB)){
        Expr();
        if(!verifyToken(CB))
            SyntaxError(CB, "Closing bracket is missing");
    }
    while(verifyToken(COMMA)){
        r = verifyToken(ID);
        if(verifyToken(OB)){
            Expr();
            if(!verifyToken(CB))
                SyntaxError(CB, "Closing bracket is missing");
        }
    }
    if(!r)
        SyntaxError(COMMA, "Comma not needed");
    if(!verifyToken(CP))
        SyntaxError(CP, "Closing parenthesis is missing");
}


bool Expr(){
    bool r = true;
    if ( isBool() || verifyToken(CHARVAL) || verifyToken(STRVAL) )
        return true;
    if (!Term())
        return false;
    while( AddOp() )
        r = Term();
    if (!r)
        SyntaxError(PLUS, "+ or - sign not needed");
    return true;
}

bool isBool(){
    if(verifyToken(TRUE) || verifyToken(FALSE))
        return true;
    return false;
}

bool Term(){
    if (!Factor())
        return false;
    bool r = true;
    while( MulOp() )
        r = Factor();
    if(!r)
        SyntaxError(MULT, "* or / sign not needed");
    return true;
}

bool AddOp(){
    if ( (verifyToken(PLUS)) || (verifyToken(MINUS)) )
        return true;
    return false;
}

bool Factor(){
    if ( !verifyToken(ID) && !verifyToken(NUM) && !verifyToken(OP) ){
        return false;
    }
    // expression between parentheses
    if (curr_token.code==OP){
        Expr();
        if(!verifyToken(CP))
            SyntaxError(CP, "Closing parenthesis is missing");
    }
    // array value
    else if (curr_token.code==ID && verifyToken(OB)){
        Expr();
        if(!verifyToken(CB))
            SyntaxError(CB, "Closing bracket is missing");
    }
    // function execution
    else if (curr_token.code==ID && verifyToken(OP)){
        bool r, r2;
        do{
            r = Expr();
        }while((r2 = verifyToken(COMMA)));
        if(r2 && !r)
            SyntaxError(COMMA, "Comma not needed");
        if(!verifyToken(CP))
            SyntaxError(CP, "Closing parenthesis is missing");
    }
    return true;
}

bool MulOp(){
    if ( verifyToken(MULT) || verifyToken(DIV) )
        return true;
    return false;
}
