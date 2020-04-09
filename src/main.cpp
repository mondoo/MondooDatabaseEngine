#include "public/SQL.h"
#include "public/DB.h"

#include <map>

void main()
{
	DB* db = new DB("test.db");

	//db->Statement("CREATE TABLE test(ID INTEGER PRIMARY KEY AUTOINCREMENT, PATH INT NOT NULL, TYPE INT NOT NULL);");
	db->Insert("test",	std::map<std::string, std::string>{
		{"PATH", "2"},
		{"TYPE", "3"}
	});
	
	//db->Statement("CREATE TABLE resources(ID INT PRIMARY KEY NOT NULL, PATH TEXT NOT NULL, TYPE INT NOT NULL);");
	//db->Select("Hello", "ID", 3);


	/*
	sqlite3* db;
	char* errorMsg = 0;
	int connection;

	connection = sqlite3_open("test.db", &db);

	if (connection)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n");
	}

	sqlite3_close(db);
	*/
}