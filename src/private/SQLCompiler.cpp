#include "../public/SQLCompiler.h"

#include <fmt/format.h>

#include "../public/Table.h"
#include "../public/QueryBuilder.h"

std::string SQLCompiler::CompileSelect(QueryBuilder& query)
{
	return fmt::format("SELECT {} FROM {}", query.GetColumns(), query.GetTable());
}

std::string SQLCompiler::CompileInsert(QueryBuilder& query)
{
	const std::string columns = [&query]() -> const std::string {
		const std::string columns = query.GetColumns();
		if (columns.length() > 0)
		{
			return fmt::format(" ({})", columns);
		}
		return columns;
	}();

	return fmt::format("INSERT INTO {}{} VALUES ({})", query.GetTable(), columns, query.GetValues());
}

std::string SQLCompiler::CompileUpdate(QueryBuilder& query)
{
	std::map<std::string, std::string> valueMap = query.GetValueMap();

	std::string update = "";
	bool first = true;
	for (const std::pair<std::string, std::string> pair : valueMap)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			update += ", ";
		}

		update += pair.first + " = " + pair.second;
	}

	return fmt::format("UPDATE {} SET {} {}", query.GetTable(), update, CompileWhere(query.GetWhere()));
}

std::string SQLCompiler::CompileDelete(QueryBuilder& query)
{
	return fmt::format("DELETE FROM {} {}", query.GetTable(), CompileWhere(query.GetWhere()));
}

std::string SQLCompiler::CompileWhere(std::pair<std::string, std::string>& where)
{
	return fmt::format("WHERE {} = {}", where.first, where.second);
}