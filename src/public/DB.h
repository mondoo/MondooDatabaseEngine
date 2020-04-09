#pragma once

#include <fmt/format.h>
#include <string>
#include "SQL.h"
#include <map>

#include <iostream>

#include <stdio.h>

#pragma optimize("", off)

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

	void Insert(const std::string& table, std::map<std::string, std::string>& insertMap)
	{
		std::string key = "";
		std::string value = "";
		bool first = true;
		for (std::map<std::string, std::string>::iterator it = insertMap.begin(); it != insertMap.end(); ++it)
		{
			if (first)
			{
				key += it->first;
				value += it->second;
				first = false;
			}
			else
			{
				key += ", " + it->first;
				value += ", " + it->second;
			}
		}

		auto CB = [](void* instance, int argc, char** argv, char** colName) {
			std::cout << "Hello world!" << std::endl;
			if (argc >= 0)
			{
				return static_cast<DB*>(instance)->PrintUpdate(argc, argv, colName);
			}
			return -1;
		};

		Statement(fmt::format("INSERT INTO {} ({}) VALUES ({}); SELECT last_insert_rowid();", table, key, value), CB, this);

		Statement("SELECT * FROM test", CB, this);
		
	}

	int PrintUpdate(int outCount, char** outArgv, char** outColName)
	{
		int i = 0;
		for (i = 0; i < outCount; i++)
		{
			printf("%s = %s\n", outColName[i], outArgv[i] ? outArgv[i] : "NULL");
		}

		return 0;
	}

	void Update()
	{

	}

	void Delete()
	{

	}

	void Statement(const std::string& statement, int (*fn)(void*, int, char**, char**) = NULL, void* classPtr = nullptr)
	{
		Exec(statement, fn, classPtr);
	}
};