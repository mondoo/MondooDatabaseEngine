#include "public/SQL.h"
#include "public/DB.h"

#include <map>

void main()
{
	DB* db = new DB("test.db");

	db->Statement("CREATE TABLE test(ID INTEGER PRIMARY KEY AUTOINCREMENT, PATH INT NOT NULL, TYPE INT NOT NULL);");
	db->Insert("test",	std::map<std::string, std::string>{
		{"PATH", "2"},
		{"TYPE", "3"}
	});
}