#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <map>

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

	QueryBuilder* Insert(std::map<std::string, std::string> values);
	QueryBuilder* Insert(std::vector<std::string> values);

	QueryBuilder* Update(std::map<std::string, std::string> values);

	QueryBuilder* Delete();

	QueryBuilder* Where(std::pair<std::string, std::string> values);

	const std::string& GetTable();

	const std::string GetColumns();
	const std::vector<std::string>& QueryBuilder::GetColumnsRaw();
	const std::string GetValues();
	const std::vector<std::string>& QueryBuilder::GetValuesRaw();

	const std::map<std::string, std::string> GetValueMap();

	std::pair<std::string, std::string>& GetWhere();

	std::string Get();

private:
	bool SetQueryType(QueryType type);

	const std::string Wrap(const std::string& value);

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
	std::vector<std::string> m_values = {};

	std::pair<std::string, std::string> m_where = {};

	std::string m_table;
};