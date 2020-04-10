#pragma once

#include <unordered_map>
#include <vector>
#include <string>

enum class QueryType : uint8_t
{
	Select,
	Insert,
	Update,
	Delete
};

class QueryBuilder
{
public:
	QueryBuilder* Select(std::vector<std::string> columns = {});

	const std::string& GetTable();

	const std::string GetColumns();

	std::string Get();

private:
	bool SetQueryType(QueryType type);

	const std::string Wrap(const std::string& value);

	const std::string WrapParts(std::vector<std::string>& parts);

protected:
	QueryType m_type;
	bool m_typeIsSet = false;

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

	std::string m_table;
};