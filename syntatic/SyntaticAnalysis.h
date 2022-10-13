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
};

#endif
