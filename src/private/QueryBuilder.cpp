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

QueryBuilder* QueryBuilder::Insert(std::map<std::string, std::string> values /* = std::map<std::string, std::string>() */)
{
	if (SetQueryType(QueryType::Insert))
	{
		return this;
	}

	for (std::map<std::string, std::string>::iterator it = values.begin(); it != values.end(); ++it)
	{
		m_columns.push_back(it->first);
		m_values.push_back(it->second);
	}

	return this;
}

QueryBuilder* QueryBuilder::Insert(std::vector<std::string> values /* = std::vector<std::string>() */)
{
	if (SetQueryType(QueryType::Insert))
	{
		return this;
	}

	m_values = values;

	return this;
}

QueryBuilder* QueryBuilder::Update(std::map<std::string, std::string> values)
{
	if (SetQueryType(QueryType::Update))
	{
		return this;
	}

	for (std::map<std::string, std::string>::iterator it = values.begin(); it != values.end(); ++it)
	{
		m_columns.push_back(it->first);
		m_values.push_back(it->second);
	}

	return this;
}

QueryBuilder* QueryBuilder::Delete()
{
	if (SetQueryType(QueryType::Delete))
	{
		return this;
	}

	return this;
}

QueryBuilder* QueryBuilder::Where(std::pair<std::string, std::string> values)
{
	m_where = values;
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

const std::vector<std::string>& QueryBuilder::GetColumnsRaw()
{
	return m_columns;
}

const std::string QueryBuilder::GetValues()
{
	return WrapParts(m_values);
}

const std::vector<std::string>& QueryBuilder::GetValuesRaw()
{
	return m_values;
}

const std::map<std::string, std::string> QueryBuilder::GetValueMap()
{
	std::map<std::string, std::string> valueMap;

	const size_t columnCount = GetColumnsRaw().size();
	const size_t valueCount = GetValuesRaw().size();

	for (size_t i = 0; i < columnCount; i++)
	{
		if (i >= columnCount || i >= valueCount)
		{
			break;
		}

		valueMap.insert_or_assign(GetColumnsRaw()[i], GetValuesRaw()[i]);
	}

	return valueMap;
}

std::pair<std::string, std::string>& QueryBuilder::GetWhere()
{
	return m_where;
}

std::string QueryBuilder::Get()
{
	std::string sql;
	switch (m_type)
	{
	case QueryType::Select:
		sql = SQLCompiler::CompileSelect(*this);
		break;
	case QueryType::Insert:
		sql = SQLCompiler::CompileInsert(*this);
		break;
	case QueryType::Update:
		sql = SQLCompiler::CompileUpdate(*this);
		break;
	case QueryType::Delete:
		sql = SQLCompiler::CompileDelete(*this);
		break;
	default:
		return "";
	}

	return sql;
}

bool QueryBuilder::SetQueryType(QueryType type)
{
	if (m_type != QueryType::None)
	{
		printf("Error query type is already set");
		return true;
	}

	m_type = type;

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

void QueryBuilder::Reset()
{
	m_type = QueryType::None;
	m_columns.clear();
	m_values.clear();
}