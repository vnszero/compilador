#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include "Lexeme.h"
#include "SymbolTable.h"
class LexicalAnalysis{
    public:
        LexicalAnalysis(const char* filmename);
        virtual ~LexicalAnalysis();

        Lexeme nextToken();

    private:
        int m_line;
        SymbolTable m_st;
        FILE* m_input;
};


#endif