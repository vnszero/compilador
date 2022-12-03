#include "TypeTable.h"

TypeTable::TypeTable() {
	//identifiers
	m_full_name_of_types['I'] = "Integer";
	m_full_name_of_types['F'] = "Float";
	m_full_name_of_types['S'] = "String";

	//support
	m_full_name_of_types['E'] = "Error";
	m_full_name_of_types['L'] = "Lambda";
	m_full_name_of_types['C'] = "Control";
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

std::string TypeTable::getFullNameOfType(char type) {
	return m_full_name_of_types[type];
}
