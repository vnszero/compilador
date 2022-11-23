
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

	private:
		std::map<std::string, char> m_types;
};

#endif
