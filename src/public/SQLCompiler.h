#pragma once

#include <string>

class QueryBuilder;

class SQLCompiler
{
public:
	static std::string CompileSelect(QueryBuilder& query);
	static std::string CompileInsert(QueryBuilder& query);
	static std::string CompileUpdate(QueryBuilder& query);
	static std::string CompileDelete(QueryBuilder& query);

	static std::string CompileWhere(std::pair<std::string, std::string>& where);
};