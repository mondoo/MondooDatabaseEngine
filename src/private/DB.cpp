#include "../public/DB.h"

#include "../public/Table.h"
#include "../public/StringUtility.h"

int DB::m_insertIDOut = -1;

DB::DB(const std::string& database)
	: SQL(database)
{}

int DB::Insert(const std::string& table, std::map<std::string, std::string>& insertMap)
{
	std::string key = "";
	std::string value = "";
	StringUtility::GenerateKeyValue(insertMap, key, value);

	Statement(fmt::format("INSERT INTO {} ({}) VALUES ({}); SELECT last_insert_rowid();", table, key, value), InsertCallback);

	int insertIDOut = m_insertIDOut;
	m_insertIDOut = -1;
	return insertIDOut;
}

void DB::Statement(const std::string& statement, int (*callback)(void*, int, char**, char**) /* = NULL */)
{
	Exec(statement, callback);
}

DBTable* DB::Table(const std::string& table)
{
	return new DBTable(table);
}

int DB::InsertCallback(void* instance, int argc, char** argv, char** columnName)
{
	if (argc >= 0)
	{
		m_insertIDOut = atoi(argv[0]);
		return 0;
	}
	return 1;
}

int DB::UpdateCallback(void* instance, int argc, char** argv, char** columnName)
{
	return 0;
}

int DB::DeleteCallback(void* instance, int argc, char** argv, char** columnName)
{
	return 0;
}
