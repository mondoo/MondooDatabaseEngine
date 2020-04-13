#include "SQLCompiler.h"

#include <fmt/format.h>

#include "Table.h"
#include "QueryBuilder.h"

std::string SQLCompiler::CompileSelect(QueryBuilder& query)
{
	return fmt::format("SELECT {} FROM {}{}{}", query.GetColumns(), query.GetTable(), CompileWhere(query.HasWhere(), query.GetWhere()), query.HasOrderBy() ? CompileOrderBy(query) : "");
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
	std::map<std::string, ValueType> valueMap = query.GetValueMap();

	std::string update = "";
	bool first = true;
	for (const std::pair<std::string, ValueType> pair : valueMap)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			update += ", ";
		}

		update += "`" + pair.first + "` = " + fmt::format(pair.second.m_wrap ? "'{}'" : "{}", pair.second.m_value);
	}

	return fmt::format("UPDATE {} SET {}{}", query.GetTable(), update, CompileWhere(query.HasWhere(), query.GetWhere()));
}

std::string SQLCompiler::CompileDelete(QueryBuilder& query)
{
	return fmt::format("DELETE FROM {}{}", query.GetTable(), CompileWhere(query.HasWhere(), query.GetWhere()));
}

std::string SQLCompiler::CompileWhere(bool hasWhere, std::pair<std::string, ValueType>& where)
{
	return hasWhere ? fmt::format(" WHERE `{}` = {}", where.first, where.second.m_value) : "";
}

std::string SQLCompiler::CompileOrderBy(QueryBuilder& query)
{
	std::string orderBy;
	if (query.HasOrderBy())
	{
		orderBy += " ORDER BY ";
		bool first = true && query.GetOrderBy().size() > 1;
		for (const KeyValuePair& pair : query.GetOrderBy())
		{
			orderBy += fmt::format("`{}` {}{}", pair.m_key, pair.m_value.m_value, first ? ", " : "");
			if (first) first = false;
		}
	}

	return orderBy;
}