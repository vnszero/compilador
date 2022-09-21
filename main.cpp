#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include "lexical/LexicalAnalysis.h"

int main(int argc, char const *argv[])
{
	if(argc!=2){
		std::cout << "usage: "<<argv[0] << " [program]" << std::endl;
		exit(1);
	}
	
	try{
		LexicalAnalysis l(argv[1]);
		std::cout << l.nextToken().str() << std::endl;
	}catch(const std::string& error){
		std::cerr << error << std::endl;
	}

	return 0;
}
