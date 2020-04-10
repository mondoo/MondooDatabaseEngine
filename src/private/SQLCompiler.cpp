#include "../public/SQLCompiler.h"

#include <fmt/format.h>

#include "../public/Table.h"
#include "../public/QueryBuilder.h"

std::string SQLCompiler::CompileSelect(QueryBuilder& query)
{
	return fmt::format("SELECT {} FROM {}", query.GetColumns(), query.GetTable());
}