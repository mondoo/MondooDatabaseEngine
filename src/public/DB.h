#pragma once

#include <fmt/format.h>
#include <string>
#include <map>

#include "SQL.h"

class DBTable;

class DB : public SQL
{
public:
	DB() = default;
	DB(const std::string& database);

	template<typename... Args>
	static void Select(Args... args);

	static int Insert(const std::string& table, std::map<std::string, std::string>& insertMap);
	static int Insert(const std::string& sql);

	template<typename... Args>
	static void Update(Args... args);

	template<typename... Args>
	static void Delete(Args... args);

	static void Statement(const std::string& statement, int (*callback)(void*, int, char**, char**) = NULL);

	static DBTable* Table(const std::string& table);

	// Callbacks
	static int InsertCallback(void* instance, int count, char** data, char** column);
	static int UpdateCallback(void* instance, int argc, char** argv, char** columnName);
	static int DeleteCallback(void* instance, int argc, char** argv, char** columnName);

private:
	static int m_insertIDOut;
};


template<typename... Args>
inline void DB::Select(Args... args)
{
	Statement(fmt::format("SELECT * FROM {} WHERE {} = {}", args...));
}

template<typename... Args>
inline void DB::Update(Args... args)
{
	Statement(fmt::format("UPDATE {} SET {} = {} WHERE {}={}", args...), UpdateCallback, this);
}

template<typename... Args>
inline void DB::Delete(Args... args)
{
	Statement(fmt::format("DELETE FROM {} WHERE {}={}", args...), DeleteCallback, this);
}
