#include "TypeTable.h"

TypeTable::TypeTable() {
}

TypeTable::~TypeTable() {
}

bool TypeTable::contains(const std::string& token) const {
	return m_types.find(token) != m_types.end();
}

char TypeTable::find(const std::string& token) {
	return this->contains(token) ? m_types[token] : 'E';
}

void TypeTable::insertType(const std::string& token, char type) {
	m_types.insert({token, type});
}
