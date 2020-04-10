#include "public/SQL.h"
#include "public/DB.h"
#include "public/Table.h"
#include <stdio.h>
#include <map>

#include "public/Types.h"

void main()
{
	DB* db = new DB("test.db");
	/*

	int lastInsert = DB::Table("test")->Insert(std::map<std::string, std::string>{
		{"PATH", "2"},
		{ "TYPE", "3" }
	});
	*/

	printf("%s\n", DB::Table("test")->Select({ "hello", "world" })->Get().c_str());
	printf("%s\n", DB::Table("test")->Select()->Get().c_str());
	printf("%s\n", DB::Table("test")->Insert("Hello", 1, "World", 5031)->Get().c_str());
	printf("%s\n", DB::Table("test")->Insert(std::vector<ValueType>{ValueType("Hello"), ValueType(1)})->Get().c_str());
	printf("%s\n", DB::Table("test")->Insert({ {"Hello", ValueType("World")}, {"Test 1", ValueType("Test 2")} })->Get().c_str());
	printf("%s\n", DB::Table("test")->Update({ {"Hello", ValueType("World")}, { "Test 1", ValueType("Test 2") } })->Where({ "ID", ValueType("1") })->Get().c_str());
	printf("%s\n", DB::Table("test")->Delete()->Where({ "ID", ValueType("1", false) })->Get().c_str());

	//db->Statement("CREATE TABLE test(ID INTEGER PRIMARY KEY AUTOINCREMENT, PATH INT NOT NULL, TYPE INT NOT NULL);");
	/*
	int lastInsert = DB::Insert("test",	std::map<std::string, std::string>{
		{"PATH", "2"},
		{"TYPE", "3"}
	});
	*/

	//printf("New Row in Table: %i\n", lastInsert);
}