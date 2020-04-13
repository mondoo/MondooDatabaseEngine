#include "public/SQL.h"
#include "public/DB.h"
#include "public/Table.h"
#include <stdio.h>
#include <map>
#include <unordered_map>

#include <fmt/format.h>
#include <string>


#include <time.h>
#include <ctime>
#include <cstdlib>

#include <iostream>

#include "public/Reflection.h"
#include "public/Types.h"

std::unordered_map<std::string, Reflection::Struct*> Reflection::StructHelpers::s_typeMaps;

void main()
{
	DB* db = new DB("test.db");

	db->Statement("DELETE FROM test;");

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
	std::srand((unsigned int)std::time(nullptr));
	for (size_t i = 0; i < 10; i++)
	{

		DB::Table("test")->Insert(std::vector<KeyValuePair>{
			{ "PATH", 0 },
			{ "TYPE", (int)std::rand() },
			{ "STRING", "XYZ" },
			{ "BOOL", true },
			{ "DOUBLE", 0.66798 },
			{ "FLOAT", 1.345f }
		})->ExecStatement();

		DB::Table("test")->Insert(std::vector<KeyValuePair>{
			{ "PATH", 120 },
			{ "TYPE", (int)std::rand() },
			{ "STRING", "ABC" },
			{ "BOOL", false },
			{ "DOUBLE", 201.567722 },
			{ "FLOAT", 12.7745f }
		})->ExecStatement();
	}


	clock_t begin = clock();
	auto printExecTime = [&begin]() {
		clock_t end = clock();
		double duration = (double)(end - begin) / CLOCKS_PER_SEC;
		std::cout << "Execution Time: " << duration << "ms" << "\n\n==========\n" << std::endl;
	};

	printf("==== SELECT TESTS ====\n");
	printf("== SELECT ALL ==\n");
	printf("== ORDER BY ==\n");
	std::vector<TestTable*> select;
	DB::Table("test")->Select()->ExecStatement(TestTable::Callback, &select, true);
	for (const TestTable* row : select)
	{
		if (row != nullptr && row->m_isValid)
		{
			std::cout << fmt::format("\n ID: {} | PATH: {} | TYPE: {} | STRING: {} | BOOL: {} | DOUBLE: {} | FLOAT: {}\n",
				row->ID, row->PATH, row->TYPE, row->STRING, row->BOOL ? "TRUE" : "FALSE", row->DOUBLE, row->FLOAT) << std::endl;
		}
	}
	printExecTime();

	
	begin = clock();
	printf("== SINGLE ROW LOOK UP ==\n");
	std::vector<TestTable*> singleRowLookup;
	DB::Table("test")->Select()->Where({ "ID", ValueType("12", false) })->ExecStatement(TestTable::Callback, &singleRowLookup, true);
	if (singleRowLookup.size() > 0)
	{
		std::cout << fmt::format("\n ID: {} | PATH: {} | TYPE: {} | STRING: {} | BOOL: {} | DOUBLE: {} | FLOAT: {}\n\n\n",
			singleRowLookup[0]->ID, singleRowLookup[0]->PATH, singleRowLookup[0]->TYPE, singleRowLookup[0]->STRING, singleRowLookup[0]->BOOL ? "TRUE" : "FALSE", singleRowLookup[0]->DOUBLE, singleRowLookup[0]->FLOAT) << std::endl;
	}
	printExecTime();



	begin = clock();
	printf("== ORDER BY ==\n");
	std::vector<TestTable*> rows;
	DB::Table("test")->Select()->OrderBy({ {"TYPE", "ASC"} })->ExecStatement(TestTable::Callback, &rows, true);

	for (const TestTable* row : rows)
	{
		if (row != nullptr && row->m_isValid)
		{
			std::cout << fmt::format("\n ID: {} | PATH: {} | TYPE: {} | STRING: {} | BOOL: {} | DOUBLE: {} | FLOAT: {}\n",
				row->ID, row->PATH, row->TYPE, row->STRING, row->BOOL ? "TRUE" : "FALSE", row->DOUBLE, row->FLOAT) << std::endl;
		}
	}

	printExecTime();
}