#pragma once

#include <string>

#include "Types.h"

class QueryBuilder;

class SQLCompiler
{
public:
	static std::string CompileSelect(QueryBuilder& query);
	static std::string CompileInsert(QueryBuilder& query);
	static std::string CompileUpdate(QueryBuilder& query);
	static std::string CompileDelete(QueryBuilder& query);

	static std::string CompileWhere(bool hasWhere, std::pair<std::string, ValueType>& where);
	static std::string SQLCompiler::CompileOrderBy(QueryBuilder& query);
};