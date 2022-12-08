#include "SemanticAnalysis.h"
#include <iomanip>

SemanticAnalysis::SemanticAnalysis() {
}

SemanticAnalysis::~SemanticAnalysis() {
}

char SemanticAnalysis::semanticSerch(std::string token, int line) {
	char analysis = m_types.find(token);
	if(analysis == 'E'){
		std::cout << "\033[1;33m"
			<< "Erro semantico linha "
			<< std::setw(2) << std::setfill('0') << line << ": "
			<< token << " não declarado." << "\033[0m" << std::endl;
	}
	return m_types.find(token);
	// std::cout << "check:" << m_types.find(m_current.token) << std::endl;
}

SemanticBody SemanticAnalysis::semanticTypeAnalysis(SemanticBody left, SemanticBody right, int line) {
	/*
		The order of the SemanticBody parameters care, Lambdas in 2nd place always


	*/

	// std::cout << ib_left.getType() << " == " << ib_right.getType() << std::endl;
	if (right.getType() == 'E') {
		//error message has already been shown
		left.setType('E');
	} else if ((left.getType() == 'I' && right.getType() == 'F') || (left.getType() == 'F' && right.getType() == 'I')){
		std::cout << "\033[1;35m"
			<< "Aviso "
			<< std::setw(2) << std::setfill('0') << line
			<< ": Esperado: " << m_types.getFullNameOfType(left.getType())
			<< " Encontrado: " << m_types.getFullNameOfType(right.getType())
			<< "\033[0m" << std::endl;
		left.setType('F');
	} else if (left.getType() != right.getType() && right.getType() != 'L' && left.getType() != 'E') {
		std::cout << "\033[1;33m"
			<< "Erro semantico linha "
			<< std::setw(2) << std::setfill('0') << line
			<< ": Esperado: " << m_types.getFullNameOfType(left.getType())
			<< " Encontrado: " << m_types.getFullNameOfType(right.getType())
			<< "\033[0m" << std::endl;
		left.setType('E');
	}
	return left;
}

SemanticBody SemanticAnalysis::semanticDeclarationAnalysis(std::string token, int line, SemanticBody sb) {
	SemanticBody ib;
	//must check if it exists first
	if (!m_types.contains(token)) {
		ib = sb;
		m_types.insertType(token, ib.getType());
		// std::cout << "insert of: " << m_current.token << " as " << ib.getType() << std::endl;
	} else {
		//double declaration error
		ib.setType('E');
		std::cout << "\033[1;33m"
			<< "Erro semantico linha "
			<< std::setw(2) << std::setfill('0') << line
			<< ": Declaracao dupla de: " << token
			<< "\033[0m" << std::endl;
	}
	return ib;
}
