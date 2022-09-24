#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <string>

enum TokenType {
	// Alternatives
	TT_UNEXPECTED_EOF = -2,
	TT_INVALID_TOKEN  = -1,
	TT_END_OF_FILE    =  0,

	// Arithmetic operators
	TT_ADD,           // +
	TT_DIV,           // /
	TT_MUL,           // *
	TT_SUB,           // -

	// Keywords
	TT_DO,            // do
	TT_ELSE,          // else
	TT_END,           // end
	TT_EXIT,		  // exit
	TT_IF,            // if
	TT_INT,			  // int
	TT_FLOAT,		  // float
	TT_PRINT,         // print
	TT_SCAN,          // scan
	TT_START,         // start
	TT_STRING,		  // string
	TT_THEN,          // then
	TT_WHILE,         // while

	// Logic operators
	TT_AND,           // &&
	TT_NOT,           // !
	TT_OR,         	  // ||

	// Relational operator
	TT_EQUAL,         // ==
	TT_GREATER,       // >
	TT_GREATER_EQUAL, // >=
	TT_LOWER,         // <
	TT_LOWER_EQUAL,   // <=
	TT_NOT_EQUAL,     // <>

	// Symbols
	TT_ASSIGN,        // =
	TT_COMMA,		  // ,
	TT_LEFT_PAR,	  // (
	TT_RIGHT_PAR,	  // )
	TT_SEMICOLON,     // ;

	// Zebras
	TT_ID,            // identifier
	TT_INTEGER_CONST, // integer_const
	TT_FLOAT_CONST,	  // float_const
	TT_LITERAL		  // literal
};

inline std::string tt2str(enum TokenType type) {
	switch (type) {
		// Alternatives
		case TT_UNEXPECTED_EOF:
			return "UNEXPECTED_EOF";
		case TT_INVALID_TOKEN:
			return "INVALID_TOKEN";
		case TT_END_OF_FILE:
			return "END_OF_FILE";

		// Arithmetic operators
		case TT_ADD:
			return "ADD";
		case TT_DIV:
			return "DIV";
		case TT_MUL:
			return "MUL";
		case TT_SUB:
			return "SUB";

		// Keywords
		case TT_DO:
			return "DO";
		case TT_ELSE:
			return "ELSE";
		case TT_END:
			return "END";
		case TT_EXIT:
			return "EXIT";
		case TT_IF:
			return "IF";
		case TT_INT:
			return "INT";
		case TT_FLOAT:
			return "FLOAT";
		case TT_PRINT:
			return "PRINT";
		case TT_SCAN:
			return "SCAN";
		case TT_START:
			return "START";
		case TT_STRING:
			return "STRING";
		case TT_THEN:
			return "THEN";
		case TT_WHILE:
			return "WHILE";

		// Logic operators
		case TT_AND:
			return "AND";
		case TT_OR:
			return "OR";
		case TT_NOT:
			return "NOT";

		// Relational operators
		case TT_EQUAL:
			return "EQUAL";
		case TT_GREATER:
			return "GREATER";
		case TT_GREATER_EQUAL:
			return "GREATER_EQUAL";
		case TT_LOWER:
			return "LOWER";
		case TT_LOWER_EQUAL:
			return "LOWER_EQUAL";
		case TT_NOT_EQUAL:
			return "NOT_EQUAL";

		// Symbols
		case TT_ASSIGN:
			return "ASSIGN";
		case TT_COMMA:
			return "COMMA";
		case TT_LEFT_PAR:
			return "LEFT_PAR";
		case TT_RIGHT_PAR:
			return "RIGHT_PAR";
		case TT_SEMICOLON:
			return "SEMICOLON";

		// Zebras
		case TT_ID:
			return "ID";
		case TT_INTEGER_CONST:
			return "INTEGER_CONST";
		case TT_FLOAT_CONST:
			return "FLOAT_CONST";
		case TT_LITERAL:
			return "LITERAL";

		default:
			throw std::string("invalid token type");
	}
}

#endif
