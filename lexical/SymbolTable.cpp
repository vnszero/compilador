#include "SymbolTable.h"

SymbolTable::SymbolTable() {
	// Arithmetic operators
	m_symbols["+"] = TT_ADD;            // +
	m_symbols["/"] = TT_DIV;            // /
	m_symbols["*"] = TT_MUL;            // *
	m_symbols["-"] = TT_SUB;            // -

	// Keywords
	m_symbols["do"] = TT_DO;            // do
	m_symbols["else"] = TT_ELSE;        // else
	m_symbols["end"] = TT_END;          // end
	m_symbols["exit"] = TT_EXIT;		// exit
	m_symbols["if"] = TT_IF;            // if
	m_symbols["int"] = TT_INT;			// int
	m_symbols["float"] = TT_FLOAT;		// float
	m_symbols["print"] = TT_PRINT;      // print
	m_symbols["scan"] = TT_SCAN;        // scan
	m_symbols["start"] = TT_START;      // start
	m_symbols["string"] = TT_STRING;	// string
	m_symbols["then"] = TT_THEN;        // then
	m_symbols["while"] = TT_WHILE;      // while

	// Logic operators
	m_symbols["&&"] = TT_AND;           // &&
	m_symbols["!"] = TT_NOT;            // !
	m_symbols["||"] = TT_OR;         	// ||

	// Relational operator
	m_symbols["=="] = TT_EQUAL;         // ==
	m_symbols[">"] = TT_GREATER;        // >
	m_symbols[">="] = TT_GREATER_EQUAL; // >=
	m_symbols["<"] = TT_LOWER;          // <
	m_symbols["<="] = TT_LOWER_EQUAL;   // <=
	m_symbols["<>"] = TT_NOT_EQUAL;     // <>

	// Symbols
	m_symbols["="] = TT_ASSIGN;         // =
	m_symbols[","] = TT_COMMA;		    // ,
	m_symbols["("] = TT_LEFT_PAR;	    // (
	m_symbols[")"] = TT_RIGHT_PAR;	    // )
	m_symbols[";"] = TT_SEMICOLON;      // ;
}

SymbolTable::~SymbolTable() {
}

bool SymbolTable::contains(const std::string& token) const {
	return m_symbols.find(token) != m_symbols.end();
}

enum TokenType SymbolTable::find(const std::string& token) {
	return this->contains(token) ? m_symbols[token] : TT_ID;
}
