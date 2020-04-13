#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <map>

#include "Types.h"

enum class QueryType : uint8_t
{
	None,
	Select,
	Insert,
	Update,
	Delete
};

class QueryBuilder
{
public:
	QueryBuilder* Select(std::vector<std::string> columns = {});

	template<typename... Args>
	QueryBuilder* Insert(Args... args);

	QueryBuilder* Insert(const std::vector<KeyValuePair>& values);
	QueryBuilder* Insert(std::vector<ValueType> values);
	QueryBuilder* Update(const std::vector<KeyValuePair>& values);

	QueryBuilder* Delete();

	QueryBuilder* Where(std::pair<std::string, ValueType> values);
	QueryBuilder* OrderBy(std::vector<KeyValuePair> values);

	const std::string& GetTable();

	const std::string GetColumns();
	const std::vector<std::string>& QueryBuilder::GetColumnsRaw();
	const std::string GetValues();
	const std::vector<ValueType>& QueryBuilder::GetValuesRaw();

	const std::map<std::string, ValueType> GetValueMap();

	bool HasWhere();
	std::pair<std::string, ValueType>& GetWhere();
	bool HasOrderBy();
	std::vector<KeyValuePair>& GetOrderBy();

	std::string GetStatement();

	void ExecStatement(int (*callback)(void*, int, char**, char**) = nullptr, void* objectPtr = nullptr, bool printStatement = false);

private:
	bool SetQueryType(QueryType type);

	const std::string Wrap(const ValueType& value);

	const std::string WrapParts(std::vector<ValueType>& parts);
	const std::string WrapParts(std::vector<std::string>& parts);

	void Reset();

protected:
	QueryType m_type = QueryType::None;

	std::unordered_map<std::string, std::vector<std::string>> m_bindings = {
		{"Select", {}},
		{"From", {}},
		{"Join", {}},
		{"Where", {}},
		{"GroupBy", {}},
		{"Order", {}}
	};

	std::vector<std::string> m_operators = {
		"=", "<", ">", "<=", ">=", "<>", "!=", "<=>", "like", "not like", "&", "|"
	};

	std::vector<std::string> m_columns = {};
	std::vector<ValueType> m_values = {};

	bool m_hasWhere = false;
	std::pair<std::string, ValueType> m_where = {};
	bool m_hasOrderBy = false;
	std::vector<KeyValuePair> m_orderBy = {};

	std::string m_table;
};

template<typename... Args>
inline QueryBuilder* QueryBuilder::Insert(Args... args)
{
	if (SetQueryType(QueryType::Insert))
	{
		return this;
	}

	m_values = { ValueType(args)... };

	return this;
}