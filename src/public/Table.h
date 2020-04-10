#pragma once

#include <string>
#include <map>

#include "../public/QueryBuilder.h"

class DBTable : public QueryBuilder
{
public:
	DBTable(const std::string& table);

	int Insert(std::map<std::string, std::string>& insertDictonary);

}; 