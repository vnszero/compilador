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
// First(<program>) = { TT_START }
void SyntaticAnalysis::procProgram() {
	eat(TT_START);
	if(m_current.type == TT_INT || m_current.type == TT_FLOAT || m_current.type == TT_STRING) {
		procDeclList();
	}
	procStmtList();
	eat(TT_EXIT);
}

// <decllist> ::= <decl> { <decl> }
// First(<decllist>) = { TT_INT, TT_FLOAT, TT_STRING }
void SyntaticAnalysis::procDeclList() {
	procDecl();
	while(m_current.type == TT_INT ||
		  m_current.type == TT_FLOAT ||
		  m_current.type == TT_STRING) {
		procDecl();
	}
}

// <decl> ::= <type> <identlist> TT_SEMICOLON
// First(<decl>) = { TT_INT, TT_FLOAT, TT_STRING }
void SyntaticAnalysis::procDecl() {
	procType();
	procIdentList();
	eat(TT_SEMICOLON);
}

// <identlist> ::= <identifier> { TT_COMMA <identifier> }
// First(<identlist>) = { TT_ID }
void SyntaticAnalysis::procIdentList() {
	procIdentifier();
	// look for TT_COMMA to keep loop
	while (m_current.type == TT_COMMA) {
		eat(TT_COMMA);
		procIdentifier();
	}
}

// <type> ::= TT_INT | TT_FLOAT | TT_STRING
// First(<type>) = { TT_INT, TT_FLOAT, TT_STRING }
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
// First(<stmtlist>) = { TT_ID, TT_IF, TT_DO, TT_SCAN, TT_PRINT }
void SyntaticAnalysis::procStmtList() {
	procStmt();
	// look for firsts to keep loop
	while (m_current.type == TT_ID ||
	       m_current.type == TT_IF ||
		   m_current.type == TT_DO ||
		   m_current.type == TT_SCAN ||
		   m_current.type == TT_PRINT) {
		procStmt();
	}
}

// <stmt> ::= <assignstmt> TT_SEMICOLON |
//				<ifstmt> | <whilestmt> |
//				<readstmt> TT_SEMICOLON |
//				<writestmt> TT_SEMICOLON
// First(<stmt>) = { TT_ID, TT_IF, TT_DO, TT_SCAN, TT_PRINT }
void SyntaticAnalysis::procStmt() {
	switch(m_current.type){
		case TT_ID:
			procAssignStmt();
			eat(TT_SEMICOLON);
			break;
		case TT_IF:
			procIfStmt();
			break;
		case TT_DO:
			procWhileStmt();
			break;
		case TT_SCAN:
			procReadStmt();
			eat(TT_SEMICOLON);
			break;
		case TT_PRINT:
			procWriteStmt();
			eat(TT_SEMICOLON);
			break;
		default:
			showError();
			break;
	}
}

// <assignstmt> ::= <identifier> TT_ASSIGN <simpleexpr>
// First(<assignstmt>) = { TT_ID }
void SyntaticAnalysis::procAssignStmt() {
	procIdentifier();
	eat(TT_ASSIGN);
	procSimpleExpr();
}

// <ifstmt> ::= TT_IF <condition> TT_THEN <stmtlist> <ifstmtprime>
// First(<ifstmt>) = { TT_IF }
void SyntaticAnalysis::procIfStmt() {
	eat(TT_IF);
	procCondition();
	eat(TT_THEN);
	procStmtList();
	procIfStmtPrime();
}

// <ifstmtprime> ::= TT_END | TT_ELSE <stmtlist> TT_END
// First(ifstmtprime) = { TT_END, TT_ELSE }
void SyntaticAnalysis::procIfStmtPrime() {
	switch(m_current.type) {
		case TT_END:
			eat(TT_END);
			break;
		case TT_ELSE:
			eat(TT_ELSE);
			procStmtList();
			eat(TT_END);
			break;
		default:
			showError();
			break;
	}
}

// <condition> ::= <expression>
// First(<condition>) = { TT_NOT, TT_SUB,
//			   		 	   TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
// note: this method looks like useless, why not call expression directly?
void SyntaticAnalysis::procCondition() {
	procExpression();
}

// <whilestmt> ::= TT_DO <stmtlist> <stmtsufix>
// First(<whilestmt>) = { TT_DO }
void SyntaticAnalysis::procWhileStmt() {
	eat(TT_DO);
	procStmtList();
	procStmtSufix();
}

// <stmtsufix> ::= TT_WHILE <condition> TT_END
// First(<stmtsufix>) = { TT_WHILE }
void SyntaticAnalysis::procStmtSufix() {
	eat(TT_WHILE);
	procCondition();
	eat(TT_END);
}

// <readstmt> ::= TT_SCAN TT_LEFT_PAR <identifier> TT_RIGHT_PAR
// First(<readstmt>) = { TT_SCAN }
void SyntaticAnalysis::procReadStmt() {
	eat(TT_SCAN);
	eat(TT_LEFT_PAR);
	procIdentifier();
	eat(TT_RIGHT_PAR);
}

// <writestmt> ::= TT_PRINT TT_LEFT_PAR <writable> TT_RIGHT_PAR
// First(<writestmt>) = { TT_PRINT }
void SyntaticAnalysis::procWriteStmt() {
	eat(TT_PRINT);
	eat(TT_LEFT_PAR);
	procWritable();
	eat(TT_RIGHT_PAR);
}

// <writable> ::= <simpleexpr> | <literal>
// First(<writable>) = { TT_NOT, TT_SUB,
//			   		 	 TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
// note: this 'or' does not make sence cause literal is included by simpleexpr
void SyntaticAnalysis::procWritable() {
	switch(m_current.type) {
		case TT_LITERAL:
			procLiteral();
			break;
		default:
			procSimpleExpr();
			break;
	}
}

// <expression> ::= <simpleexpr> <expressionprime>
// First(<expression>) = { TT_NOT, TT_SUB,
//			   		 	   TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
void SyntaticAnalysis::procExpression() {
	procSimpleExpr();
	procExpressionPrime();
}

// <expressionprime> ::= <relop> <simpleexpr> <expressionprime> | LAMBDA
// First(<expressionprime>) = { TT_EQUAL, TT_GREATER, TT_GREATER_EQUAL,
//			   					TT_LOWER, TT_LOWER_EQUAL, TT_NOT_EQUAL, LAMBDA }
void SyntaticAnalysis::procExpressionPrime() {
	switch(m_current.type) {
		case TT_EQUAL:
		case TT_GREATER:
		case TT_GREATER_EQUAL:
		case TT_LOWER:
		case TT_LOWER_EQUAL:
		case TT_NOT_EQUAL:
			procRelop();
			procSimpleExpr();
			procExpressionPrime();
			break;
		default: break; //LAMBDA
	}
}

// <simpleexpr> ::= <term> <simpleexprprime>
// First(<simpleexpr>) = { TT_NOT, TT_SUB,
//			   		 	   TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
void SyntaticAnalysis::procSimpleExpr() {
	procTerm();
	procSimpleExprPrime();
}

// <simpleexprprime> ::= <addop> <term> <simpleexprprime> | LAMBDA
// First(<simpleexprprime>) = { TT_ADD, TT_SUB, TT_OR, LAMBDA }
void SyntaticAnalysis::procSimpleExprPrime() {
	switch(m_current.type) {
		case TT_ADD:
		case TT_SUB:
		case TT_OR:
			procAddOp();
			procTerm();
			procSimpleExprPrime();
			break;
		default: break; //LAMBDA
	}
}

// <term> ::= <fatora> <termprime>
// First(<term>) = { TT_NOT, TT_SUB,
//			   		 TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
void SyntaticAnalysis::procTerm() {
	procFatora();
	procTermPrime();
}

// <termprime> ::= <mulop> <fatora> <termprime> | LAMBDA
// First(<termprime>) = { TT_MUL, TT_DIV, TT_AND, LAMBDA }
void SyntaticAnalysis::procTermPrime() {
	switch(m_current.type) {
		case TT_MUL:
		case TT_DIV:
		case TT_AND:
			procMulOp();
			procFatora();
			procTermPrime();
			break;
		default: break; //LAMBDA
	}
}

// <fatora> ::= [ TT_NOT | TT_SUB ] <factor>
// First(<fatora>) = { TT_NOT, TT_SUB,
//			   		   TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
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
// First(<factor>) = { TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
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
// First(<relop>) = { TT_EQUAL, TT_GREATER, TT_GREATER_EQUAL,
//			   		  TT_LOWER, TT_LOWER_EQUAL, TT_NOT_EQUAL }
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
// First(<addop>) = { TT_ADD, TT_SUB, TT_OR }
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
// First(<mulop>) = { TT_MUL, TT_DIV, TT_AND }
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
// First(<constant>) = { TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL }
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
// First(<literal>) = { TT_LITERAL }
void SyntaticAnalysis::procLiteral() {
	eat(TT_LITERAL);
}

// <identifier> ::= TT_ID
// First(<identifier>) = { TT_ID }
void SyntaticAnalysis::procIdentifier() {
	eat(TT_ID);
}
