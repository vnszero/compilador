
#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include <map>
#include <string>

class TypeTable {
	public:
		TypeTable();
		virtual ~TypeTable();

		bool contains(const std::string& token) const;
		char find(const std::string& token);
		void insertType(const std::string& token, char type);
		std::string getFullNameOfType(char type);

	private:
		std::map<char, std::string> m_full_name_of_types;
		std::map<std::string, char> m_types;
};

#endif
