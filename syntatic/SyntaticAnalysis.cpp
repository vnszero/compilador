#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "SyntaticAnalysis.h"

SyntaticAnalysis::SyntaticAnalysis(LexicalAnalysis& lex):
	m_lex(lex), m_current(lex.nextToken()) {
}
SyntaticAnalysis::~SyntaticAnalysis() {
}
void SyntaticAnalysis::start(){

}

void SyntaticAnalysis::advance() {
	m_current = m_lex.nextToken();
}

void SyntaticAnalysis::eat(enum TokenType type) {
	std::cout << "Expected (..., " << tt2str(type)
			  << "), found (\"" + m_current.token << "\", "
			  << tt2str(m_current.type) << ")";

	if (type == m_current.type) {
		advance();
	} else {
		showError();
	}
}

void SyntaticAnalysis::showError() {
	std::cout << std::setw(2) << std::setfill('0') << m_lex.line() << std::endl;

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
