#ifndef SEMANTIC_BODY_H
#define SEMANTIC_BODY_H

class SemanticBody {
	public:
		SemanticBody();
		virtual ~SemanticBody();

		// getter and setter for all properties
		void setType(char type); //I, F, S, E, L, C by default
		char getType();

	private:
		// here comes all semantic properties
		char m_type;
		/*
			identifiers:
				I - Integer
				F - Float
				S - String/Literal

			support:
				C - Control
				E - Error
				L - Lambda
		*/
};

#endif
