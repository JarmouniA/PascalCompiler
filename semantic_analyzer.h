#ifndef SEMANTIC_ANALYZER_H_INCLUDED
#define SEMANTIC_ANALYZER_H_INCLUDED

/** semantic rules
    1- All the declarations must be in CONSTS, TYPES or VARS
    2- No double declarations
    3- After BEGIN, all the IDs have to be already declared
    4- The value of a const cannot be changed in the program
    5- The program's ID cannot be used in the program as an ID
*/
    int currTokenNbr;
    extern Token next_token();
    extern Token previous_token();
    void SemanticError(int, char*);
    void semanticAnalysis();
    bool isConstant(Token);

#endif // SEMANTIC_ANALYZER_H_INCLUDED
