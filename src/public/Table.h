#pragma once

#include <string>
#include <map>

class DBTable
{
public:
	DBTable(const std::string& table);

	int Insert(std::map<std::string, std::string>& insertDictonary);

private:
	std::string m_table;
}; 