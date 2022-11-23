#include "SemanticBody.h"

SemanticBody::SemanticBody() {
	m_type = 'C';
}

SemanticBody::~SemanticBody() {
}

void SemanticBody::setType(char type) {
	m_type = type;
}

char SemanticBody::getType() {
	return m_type;
}
