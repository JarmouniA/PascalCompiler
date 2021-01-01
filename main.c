#include "dependencies.h"
#include "header.h"

const char* out_file_path = "./tokens.txt";
const char* xml_file_path = "./syntax_tree.xml";

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Program file's path is missing\n");
        exit(1);
    }

    const char* in_file_path = argv[1];

    prog_file = fopen(in_file_path, "r");
    if(prog_file == NULL){
        perror("Error while opening program file");
        exit(1);
    }

    out_file = fopen(out_file_path, "w");
    if(out_file == NULL){
        perror("Error while opening tokens output file");
        exit(1);
    }

    lexicalAnalysis(); // Lexical Analysis : tokenization

    fclose(prog_file);
    fclose(out_file);

    xml_file = fopen(xml_file_path, "w");
    if(xml_file == NULL){
        perror("Error while opening syntax tree file");
        exit(1);
    }

    fprintf(xml_file,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(xml_file,"<note>\n");
    Program(); //Syntax Analysis : parsing
    fprintf(xml_file,"</note>\n");

    fclose(xml_file);

    semanticAnalysis(); //semantic analysis

    printf("Program compiled\nOutput files: ./tokens.txt, ./syntax_tree.xml\n");

    exit(0);
}
