#pragma once

#include <stdio.h>
#include <string>

class SQL
{
public:
	SQL() = default;
	SQL(const std::string& database);

	~SQL();

	bool OpenDB(const std::string& database);

	void CloseDB();

	static void Exec(const std::string& sql, int (*fn)(void*, int, char**, char**) = NULL, void* objectPtr = nullptr);

private:
	static void PrintError(const char* Msg, const char* errMsg);

protected:
	static struct sqlite3* m_db;
};
