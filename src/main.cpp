#include "public/SQL.h"
#include "public/DB.h"
#include "public/Table.h"
#include <stdio.h>
#include <map>

void main()
{
	DB* db = new DB("test.db");

	int lastInsert = DB::Table("test")->Insert(std::map<std::string, std::string>{
		{"PATH", "2"},
		{ "TYPE", "3" }
	});

	//db->Statement("CREATE TABLE test(ID INTEGER PRIMARY KEY AUTOINCREMENT, PATH INT NOT NULL, TYPE INT NOT NULL);");
	/*
	int lastInsert = DB::Insert("test",	std::map<std::string, std::string>{
		{"PATH", "2"},
		{"TYPE", "3"}
	});
	*/

	printf("New Row in Table: %i\n", lastInsert);
}