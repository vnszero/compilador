#include "LexicalAnalysis.h"
LexicalAnalysis::LexicalAnalysis(const char* filename): m_line(1){
    m_input = fopen(filename,"r");
}

LexicalAnalysis::~LexicalAnalysis(){
    if(m_input !=nullptr)
        fclose(m_input);
}

Lexeme LexicalAnalysis::nextToken(){
    Lexeme lex;

    return lex;
}

    /*private:
        int m_line;
        SymbolTable m_st;
        FILE* m_input;*/
        