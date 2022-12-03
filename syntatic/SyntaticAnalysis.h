#ifndef SYNTATIC_ANALYSIS_H
#define SYNTATIC_ANALYSIS_H

#include "../lexical/LexicalAnalysis.h"
#include "../semantic/SemanticAnalysis.h"
#include "../semantic/SemanticBody.h"
#include "../semantic/TypeTable.h"

class SyntaticAnalysis {
	public:
		SyntaticAnalysis(LexicalAnalysis& lex, SemanticAnalysis& sem);
		virtual ~SyntaticAnalysis();

		void start();
	private:
		LexicalAnalysis& m_lex;
		SemanticAnalysis& m_sem;
		Lexeme m_current;
		void advance();
		void eat(enum TokenType type);
		void showLexemeSyntaticError();

		/* TP base */
		// <program> ::= TT_START [ <decllist> ] <stmtlist> TT_EXIT
		void procProgram(/*no param*/); //no return

		// <decllist> ::= <decl> { <decl> }
		void procDeclList(/*no param*/); //no return

		// <decl> ::= <type> <identlist> TT_SEMICOLON
		void procDecl(/*no param*/); //no return

		// <identlist> ::= <identifier> { TT_COMMA <identifier> }
		void procIdentList(SemanticBody sb); //no return

		// <type> ::= TT_INT | TT_FLOAT | TT_STRING
		SemanticBody procType(/*no param*/); //-> SemanticBody

		// <stmtlist> ::= <stmt> { <stmt> }
		void procStmtList(/*no param*/); //no return

		// <stmt> ::= <assignstmt> TT_SEMICOLON |
		//				<ifstmt> | <whilestmt> |
		//				<readstmt> TT_SEMICOLON |
		//				<writestmt> TT_SEMICOLON
		void procStmt(/*no param*/); //no return

		// <assignstmt> ::= <identifier> TT_ASSIGN <simpleexpr>
		void procAssignStmt(/*no param*/); //no return

		// <ifstmt> ::= TT_IF <condition> TT_THEN <stmtlist> <ifstmtprime>
		void procIfStmt(/*no param*/); //no return

		// <ifstmtprime> ::= TT_END | TT_ELSE <stmtlist> TT_END
		void procIfStmtPrime(/*no param*/); //no return

		// <condition> ::= <expression>
		void procCondition(/*no param*/); //no return

		// <whilestmt> ::= TT_DO <stmtlist> <stmtsufix>
		void procWhileStmt(/*no param*/); //no return

		// <stmtsufix> ::= TT_WHILE <condition> TT_END
		void procStmtSufix(/*no param*/); //no return

		// <readstmt> ::= TT_SCAN TT_LEFT_PAR <identifier> TT_RIGHT_PAR
		void procReadStmt(/*no param*/); //no return

		// <writestmt> ::= TT_PRINT TT_LEFT_PAR <writable> TT_RIGHT_PAR
		void procWriteStmt(/*no param*/); //no return

		// <writable> ::= <simpleexpr> | <literal>
		void procWritable(/*no param*/); //no return

		// <expression> ::= <simpleexpr> <expressionprime>
		SemanticBody procExpression(SemanticBody sb); //-> SemanticBody

		// <expressionprime> ::= <relop> <simpleexpr> <expressionprime>
		SemanticBody procExpressionPrime(SemanticBody sb); //-> SemanticBody

		// <simpleexpr> ::= <term> <simpleexprprime>
		SemanticBody procSimpleExpr(SemanticBody sb); //-> SemanticBody

		// <simpleexprprime> ::= <addop> <term> <simpleexprprime> | LAMBDA
		SemanticBody procSimpleExprPrime(SemanticBody sb); //-> SemanticBody

		// <term> ::= <fatora> <termprime>
		SemanticBody procTerm(SemanticBody sb); //-> SemanticBody

		// <termprime> ::= <mulop> <fatora> <termprime> | LAMBDA
		SemanticBody procTermPrime(SemanticBody sb); //-> SemanticBody

		// <fatora> ::= [ TT_NOT | TT_SUB ] <factor>
		SemanticBody procFatora(SemanticBody sb); //-> SemanticBody

		// <factor> ::= <identifier> | <constant> | TT_LEFT_PAR <expression> TT_RIGHT_PAR
		SemanticBody procFactor(SemanticBody sb); //-> SemanticBody

		// <relop> ::= TT_EQUAL | TT_GREATER | TT_GREATER_EQUAL |
		//			   TT_LOWER | TT_LOWER_EQUAL | TT_NOT_EQUAL
		void procRelop(/*no param*/); //no return

		// <addop> ::= TT_ADD | TT_SUB | TT_OR
		void procAddOp(/*no param*/); //no return

		// <mulop> ::= TT_MUL | TT_DIV | TT_AND
		void procMulOp(/*no param*/); //no return

		// <constant> ::= TT_INTEGER_CONST | TT_FLOAT_CONST | TT_LITERAL
		SemanticBody procConstant(/*no param*/); //->sb

		//<literal> ::= TT_LITERAL
		void procLiteral(/*no param*/); //no return

		// <identifier> ::= TT_ID
		SemanticBody procIdentifier(SemanticBody sb); //->sb
};

#endif
