#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include "lexical/LexicalAnalysis.h"
#include "syntatic/SyntaticAnalysis.h"
#include "semantic/SemanticAnalysis.h"

int main(int argc, char const *argv[])
{
	if(argc!=2){
		std::cout << "usage: "<<argv[0] << " [program]" << std::endl;
		exit(1);
	}

	try{
		Lexeme lex;
		LexicalAnalysis l(argv[1]);
		SemanticAnalysis sem;
		SyntaticAnalysis s(l, sem);
		// Lexical Test
		// while((lex = l.nextToken()).type > 0){
		// 	std::cout << lex.str() << std::endl;
		// }
		// std::cout << lex.str() << std::endl;

		// Syntatic Test
		s.start();

	}catch(const std::string& error){
		std::cerr << error << std::endl;
	}

	return 0;
}
