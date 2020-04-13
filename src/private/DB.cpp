#include "DB.h"

#include "Table.h"
#include "StringUtility.h"

int DB::m_insertIDOut = -1;

DB::DB(const std::string& database)
	: SQL(database)
{}

void DB::Select(const std::string& sql)
{
	Statement(sql, SelectCallback);
}

void DB::SelectFill(const std::string& sql, int (*callback)(void*, int, char**, char**), void* objectPtr)
{
	Statement(sql, callback, objectPtr);
}

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

int DB::Insert(const std::string& sql)
{
	Statement(sql + "SELECT last_insert_rowid();" , InsertCallback);

	int insertIDOut = m_insertIDOut;
	m_insertIDOut = -1;
	return insertIDOut;
}

void DB::Statement(const std::string& statement, int (*callback)(void*, int, char**, char**), void* objectPtr)
{
	Exec(statement, callback, objectPtr);
}

DBTable* DB::Table(const std::string& table)
{
	return new DBTable(table);
}

int DB::SelectCallback(void* instance, int count, char** data, char** column)
{
	if (count > 0)
	{
		for (size_t i = 0; i < count; i++)
		{
			printf("| %s ", column[i]);
		}
		printf("\n");

		for (size_t i = 0; i < count; i++)
		{
			printf("| %s ", data[i]);
		}

		printf("\n");
		return 0;
	}
	return 1;
}

int DB::InsertCallback(void* instance, int count, char** data, char** column)
{
	if (count > 0)
	{
		printf("Last Insert ID: %s\n", data[0]);
		m_insertIDOut = atoi(data[0]);
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
