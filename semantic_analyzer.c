#include "dependencies.h"
#include "semantic_analyzer.h"

void SemanticError(int line, char *message){
    printf("Semantic Error: Line %d:\n%s\n", line, message);
}

// rule 1 and 4 are verified during syntax analysis, "Assign_Execute" function

void semanticAnalysis(){
    // rule 2
    for(int i=1; i<nbr_IDs-1;i++){
        ID_struct ID1 =IDs[i];
        // rule 5
        if(strcmp(IDs[0].name, ID1.name)==0)
            SemanticError(ID1.line, "The program's ID cannot be used in the program as an ID");
        for(int j=1; j<nbr_IDs-1; j++){
            ID_struct ID2=IDs[j];
            if( j!=i && strcmp(ID1.name, ID2.name)==0 && strcmp(ID1.block, ID2.block)==0)
                SemanticError(ID1.line, "Double declaration of this ID");
        }
    }
}

// rule 4
bool isConstant(Token tkn){
    for(int i=1; i<nbr_IDs-1;i++){
        ID_struct ID1 =IDs[i];
        if( strcmp(ID1.name, tkn.value)==0 && ID1.type==TCONST)
            return true;
    }
    return false;
}
