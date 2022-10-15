#ifndef SYNTATIC_ANALYSIS_H
#define SYNTATIC_ANALYSIS_H

#include "../lexical/LexicalAnalysis.h"

class SyntaticAnalysis {
	public:
		SyntaticAnalysis(LexicalAnalysis& lex);
		virtual ~SyntaticAnalysis();

		void start();
	private:
		LexicalAnalysis& m_lex;
		Lexeme m_current;
		void advance();
		void eat(enum TokenType type);
		void showError();

		/* TP base */
		// <program> ::= TT_START [ <decllist> ] <stmtlist> TT_EXIT
		void procProgram();

		// <decllist> ::= <decl> { <decl> }
		void procDeclList();

		// <decl> ::= <type> <identlist> TT_SEMICOLON
		void procDecl();

		// <identlist> ::= <identifier> { TT_COMMA <identifier> }
		void procIdentList();

		// <type> ::= TT_INT | TT_FLOAT | TT_STRING
		void procType();

		// <stmtlist> ::= <stmt> { <stmt> }
		void procStmtList();

		// <stmt> ::= <assignstmt> TT_SEMICOLON |
		//				<ifstmt> | <whilestmt> |
		//				<readstmt> TT_SEMICOLON |
		//				<writestmt> TT_SEMICOLON
		void procStmt();

		// <assignstmt> ::= <identifier> TT_ASSIGN <simpleexpr>
		void procAssignStmt();

		// <ifstmt> ::= TT_IF <condition> TT_THEN <stmtlist> TT_END |
		//				TT_IF <condition> TT_THEN <stmtlist> TT_ELSE <stmtlist> TT_END
		void procIfStmt();

		// <condition> ::= <expression>
		void procCondition();

		// <whilestmt> ::= TT_DO <stmtlist> <stmtsufix>
		void procWhileStmt();

		// <stmtsufix> ::= TT_WHILE <condition> TT_END
		void procStmtSufix();

		// <readstmt> ::= TT_SCAN TT_LEFT_PAR <identifier> TT_RIGHT_PAR
		void procReadStmt();

		// <writestmt> ::= TT_PRINT TT_LEFT_PAR <writable> TT_RIGHT_PAR
		void procWriteStmt();

		// <writable> ::= <simpleexpr> | <literal>
		void procWritable();

		// <expression> ::= <simpleexpr> | <simpleexpr> <relop> <simpleexpr>
		void procExpression();

		// <simpleexpr> ::= <term> | <simpleexpr> <addop> <term>
		void procSimpleExpr();

		// <term> ::= <fatora> | <term> <mulop> <fatora>
		void procTerm();

		// <fatora> ::= <factor> | TT_NOT <factor> | TT_SUB <factor>
		void procFatora();

		// <factor> ::= <identifier> | <constant> | TT_LEFT_PAR <expression> TT_RIGHT_PAR
		void procFactor();

		// <relop> ::= TT_EQUAL | TT_GREATER | TT_GREATER_EQUAL |
		//			   TT_LOWER | TT_LOWER_EQUAL | TT_NOT_EQUAL
		void procRelop();

		// <addop> ::= TT_ADD | TT_SUB | TT_OR
		void procAddOp();

		// <mulop> ::= TT_MUL | TT_DIV | TT_AND
		void procMulOp();

		// <constant> ::= TT_INTEGER_CONST | TT_FLOAT_CONST | TT_LITERAL
		void procConstant();

		//<literal> ::= TT_LITERAL
		void procLiteral();

		// <identifier> ::= TT_ID
		void procIdentifier();
};

#endif
