#include "public/SQL.h"
#include "public/DB.h"
#include "public/Table.h"
#include <stdio.h>
#include <map>
#include <unordered_map>

#include "public/Reflection.h"
#include "public/Types.h"

std::unordered_map<std::string, Reflection::Struct*> Reflection::StructHelpers::s_typeMaps;

void main()
{
	DB* db = new DB("test.db");

	printf("%s\n", DB::Table("test")->Select({ "hello", "world" })->Get().c_str());
	printf("%s\n", DB::Table("test")->Select()->Get().c_str());

	printf("%s\n", DB::Table("test")->Insert("Hello", 1, "World", 5031)->Get().c_str());
	printf("%s\n", DB::Table("test")->Insert(std::vector<ValueType>{ValueType("Hello"), ValueType(1)})->Get().c_str());
	printf("%s\n", DB::Table("test")->Insert(std::vector<KeyValuePair>{
		{"Hello", "World"},
		{"Test 1", "Test 2"},
		{"Test 2", 10}
	})->Get().c_str());

	printf("%s\n", DB::Table("test")->Update(std::vector<KeyValuePair>{
		{"Hello", "World"},
		{ "Test 1", "Test 2" }
	})->Where({ "ID", ValueType("1") })->Get().c_str());

	printf("%s\n", DB::Table("test")->Delete()->Where({ "ID", ValueType("1", false) })->Get().c_str());

	printf("==== SELECT TESTS ====\n");

	DB::Table("test")->Select()->Exec();
	DB::Table("test")->Select({"PATH", "TYPE"})->Exec();
	printf("%s\n", DB::Table("test")->Select({ "PATH", "TYPE" })->Get().c_str());

	printf(DB::Table("test")->Select()->Where({ "ID", ValueType("900", false) })->Get().c_str());
	for (int i = 1000; i < 1152; i++)
	{
		TestTable output;
		DB::Table("test")->Select()->Where({ "ID", ValueType(i, false) })->Exec(output.Callback, &output);
		if (output.m_isValid)
		{
			printf("\nID: %i, PATH: %i, TYPE: %i, STRING: %s\n", output.ID, output.PATH, output.TYPE, output.STRING.c_str());
		}
	}

	printf("\n");

	printf("==== INSERT TESTS ====\n");
	DB::Table("test")->Insert(std::vector<KeyValuePair>{
		{ "PATH", 0 },
		{ "TYPE", 1 },
		{ "STRING", "Hello World" }
	})->Exec();

	for (size_t i = 0; i < 10; i++)
	{
		DB::Table("test")->Insert(std::vector<KeyValuePair>{
			{ "PATH", 0 },
			{ "TYPE", 1 },
			{ "STRING", "XYZ" }
		})->Exec();
	}
	
	printf("\n");
}