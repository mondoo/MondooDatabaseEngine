#include "../public/QueryBuilder.h"
#include "../public/StringUtility.h"
#include "../public/SQLCompiler.h"

#include <stdio.h>
#include <fmt/format.h>

QueryBuilder* QueryBuilder::Select(std::vector<std::string> columns /* = {} */)
{
	if (SetQueryType(QueryType::Select))
	{
		return this;
	}

	m_type = QueryType::Select;
	m_typeIsSet = true;

	m_columns.clear();

	if (columns.size() == 0)
	{
		m_columns.push_back("*");
	}
	else
	{
		for (const std::string& column : columns)
		{
			m_columns.push_back(column);
		}
	}

	return this;
}

const std::string& QueryBuilder::GetTable()
{
	return m_table;
}

const std::string QueryBuilder::GetColumns()
{
	return WrapParts(m_columns);
}

std::string QueryBuilder::Get()
{
	switch (m_type)
	{
	case QueryType::Select:
		return SQLCompiler::CompileSelect(*this);
	default:
		return "";
	}
}

bool QueryBuilder::SetQueryType(QueryType type)
{
	if (m_typeIsSet)
	{
		printf("Error query type is already set");
		return true;
	}

	m_type = QueryType::Select;
	m_typeIsSet = true;

	return false;
}

const std::string QueryBuilder::Wrap(const std::string& value)
{
	if (value.find(" as ") != 0)
	{
		return value;
	}

	return WrapParts(StringUtility::Split(value, '.'));
}

const std::string QueryBuilder::WrapParts(std::vector<std::string>& parts)
{
	return StringUtility::Join(parts, ',');
}