#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include "SemanticBody.h"
#include "TypeTable.h"
#include <string>
#include <sstream>
#include <iostream>

class SemanticAnalysis {
	public:
		SemanticAnalysis();
		virtual ~SemanticAnalysis();

		// Semantic rulers
		char semanticSerch(std::string token);
		SemanticBody semanticTypeAnalysis(SemanticBody left, SemanticBody right, int line);
		SemanticBody semanticDeclarationAnalysis(std::string token, int line, SemanticBody sb);

	private:
		TypeTable m_types;
};

#endif
