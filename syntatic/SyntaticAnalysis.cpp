#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include "SyntaticAnalysis.h"

SyntaticAnalysis::SyntaticAnalysis(LexicalAnalysis& lex) :
	m_lex(lex), m_current(lex.nextToken()) {
}
SyntaticAnalysis::~SyntaticAnalysis() {
}
void SyntaticAnalysis::start(){
	procProgram();
	eat(TT_END_OF_FILE);
}

void SyntaticAnalysis::advance() {
	m_current = m_lex.nextToken();
}

void SyntaticAnalysis::eat(enum TokenType type) {
	std::cout << "Expected (..., " << tt2str(type)
	          <<  "), found (\"" + m_current.token << "\", "
			     << tt2str(m_current.type) << ")" << std::endl;
	if (type == m_current.type) {
		advance();
	} else {
		showError();
	}
}

void SyntaticAnalysis::showError() {
	std::cout << std::setw(2) << std::setfill('0') << m_lex.line() << ": ";

	switch (m_current.type) {
		case TT_INVALID_TOKEN:
			std::cout << "Lexema invalido [" << m_current.token << "]" << std::endl;
			break;
		case TT_UNEXPECTED_EOF:
		case TT_END_OF_FILE:
			std::cout << "Fim de arquivo inesperado" << std::endl;
			break;
		default:
			std::cout << "Lexema nao esperado [" << m_current.token << "]" << std::endl;
			break;
	}
	exit(1);
}

/* TP base */
// <program> ::= TT_START [ <decllist> ] <stmtlist> TT_EXIT
void SyntaticAnalysis::procProgram() {
	eat(TT_START);
	// [ <decllist> ]
	procStmtList();
	eat(TT_EXIT);
}

// <decllist> ::= <decl> { <decl> }
void SyntaticAnalysis::procDeclList() {
	procDecl();
	// { <decl> }
}

// <decl> ::= <type> <identlist> TT_SEMICOLON
void SyntaticAnalysis::procDecl() {
	procType();
	procIdentList();
	eat(TT_SEMICOLON);
}

// <identlist> ::= <identifier> { TT_COMMA <identifier> }
void SyntaticAnalysis::procIdentList() {
	procIdentifier();
	// { TT_COMMA <identifier> }
}

// <type> ::= TT_INT | TT_FLOAT | TT_STRING
void SyntaticAnalysis::procType() {
	switch(m_current.type) {
		case TT_INT:
			eat(TT_INT);
			break;
		case TT_FLOAT:
			eat(TT_FLOAT);
			break;
		case TT_STRING:
			eat(TT_STRING);
			break;
		default:
			showError();
			break;
	}
}

// <stmtlist> ::= <stmt> { <stmt> }
void SyntaticAnalysis::procStmtList() {
	procStmt();
	// { <stmt> }
}

// <stmt> ::= <assignstmt> TT_SEMICOLON |
//				<ifstmt> | <whilestmt> |
//				<readstmt> TT_SEMICOLON |
//				<writestmt> TT_SEMICOLON
void SyntaticAnalysis::procStmt() {
	/* look for firsts */
}

// <assignstmt> ::= <identifier> TT_ASSIGN <simpleexpr>
void SyntaticAnalysis::procAssignStmt() {
	procIdentifier();
	eat(TT_ASSIGN);
	procSimpleExpr();
}

// <ifstmt> ::= TT_IF <condition> TT_THEN <stmtlist> TT_END |
//				TT_IF <condition> TT_THEN <stmtlist> TT_ELSE <stmtlist> TT_END
void SyntaticAnalysis::procIfStmt() {
	eat(TT_IF);
	procCondition();
	eat(TT_THEN);
	procStmtList();
	switch(m_current.type) {
		case TT_END:
			eat(TT_END);
			break;
		case TT_ELSE:
			eat(TT_ELSE);
			procStmtList();
			eat(TT_END);
		default:
			showError();
			break;
	}

}

// <condition> ::= <expression>
void SyntaticAnalysis::procCondition() {
	procExpression(); // ?
}

// <whilestmt> ::= TT_DO <stmtlist> <stmtsufix>
void SyntaticAnalysis::procWhileStmt() {
	eat(TT_DO);
	procStmtList();
	procStmtSufix();
}

// <stmtsufix> ::= TT_WHILE <condition> TT_END
void SyntaticAnalysis::procStmtSufix() {
	eat(TT_WHILE);
	procCondition();
	eat(TT_END);
}

// <readstmt> ::= TT_SCAN TT_LEFT_PAR <identifier> TT_RIGHT_PAR
void SyntaticAnalysis::procReadStmt() {
	eat(TT_SCAN);
	eat(TT_LEFT_PAR);
	procIdentifier();
	eat(TT_RIGHT_PAR);
}

// <writestmt> ::= TT_PRINT TT_LEFT_PAR <writable> TT_RIGHT_PAR
void SyntaticAnalysis::procWriteStmt() {
	eat(TT_PRINT);
	eat(TT_LEFT_PAR);
	procWritable();
	eat(TT_RIGHT_PAR);
}

// <writable> ::= <simpleexpr> | <literal>
void SyntaticAnalysis::procWritable() {
	// <simpleexpr> | <literal>
}

// <expression> ::= <simpleexpr> | <simpleexpr> <relop> <simpleexpr>
void SyntaticAnalysis::procExpression() {
	// <simpleexpr> | <simpleexpr> <relop> <simpleexpr>
}

// <simpleexpr> ::= <term> | <simpleexpr> <addop> <term>
void SyntaticAnalysis::procSimpleExpr() {
	// <term> | <simpleexpr> <addop> <term>
}

// <term> ::= <fatora> | <term> <mulop> <fatora>
// <term> ::= [ <term> <mulop> ] <fatora>
void SyntaticAnalysis::procTerm() {
	// left call
}

// <fatora> ::= <factor> | TT_NOT <factor> | TT_SUB <factor>
// <fatora> ::= [ TT_NOT | TT_SUB ] <factor>
void SyntaticAnalysis::procFatora() {
	if(m_current.type == TT_NOT) {
		eat(TT_NOT);
	}
	else if (m_current.type == TT_SUB) {
		eat(TT_SUB);
	}
	procFactor();
}

// <factor> ::= <identifier> | <constant> | TT_LEFT_PAR <expression> TT_RIGHT_PAR
void SyntaticAnalysis::procFactor() {
	switch(m_current.type) {
		case TT_ID:
			procIdentifier();
			break;
		case TT_INTEGER_CONST:
		case TT_FLOAT_CONST:
		case TT_LITERAL:
			procConstant();
			break;
		case TT_LEFT_PAR:
			eat(TT_LEFT_PAR);
			procExpression();
			eat(TT_RIGHT_PAR);
			break;
		default:
			showError();
			break;
	}
}

// <relop> ::= TT_EQUAL | TT_GREATER | TT_GREATER_EQUAL |
//			   TT_LOWER | TT_LOWER_EQUAL | TT_NOT_EQUAL
void SyntaticAnalysis::procRelop() {
	switch(m_current.type) {
		case TT_EQUAL:
			eat(TT_EQUAL);
			break;
		case TT_GREATER:
			eat(TT_GREATER);
			break;
		case TT_GREATER_EQUAL:
			eat(TT_GREATER_EQUAL);
			break;
		case TT_LOWER:
			eat(TT_LOWER);
			break;
		case TT_LOWER_EQUAL:
			eat(TT_LOWER_EQUAL);
			break;
		case TT_NOT_EQUAL:
			eat(TT_NOT_EQUAL);
			break;
		default:
			showError();
			break;
	}
}

// <addop> ::= TT_ADD | TT_SUB | TT_OR
void SyntaticAnalysis::procAddOp() {
	switch(m_current.type) {
		case TT_ADD:
			eat(TT_ADD);
			break;
		case TT_SUB:
			eat(TT_SUB);
			break;
		case TT_OR:
			eat(TT_OR);
			break;
		default:
			showError();
			break;
	}
}

// <mulop> ::= TT_MUL | TT_DIV | TT_AND
void SyntaticAnalysis::procMulOp() {
	switch(m_current.type) {
		case TT_MUL:
			eat(TT_MUL);
			break;
		case TT_DIV:
			eat(TT_DIV);
			break;
		case TT_AND:
			eat(TT_AND);
			break;
		default:
			showError();
			break;
	}
}

// <constant> ::= TT_INTEGER_CONST | TT_FLOAT_CONST | <literal>
void SyntaticAnalysis::procConstant() {
	switch(m_current.type) {
		case TT_INTEGER_CONST:
			eat(TT_INTEGER_CONST);
			break;
		case TT_FLOAT_CONST:
			eat(TT_FLOAT_CONST);
			break;
		case TT_LITERAL:
			procLiteral();
			break;
		default:
			showError();
			break;
	}
}

//<literal> ::= TT_LITERAL
void SyntaticAnalysis::procLiteral() {
	eat(TT_LITERAL);
}

// <identifier> ::= TT_ID
void SyntaticAnalysis::procIdentifier() {
	eat(TT_ID);
}
