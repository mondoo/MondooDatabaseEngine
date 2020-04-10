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

void SQL::Exec(const std::string& sql, int (*fn)(void*, int, char**, char**) /* = NULL */)
{
	char* errorMsg = 0;
	int response;

	if (fn)
	{
		typedef int function_t(void*, int, char**, char**);
		response = sqlite3_exec(m_db, sql.c_str(), fn, 0, &errorMsg);
	}
	else
	{
		response = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &errorMsg);
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