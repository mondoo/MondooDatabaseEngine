#include "../public/SQL.h"
#include <sqlite3.h>

sqlite3* SQL::m_db;

SQL::SQL(const std::string& database)
{
	OpenDB(database);
}

SQL::~SQL()
{
	if (m_db)
		CloseDB();
}

bool SQL::OpenDB(const std::string& database)
{
	int connection = sqlite3_open(database.c_str(), &m_db);
	if (connection)
	{
		PrintError("Can't open database: %s\n", sqlite3_errmsg(m_db));
		return false;
	}

	return true;
}

void SQL::CloseDB()
{
	sqlite3_close(m_db);
}

void SQL::Exec(const std::string& sql, int (*fn)(void*, int, char**, char**), void* objectPtr)
{
	char* errorMsg = 0;
	int response;

	if (m_db == nullptr)
	{
		PrintError("No database connection open.", NULL);
		return;
	}

	if (fn)
	{
		response = sqlite3_exec(m_db, sql.c_str(), fn, objectPtr, &errorMsg);
	}
	else
	{
		response = sqlite3_exec(m_db, sql.c_str(), NULL, objectPtr, &errorMsg);
	}

	if (response != SQLITE_OK)
	{
		PrintError("SQL error: %s\n", errorMsg);
		sqlite3_free(errorMsg);
	}
}

void SQL::PrintError(const char* Msg, const char* errMsg)
{
	fprintf(stderr, Msg, errMsg);
}