#pragma once

#include <stdio.h>
#include <string>
#include <sqlite3.h>
#include <functional>

class SQL
{
public:
	SQL() = default;
	SQL(const std::string& database)
	{
		OpenDB(database);
	}

	~SQL()
	{
		if (m_db)
			CloseDB();
	}

	bool OpenDB(const std::string& database)
	{
		int connection = sqlite3_open(database.c_str(), &m_db);
		if (connection)
		{
			PrintError("Can't open database: %s\n", sqlite3_errmsg(m_db));
			return false;
		}

		return true;
	}

	void CloseDB()
	{
		sqlite3_close(m_db);
	}

	void Exec(const std::string& sql, int (*fn)(void*, int, char**, char**) = NULL, void* classRef = nullptr)
	{
		char* errorMsg = 0;
		int response;
		
		if (fn)
		{
			typedef int function_t(void*, int, char**, char**);
			response = sqlite3_exec(m_db, sql.c_str(), fn, classRef ? classRef : 0, &errorMsg);
		}
		else
		{
			response = sqlite3_exec(m_db, sql.c_str(), NULL, classRef ? classRef : 0, &errorMsg);
		}

		if (response != SQLITE_OK)
		{
			PrintError("SQL error: %s\n", errorMsg);
			sqlite3_free(errorMsg);
		}
	}

private:
	void PrintError(const char* Msg, const char* errMsg)
	{
		fprintf(stderr, Msg, errMsg);
	}

protected:
	sqlite3* m_db;
};
