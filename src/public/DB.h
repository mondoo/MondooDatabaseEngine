#pragma once

#include <fmt/format.h>
#include <string>
#include <map>

#include "SQL.h"

class DB : public SQL
{
public:
	DB() = default;
	DB(const std::string& database)
		: SQL(database)
	{}

	template<typename... Args>
	void Select(Args... args)
	{
		Statement(fmt::format("SELECT * FROM {} WHERE {} = {}", args...));
	}

	int Insert(const std::string& table, std::map<std::string, std::string>& insertMap)
	{
		std::string key = "";
		std::string value = "";
		GenerateKeyValue(insertMap, key, value);

		auto callback = [](void* instance, int argc, char** argv, char** colName) {
			return static_cast<DB*>(instance)->InsertCallback(instance, argc, argv, colName);
		};

		Statement(fmt::format("INSERT INTO {} ({}) VALUES ({}); SELECT last_insert_rowid();", table, key, value), callback, this);

		int insertIDOut = m_insertIDOut;
		m_insertIDOut = -1;
		return insertIDOut;
	}

	template<typename... Args>
	void Update(Args... args)
	{
		auto callback = [](void* instance, int argc, char** argv, char** colName) {
			return static_cast<DB*>(instance)->UpdateCallback(instance, argc, argv, colName);
		};

		Statement(fmt::format("UPDATE {} SET {} = {} WHERE {}={}", args...), callback, this);
	}

	template<typename... Args>
	void Delete(Args... args)
	{
		auto callback = [](void* instance, int argc, char** argv, char** colName) {
			return static_cast<DB*>(instance)->DeleteCallback(instance, argc, argv, colName);
		};

		Statement(fmt::format("DELETE FROM {} WHERE {}={}", args...), callback, this);
	}

	void Statement(const std::string& statement, int (*callback)(void*, int, char**, char**) = NULL, void* self = nullptr)
	{
		Exec(statement, callback, self);
	}

private:
	int InsertCallback(void* instance, int argc, char** argv, char** columnName)
	{
		if (argc >= 0)
		{
			m_insertIDOut = atoi(argv[0]);
			return 0;
		}
		return 1;
	}

	int UpdateCallback(void* instance, int argc, char** argv, char** columnName)
	{
		return 0;
	}

	int DeleteCallback(void* instance, int argc, char** argv, char** columnName)
	{
		return 0;
	}

	void GenerateKeyValue(std::map<std::string, std::string>& insertMap, std::string& outKey, std::string& outValue)
	{
		bool first = true;
		for (std::map<std::string, std::string>::iterator it = insertMap.begin(); it != insertMap.end(); ++it)
		{
			if (first)
			{
				outKey += it->first;
				outValue += it->second;
				first = false;
			}
			else
			{
				outKey += ", " + it->first;
				outValue += ", " + it->second;
			}
		}
	}

private:
	int m_insertIDOut = -1;
};