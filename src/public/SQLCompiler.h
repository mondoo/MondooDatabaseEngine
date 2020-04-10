#pragma once

#include <string>

class QueryBuilder;

class SQLCompiler
{
public:
	static std::string CompileSelect(QueryBuilder& query);
};