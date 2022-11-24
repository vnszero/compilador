#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include "SyntaticAnalysis.h"
#include <cstring>

SyntaticAnalysis::SyntaticAnalysis(LexicalAnalysis& lex) :
	m_lex(lex), m_current(lex.nextToken()) {
}
SyntaticAnalysis::~SyntaticAnalysis() {
}
void SyntaticAnalysis::start(/*no param*/){ //no return
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

		// SEMICOLON, ELSE, END and THEN will be tuner tokens
		if (type == TT_SEMICOLON
			|| type == TT_ELSE
			|| type == TT_END
			|| type == TT_THEN) {
			do {
				advance();
				if (type == m_current.type) {
					advance();
					break;
				}
			} while (1);
		}

	}
}

void SyntaticAnalysis::showError() {
	std::cout << "\033[1;31m" << std::setw(2) << std::setfill('0') << m_lex.line() << ": " << "\033[0m";

	switch (m_current.type) {
		case TT_INVALID_TOKEN:
			std::cout << "\033[1;31m" << "Lexema invalido [" << m_current.token << "]" << "\033[0m" << std::endl;
			break;
		case TT_UNEXPECTED_EOF:
		case TT_END_OF_FILE:
			std::cout << "\033[1;31m" << "Fim de arquivo inesperado" << "\033[0m" << std::endl;
			break;
		default:
			std::cout << "\033[1;31m" << "Lexema nao esperado [" << m_current.token << "]" << "\033[0m" << std::endl;
			break;
	}
	// exit(1); // try to recover from error
}

/* TP base */
// <program> ::= TT_START [ <decllist> ] <stmtlist> TT_EXIT
// First(<program>) = { TT_START }
void SyntaticAnalysis::procProgram(/*no param*/) { //no return
	eat(TT_START);
	if(m_current.type == TT_INT || m_current.type == TT_FLOAT || m_current.type == TT_STRING) {
		procDeclList();
	}
	procStmtList();
	eat(TT_EXIT);
}

// <decllist> ::= <decl> { <decl> }
// First(<decllist>) = { TT_INT, TT_FLOAT, TT_STRING }
void SyntaticAnalysis::procDeclList(/*no param*/) { //no return
	procDecl();
	while(m_current.type == TT_INT ||
		  m_current.type == TT_FLOAT ||
		  m_current.type == TT_STRING) {
		procDecl();
	}
}

// <decl> ::= <type> <identlist> TT_SEMICOLON
// First(<decl>) = { TT_INT, TT_FLOAT, TT_STRING }
void SyntaticAnalysis::procDecl(/*no param*/) { //no return
	SemanticBody ib;
	ib = procType();
	//set sb based on types
	procIdentList(ib);
	eat(TT_SEMICOLON);
}

// <identlist> ::= <identifier> { TT_COMMA <identifier> }
// First(<identlist>) = { TT_ID }
void SyntaticAnalysis::procIdentList(SemanticBody sb) { //no return
	procIdentifier(sb);
	// look for TT_COMMA to keep loop
	while (m_current.type == TT_COMMA) {
		eat(TT_COMMA);
		procIdentifier(sb);
	}
}

// <type> ::= TT_INT | TT_FLOAT | TT_STRING
// First(<type>) = { TT_INT, TT_FLOAT, TT_STRING }
SemanticBody SyntaticAnalysis::procType(/*no param*/) { //-> SemanticBody
	SemanticBody ib;
	switch(m_current.type) {
		case TT_INT:
			eat(TT_INT);
			ib.setType('I');
			break;
		case TT_FLOAT:
			eat(TT_FLOAT);
			ib.setType('F');
			break;
		case TT_STRING:
			eat(TT_STRING);
			ib.setType('S');
			break;
		default:
			showError();
			ib.setType('E');
			break;
	}
	return ib;
}

// <stmtlist> ::= <stmt> { <stmt> }
// First(<stmtlist>) = { TT_ID, TT_IF, TT_DO, TT_SCAN, TT_PRINT }
void SyntaticAnalysis::procStmtList(/*no param*/) { //no return
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
void SyntaticAnalysis::procStmt(/*no param*/) { //no return
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
void SyntaticAnalysis::procAssignStmt(/*no param*/) { //no return
	SemanticBody sb, ib_left, ib_right;

	ib_left = procIdentifier(sb);
	eat(TT_ASSIGN);
	ib_right = procSimpleExpr(sb);
	std::cout << ib_left.getType() << " == " << ib_right.getType() << std::endl;
	if(ib_left.getType() != ib_right.getType()){
		std::cout << "semantic error procAssignStmt" << std::endl;
	}
}

// <ifstmt> ::= TT_IF <condition> TT_THEN <stmtlist> <ifstmtprime>
// First(<ifstmt>) = { TT_IF }
void SyntaticAnalysis::procIfStmt(/*no param*/) { //no return
	eat(TT_IF);
	procCondition();
	eat(TT_THEN);
	procStmtList();
	procIfStmtPrime();
}

// <ifstmtprime> ::= TT_END | TT_ELSE <stmtlist> TT_END
// First(ifstmtprime) = { TT_END, TT_ELSE }
void SyntaticAnalysis::procIfStmtPrime(/*no param*/) { //no return
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
void SyntaticAnalysis::procCondition(/*no param*/) { //no return
	SemanticBody sb;
	procExpression(sb);
}

// <whilestmt> ::= TT_DO <stmtlist> <stmtsufix>
// First(<whilestmt>) = { TT_DO }
void SyntaticAnalysis::procWhileStmt(/*no param*/) { //no return
	eat(TT_DO);
	procStmtList();
	procStmtSufix();
}

// <stmtsufix> ::= TT_WHILE <condition> TT_END
// First(<stmtsufix>) = { TT_WHILE }
void SyntaticAnalysis::procStmtSufix(/*no param*/) { //no return
	eat(TT_WHILE);
	procCondition();
	eat(TT_END);
}

// <readstmt> ::= TT_SCAN TT_LEFT_PAR <identifier> TT_RIGHT_PAR
// First(<readstmt>) = { TT_SCAN }
void SyntaticAnalysis::procReadStmt(/*no param*/) { //no return
	SemanticBody sb;
	eat(TT_SCAN);
	eat(TT_LEFT_PAR);
	procIdentifier(sb);
	eat(TT_RIGHT_PAR);
}

// <writestmt> ::= TT_PRINT TT_LEFT_PAR <writable> TT_RIGHT_PAR
// First(<writestmt>) = { TT_PRINT }
void SyntaticAnalysis::procWriteStmt(/*no param*/) { //no return
	eat(TT_PRINT);
	eat(TT_LEFT_PAR);
	procWritable();
	eat(TT_RIGHT_PAR);
}

// <writable> ::= <simpleexpr> | <literal>
// First(<writable>) = { TT_NOT, TT_SUB,
//			   		 	 TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
// note: this 'or' does not make sence cause literal is included by simpleexpr
void SyntaticAnalysis::procWritable(/*no param*/) { //no return
	SemanticBody sb;
	switch(m_current.type) {
		case TT_LITERAL:
			procLiteral();
			break;
		default:
			procSimpleExpr(sb);
			break;
	}
}

// <expression> ::= <simpleexpr> <expressionprime>
// First(<expression>) = { TT_NOT, TT_SUB,
//			   		 	   TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
SemanticBody SyntaticAnalysis::procExpression(SemanticBody sb) { //-> SemanticBody
	SemanticBody ib_expr, ib_expr_prime;
	ib_expr = procSimpleExpr(sb);
	ib_expr_prime = procExpressionPrime(sb);
	if (ib_expr.getType() != ib_expr_prime.getType()) {
		std::cout << "semantic error procExpression" << std::endl;
		ib_expr.setType('E');
	}
	return ib_expr;
}

// <expressionprime> ::= <relop> <simpleexpr> <expressionprime> | LAMBDA
// First(<expressionprime>) = { TT_EQUAL, TT_GREATER, TT_GREATER_EQUAL,
//			   					TT_LOWER, TT_LOWER_EQUAL, TT_NOT_EQUAL, LAMBDA }
SemanticBody SyntaticAnalysis::procExpressionPrime(SemanticBody sb) { //-> SemanticBody
	SemanticBody ib_simple_expr, ib_expr_prime;
	switch(m_current.type) {
		case TT_EQUAL:
		case TT_GREATER:
		case TT_GREATER_EQUAL:
		case TT_LOWER:
		case TT_LOWER_EQUAL:
		case TT_NOT_EQUAL:
			procRelop();
			ib_simple_expr = procSimpleExpr(sb);
			ib_expr_prime = procExpressionPrime(sb);
			break;
		default:
			ib_simple_expr.setType('L');
			ib_expr_prime.setType('L');
			break; //LAMBDA
	}
	if (ib_simple_expr.getType() != ib_expr_prime.getType() && ib_expr_prime.getType() != 'L') {
		std::cout << "semantic error procExpressionPrime" << std::endl;
		ib_simple_expr.setType('E');
	}
	return ib_simple_expr;
}

// <simpleexpr> ::= <term> <simpleexprprime>
// First(<simpleexpr>) = { TT_NOT, TT_SUB,
//			   		 	   TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
SemanticBody SyntaticAnalysis::procSimpleExpr(SemanticBody sb) { //-> SemanticBody
	SemanticBody ib_term, ib_simple_expr_prime;
	ib_term = procTerm(sb);
	ib_simple_expr_prime = procSimpleExprPrime(sb);
	std::cout << ib_term.getType() << " == " << ib_simple_expr_prime.getType() << std::endl;
	if (ib_term.getType() != ib_simple_expr_prime.getType() && ib_simple_expr_prime.getType() != 'L') {
		std::cout << "semantic error procSimpleExpr" << std::endl;
		ib_term.setType('E');
	}
	return ib_term;
}

// <simpleexprprime> ::= <addop> <term> <simpleexprprime> | LAMBDA
// First(<simpleexprprime>) = { TT_ADD, TT_SUB, TT_OR, LAMBDA }
SemanticBody SyntaticAnalysis::procSimpleExprPrime(SemanticBody sb) { //-> SemanticBody
	SemanticBody ib_term, ib_simple_expr_prime;
	switch(m_current.type) {
		case TT_ADD:
		case TT_SUB:
		case TT_OR:
			procAddOp();
			ib_term = procTerm(sb);
			ib_simple_expr_prime = procSimpleExprPrime(sb);
			break;
		default:
			ib_term.setType('L');
			ib_simple_expr_prime.setType('L');
			break; //LAMBDA
	}
	if (ib_term.getType() != ib_simple_expr_prime.getType() && ib_simple_expr_prime.getType() != 'L') {
		std::cout << "semantic error SimpleExprPrime" << std::endl;
		ib_term.setType('E');
	}
	return ib_term;
}

// <term> ::= <fatora> <termprime>
// First(<term>) = { TT_NOT, TT_SUB,
//			   		 TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
SemanticBody SyntaticAnalysis::procTerm(SemanticBody sb) { //-> SemanticBody
	SemanticBody ib_fatora, ib_term_prime;
	ib_fatora = procFatora(sb);
	ib_term_prime = procTermPrime(sb);
	std::cout << ib_fatora.getType() << " == " << ib_term_prime.getType() << std::endl;
	if (ib_fatora.getType() != ib_term_prime.getType() && ib_term_prime.getType() != 'L') {
		std::cout << "semantic error procTerm" << std::endl;
		ib_fatora.setType('E');
	}
	return ib_fatora;
}

// <termprime> ::= <mulop> <fatora> <termprime> | LAMBDA
// First(<termprime>) = { TT_MUL, TT_DIV, TT_AND, LAMBDA }
SemanticBody SyntaticAnalysis::procTermPrime(SemanticBody sb) { //-> SemanticBody
	SemanticBody ib_fatora, ib_term_prime;
	switch(m_current.type) {
		case TT_MUL:
		case TT_DIV:
		case TT_AND:
			procMulOp();
			ib_fatora = procFatora(sb);
			ib_term_prime = procTermPrime(sb);
			break;
		default:
			ib_fatora.setType('L');
			ib_term_prime.setType('L');
			break; //LAMBDA
	}
	if (ib_fatora.getType() != ib_term_prime.getType() && ib_term_prime.getType() != 'L') {
		std::cout << "semantic error procTermPrime" << std::endl;
		ib_fatora.setType('E');
	}
	return ib_fatora;
}

// <fatora> ::= [ TT_NOT | TT_SUB ] <factor>
// First(<fatora>) = { TT_NOT, TT_SUB,
//			   		   TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
SemanticBody SyntaticAnalysis::procFatora(SemanticBody sb) { //-> SemanticBody
	SemanticBody ib_factor;
	if(m_current.type == TT_NOT) {
		eat(TT_NOT);
	}
	else if (m_current.type == TT_SUB) {
		eat(TT_SUB);
	}
	ib_factor = procFactor(sb);
	return ib_factor;
}

// <factor> ::= <identifier> | <constant> | TT_LEFT_PAR <expression> TT_RIGHT_PAR
// First(<factor>) = { TT_ID, TT_INTEGER_CONST, TT_FLOAT_CONST, TT_LITERAL, TT_LEFT_PAR }
SemanticBody SyntaticAnalysis::procFactor(SemanticBody sb) { //-> SemanticBody
	SemanticBody ib;
	switch(m_current.type) {
		case TT_ID:
			ib = procIdentifier(sb);
			break;
		case TT_INTEGER_CONST:
		case TT_FLOAT_CONST:
		case TT_LITERAL:
			ib = procConstant();
			break;
		case TT_LEFT_PAR:
			eat(TT_LEFT_PAR);
			ib = procExpression(sb);
			eat(TT_RIGHT_PAR);
			break;
		default:
			showError();
			ib.setType('E');
			break;
	}
	return ib;
}

// <relop> ::= TT_EQUAL | TT_GREATER | TT_GREATER_EQUAL |
//			   TT_LOWER | TT_LOWER_EQUAL | TT_NOT_EQUAL
// First(<relop>) = { TT_EQUAL, TT_GREATER, TT_GREATER_EQUAL,
//			   		  TT_LOWER, TT_LOWER_EQUAL, TT_NOT_EQUAL }
void SyntaticAnalysis::procRelop(/*no param*/) { //no return
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
void SyntaticAnalysis::procAddOp(/*no param*/) { //no return
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
void SyntaticAnalysis::procMulOp(/*no param*/) { //no return
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
SemanticBody SyntaticAnalysis::procConstant(/*no param*/) { //-> SemanticBody
	SemanticBody sb;
	switch(m_current.type) {
		case TT_INTEGER_CONST:
			eat(TT_INTEGER_CONST);
			sb.setType('I');
			break;
		case TT_FLOAT_CONST:
			eat(TT_FLOAT_CONST);
			sb.setType('F');
			break;
		case TT_LITERAL:
			procLiteral();
			sb.setType('S');
			break;
		default:
			showError();
			sb.setType('E');
			break;
	}
	return sb;
}

//<literal> ::= TT_LITERAL
// First(<literal>) = { TT_LITERAL }
void SyntaticAnalysis::procLiteral(/*no param*/) { //no return
	eat(TT_LITERAL);
}

// <identifier> ::= TT_ID
// First(<identifier>) = { TT_ID }
SemanticBody SyntaticAnalysis::procIdentifier(SemanticBody sb) { //-> SemanticBody
	SemanticBody ib;
	if (sb.getType() == 'C') {
		//must find in symbol table
		ib.setType(m_types.find(m_current.token));
		std::cout << "check:" << m_types.find(m_current.token) << std::endl;
	} else {
		//must add in symbol table

		//must check if it exists first
		if (!m_types.contains(m_current.token)) {
			ib = sb;
			m_types.insertType(m_current.token, ib.getType());
			std::cout << "insert of: " << m_current.token << " as " << ib.getType() << std::endl;
		} else {
			//double declaration error
			ib.setType('E');
			std::cout << "semantic error double declaration of: "<< m_current.token << std::endl;
		}
	}
	eat(TT_ID);
	return ib;
}
