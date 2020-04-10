#include "../public/Table.h"
#include "../public/DB.h"

DBTable::DBTable(const std::string& table)
{
	m_table = table;
}

int DBTable::Insert(std::map<std::string, std::string>& insertDictonary)
{
	return DB::Insert(m_table, insertDictonary);
}