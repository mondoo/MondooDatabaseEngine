#include "public/SQL.h"
#include "public/DB.h"
#include "public/Table.h"
#include <stdio.h>
#include <map>

#include "public/Types.h"

struct TestTable
{
	TestTable() = default;

	int m_id;
	int m_path;
	int m_type;

	bool m_isValid = false;

	static int Callback(void* out, int count, char** data, char** column)
	{
		if (count > 0)
		{
			TestTable* dataOut = (TestTable*)out;
			
			for (size_t i = 0; i < count; i++)
			{
				if (ColumnCheck(column[i], "ID"))
				{
					dataOut->m_id = atoi(data[i]);
				}
				else if (ColumnCheck(column[i], "PATH"))
				{
					dataOut->m_path = atoi(data[i]);
				}
				else if (ColumnCheck(column[i], "TYPE"))
				{
					dataOut->m_type = atoi(data[i]);
				}
			}

			dataOut->m_isValid = true;

			return 0;
		}
		return 1;
	}

private:
	static bool ColumnCheck(char* column, char* key)
	{
		return strcmp(column, key) == 0;
	}
};

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

	TestTable output;
	printf(DB::Table("test")->Select()->Where({ "ID", ValueType("100", false) })->Get().c_str());
	DB::Table("test")->Select()->Where({ "ID", ValueType("100", false) })->Exec(output.Callback, &output);
	if (output.m_isValid)
	{
		printf("ID: %i, PATH: %i, TYPE: %i\n", output.m_id, output.m_path, output.m_type);
	}

	printf("\n");

	printf("==== INSERT TESTS ====\n");
	DB::Table("test")->Insert(std::vector<KeyValuePair>{
		{ "PATH", 0 },
		{ "TYPE", 1 }
	})->Exec();

	for (size_t i = 0; i < 10; i++)
	{
		DB::Table("test")->Insert(std::vector<KeyValuePair>{
			{ "PATH", 0 },
			{ "TYPE", 1 }
		})->Exec();
	}

	printf("\n");

	//db->Statement("CREATE TABLE test(ID INTEGER PRIMARY KEY AUTOINCREMENT, PATH INT NOT NULL, TYPE INT NOT NULL);");
	/*
	int lastInsert = DB::Insert("test",	std::map<std::string, std::string>{
		{"PATH", "2"},
		{"TYPE", "3"}
	});
	*/

	//printf("New Row in Table: %i\n", lastInsert);
}