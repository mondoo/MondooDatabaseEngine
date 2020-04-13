#include "public/SQL.h"
#include "public/DB.h"
#include "public/Table.h"
#include <stdio.h>
#include <map>
#include <unordered_map>

#include <fmt/format.h>
#include <string>
#include <time.h>
#include <iostream>

#include "public/Reflection.h"
#include "public/Types.h"

std::unordered_map<std::string, Reflection::Struct*> Reflection::StructHelpers::s_typeMaps;

void main()
{
	DB* db = new DB("test.db");

	printf("%s\n", DB::Table("test")->Select({ "hello", "world" })->GetStatement().c_str());
	printf("%s\n", DB::Table("test")->Select()->GetStatement().c_str());

	printf("%s\n", DB::Table("test")->Insert("Hello", 1, "World", 5031)->GetStatement().c_str());
	printf("%s\n", DB::Table("test")->Insert(std::vector<ValueType>{ValueType("Hello"), ValueType(1)})->GetStatement().c_str());
	printf("%s\n", DB::Table("test")->Insert(std::vector<KeyValuePair>{
		{"Hello", "World"},
		{"Test 1", "Test 2"},
		{"Test 2", 10}
	})->GetStatement().c_str());

	printf("%s\n", DB::Table("test")->Update(std::vector<KeyValuePair>{
		{"Hello", "World"},
		{ "Test 1", "Test 2" }
	})->Where({ "ID", ValueType("1") })->GetStatement().c_str());

	printf("%s\n", DB::Table("test")->Delete()->Where({ "ID", ValueType("1", false) })->GetStatement().c_str());

	printf("==== SELECT TESTS ====\n");

	DB::Table("test")->Select()->ExecStatement();
	DB::Table("test")->Select({"PATH", "TYPE"})->ExecStatement();
	printf("%s\n", DB::Table("test")->Select({ "PATH", "TYPE" })->GetStatement().c_str());

	printf("==== INSERT TESTS ====\n");
	for (size_t i = 0; i < 10; i++)
	{
		DB::Table("test")->Insert(std::vector<KeyValuePair>{
			{ "PATH", 0 },
			{ "TYPE", 1 },
			{ "STRING", "XYZ" },
			{ "BOOL", true },
			{ "DOUBLE", 0.66798 },
			{ "FLOAT", 1.345f }
		})->ExecStatement();

		DB::Table("test")->Insert(std::vector<KeyValuePair>{
			{ "PATH", 120 },
			{ "TYPE", 0 },
			{ "STRING", "ABC" },
			{ "BOOL", false },
			{ "DOUBLE", 201.567722 },
			{ "FLOAT", 12.7745f }
		})->ExecStatement();
	}

	printf("==== MODEL TESTS ====\n");
	printf(DB::Table("test")->Select()->Where({ "ID", ValueType("900", false) })->GetStatement().c_str());
	for (int i = 0; i < 20; i++)
	{
		clock_t begin = clock();
		TestTable output;
		DB::Table("test")->Select()->Where({ "ID", ValueType(i, false) })->ExecStatement(output.Callback, &output);
		if (output.m_isValid)
		{
			std::cout << fmt::format("\n\n==========\n\n ID | {}\n PATH | {}\n TYPE | {}\n STRING | {}\n BOOL | {}\n DOUBLE | {}\n FLOAT | {}\n",
				output.ID, output.PATH, output.TYPE, output.STRING.c_str(), output.BOOL ? "TRUE" : "FALSE", output.DOUBLE, output.FLOAT) << std::endl;
			clock_t end = clock();
			double duration = (double)(end - begin) / CLOCKS_PER_SEC;
			std::cout << "Execution Time: " << duration << "ms" << "\n\n==========\n" << std::endl;
		}
	}
}