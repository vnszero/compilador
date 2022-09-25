#include <iostream>

#include <string>
#include <cctype>
#include "LexicalAnalysis.h"

#define START_STATE 1
#define COMMENT_STATE 2
#define MULTI_LINE_COMMENT_STATE 3
#define MULTI_LINE_END_COMMENT_STATE 4
#define ONE_LINE_COMMENT_STATE 5
#define AND_LINE_STATE 6
#define OR_LINE_STATE 7
#define EQUAL_ASSIGN_LINE_STATE 8
#define GREATER_GREATER_EQUAL_LINE_STATE 9
#define LOWER_LOWER_EQUAL_NOT_EQUAL_LINE_STATE 10
#define KEYWORDS_IDS_LINE_STATE 11
#define NUMBER_LINE_STATE 12
#define FLOAT_DOT_STATE 13
#define FLOAT_DECIMAL_STATE 14
#define LITERAL_LINE_STATE 15
#define ST_FIND_STATE 16
#define ERROR_STATE 17
#define DEFAULT_END_STATE 18

LexicalAnalysis::LexicalAnalysis(const char* filename) : m_line(1) {
	m_input = fopen(filename, "r");
	if (!m_input)
		throw std::string("Unable to open file");
}

LexicalAnalysis::~LexicalAnalysis() {
	if (m_input != nullptr)
		fclose(m_input);
}

Lexeme LexicalAnalysis::nextToken() {
	int state;
	Lexeme lex;

	state = START_STATE;
	while (state != DEFAULT_END_STATE
				&& state != ERROR_STATE
				&& state != ST_FIND_STATE) {

		int c = fgetc(m_input);

	// 	// std::cout << "[" << state << ", " << c
	// 	//           << " ('" << (char) c << "')]" << std::endl;

		switch (state) {
			case START_STATE: // Start point

				// Comment and unused flow
				if (c == ' ' || c == '\t' || c == '\r') {
					state = START_STATE;
				} else if (c == '\n') {
					m_line++;
					state = START_STATE;
				} else if (c == '/') {
					state = COMMENT_STATE;
				}

				// Arithmetic operators flow
				else if (c == '+' || c == '/' || c == '*' || c == '-') {
					lex.token += (char) c;
					state = ST_FIND_STATE;
				}

				// Logic operators
				else if (c == '&') {
					lex.token += (char) c;
					state = AND_LINE_STATE;
				} else if (c == '!') {
					lex.token += (char) c;
					state = ST_FIND_STATE;
				} else if ( c == '|') {
					lex.token += (char) c;
					state = OR_LINE_STATE;
				}

				// Relational operator except ==
				else if (c == '>') { // GREATER or GREATER_EQUAL
					lex.token += (char) c;
					state = GREATER_GREATER_EQUAL_LINE_STATE;
				} else if (c == '<') { // LOWER or LOWER_EQUAL or NOT_EQUAL
					lex.token += (char) c;
					state = LOWER_LOWER_EQUAL_NOT_EQUAL_LINE_STATE;
				}

				// Symbols
				else if (c == '=') { // EQUAL or ASSIGN
					lex.token += (char) c;
					state = EQUAL_ASSIGN_LINE_STATE;
				} else if (c == ',' || c == '(' || c == ')' || c == ';') {
					lex.token += (char) c;
					state = ST_FIND_STATE;
				}

				// Zebras - Keywords or ids
				else if (c == '_' || isalpha(c)) {
					lex.token += (char) c;
					state = KEYWORDS_IDS_LINE_STATE;
				}

				// Zebras - Literals
				else if (c == '{') {
					lex.token += (char) c;
					state = LITERAL_LINE_STATE;
				}

				// Zebras - Numbers
				else if (isdigit(c)) {
					lex.token += (char) c;
					state = NUMBER_LINE_STATE;
				}

				// end of file or invalid token
				else {
					if (c == -1) {
						lex.type = TT_END_OF_FILE;
						state = DEFAULT_END_STATE;
					} else {
						lex.token += (char) c;
						lex.type = TT_INVALID_TOKEN;
						state = ERROR_STATE;
					}
				}
				break;
			case COMMENT_STATE: // Start comment machine
				if (c == '*') {
					state = MULTI_LINE_COMMENT_STATE;
				} else if (c == '/') {
					state = ONE_LINE_COMMENT_STATE;
				} else {
					if (c == -1) {
						lex.type = TT_UNEXPECTED_EOF;
						state = ERROR_STATE;
					} else {
						lex.type = TT_INVALID_TOKEN;
						state = ERROR_STATE;
					}
				}
				break;
			case MULTI_LINE_COMMENT_STATE: // Multiline comment flow
				if (c == '*') {
					state = MULTI_LINE_END_COMMENT_STATE;
				} else if(c == '\n'){
					m_line++;
					state = MULTI_LINE_COMMENT_STATE;
				} else if(c == -1) {
					lex.type = TT_END_OF_FILE;
					state = DEFAULT_END_STATE;
				} else {
					state = MULTI_LINE_COMMENT_STATE;
				}
				break;
			case MULTI_LINE_END_COMMENT_STATE:	// End multiline comment flow
				if (c == '/') {
					state = START_STATE;
				} else if(c == -1) {
					lex.type = TT_END_OF_FILE;
					state = DEFAULT_END_STATE;
				} else {
					state = MULTI_LINE_COMMENT_STATE;
				}
				break;
			case ONE_LINE_COMMENT_STATE: // One line comment flow
				if (c == '\n') {
					m_line++;
					state = START_STATE;
				} else if(c == -1) {
					lex.type = TT_END_OF_FILE;
					state = DEFAULT_END_STATE;
				} else {
					state = ONE_LINE_COMMENT_STATE;
				}
				break;
			case AND_LINE_STATE: // And flow
				if (c == '&') {
					lex.token += (char) c;
					state = ST_FIND_STATE;
				} else {
					if (c == -1) {
						lex.type = TT_UNEXPECTED_EOF;
						state = ERROR_STATE;
					} else {
						lex.type = TT_INVALID_TOKEN;
						state = ERROR_STATE;
					}
				}
				break;
			case OR_LINE_STATE:	// Or flow
				if (c == '|') {
					lex.token += (char) c;
					state = ST_FIND_STATE;
				} else {
					if (c == -1) {
						lex.type = TT_UNEXPECTED_EOF;
						state = ERROR_STATE;
					} else {
						lex.type = TT_INVALID_TOKEN;
						state = ERROR_STATE;
					}
				}
				break;
			case GREATER_GREATER_EQUAL_LINE_STATE: // Greater or greater equal
				if (c == '=') {
					lex.token += (char) c;
				} else {
					if (c != -1)
						ungetc(c, m_input);
				}
				state = ST_FIND_STATE;
				break;
			case LOWER_LOWER_EQUAL_NOT_EQUAL_LINE_STATE: // Lower or lower equal or not equal
				if (c == '=') {
					lex.token += (char) c;
				} else if (c == '>') {
					lex.token += (char) c;
				} else {
					if (c != -1)
						ungetc(c, m_input);
				}
				state = ST_FIND_STATE;
				break;
			case EQUAL_ASSIGN_LINE_STATE: // Equal or assign flow
				if (c == '=') {
					lex.token += (char) c;
				} else {
					if (c != -1)
						ungetc(c, m_input);
				}
				state = ST_FIND_STATE;
				break;
			case KEYWORDS_IDS_LINE_STATE: // Keywords or ids flow
				if(c == '_' || isalpha(c) || isdigit(c)){
					lex.token += (char) c;
					state = KEYWORDS_IDS_LINE_STATE;
				} else {
					if (c != -1)
						ungetc(c, m_input);
					state = ST_FIND_STATE;
				}
				break;
			case LITERAL_LINE_STATE: // begin of a literal
				if ((c >= 0 && c <= 9)
						|| (c >= 11 && c <= 122)
						|| (c == 124)
						|| (c >= 126 && c <= 255)) { // any ASCII except \n { }

					lex.token += (char) c;
					state = LITERAL_LINE_STATE;
				} else if ( c == '}') {
					lex.token += (char) c;
					lex.type = TT_LITERAL;
					state = DEFAULT_END_STATE;
				} else {
					lex.type = TT_INVALID_TOKEN;
					state = ERROR_STATE;
				}
				break;
			case NUMBER_LINE_STATE: // Numerical flow
				if (isdigit(c)) {
					lex.token += (char) c;
					state = NUMBER_LINE_STATE;
				} else if (c == '.') { // its a float
					lex.token += (char) c;
					state = FLOAT_DOT_STATE;
				} else {
					if (c != -1)
						ungetc(c, m_input);
					lex.type = TT_INTEGER_CONST;
					state = DEFAULT_END_STATE;
				}
				break;
			case FLOAT_DOT_STATE: // After dot float flow
				if(isdigit(c)) {
					lex.token += (char) c;
					state = FLOAT_DECIMAL_STATE;
				} else {
					if (c == -1) {
						lex.type = TT_UNEXPECTED_EOF;
						state = ERROR_STATE;
					} else {
						lex.token += (char) c;
						lex.type = TT_INVALID_TOKEN;
						state = ERROR_STATE;
					}
				}
				break;
			case FLOAT_DECIMAL_STATE: // Decimal float flow
				if(isdigit(c)) {
					lex.token += (char) c;
					state = FLOAT_DECIMAL_STATE;
				} else {
					if (c != -1)
						ungetc(c, m_input);
					lex.type = TT_FLOAT_CONST;
					state = DEFAULT_END_STATE;
				}
				break;
			default:
				throw std::string("invalid state");
		}
	}

	if (state == ST_FIND_STATE)
		lex.type = m_st.find(lex.token);

	return lex;
}
