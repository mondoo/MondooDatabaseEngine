# MondooDBEngine

*README still **WIP***

A simple C++ sqlite database system to simplify transactions. Currently supports basic models with the intention of implementing an ORM system to aid with developing.


# USAGE

Currently supports **SELECT**, **UPDATE**, **INSERT**, **DELETE**

The examples use a table with the schema:

    CREATE TABLE "test" (
	"ID"	INTEGER,
	"PATH"	INT NOT NULL,
	"TYPE"	INT NOT NULL,
	"STRING"	TEXT,
	"BOOL"	INTEGER,
	"DOUBLE"	REAL,
	"FLOAT"	REAL,
	PRIMARY KEY("ID") );

## SELECT

### Select All Columns

`DB::Table("test")->Select()`

### Select Specific Columns

`DB::Table("test")->Select({ "PATH", "TYPE" })`

### Select and Fill a Model

    TestTable output;
	DB::Table("test")->Select()->Where({ "ID", ValueType(1, false) })->ExecStatement(output.Callback, &output);

## UPDATE

    DB::Table("test")->Update(std::vector<KeyValuePair>{
		{"PATH", 12},
		{ "TYPE", 14 }
	})->Where({ "ID", ValueType(1, false) })->ExecStatement();

## INSERT

    DB::Table("test")->Insert(std::vector<KeyValuePair>{
		{ "PATH", 0 },
		{ "TYPE", 1 },
		{ "STRING", "XYZ" },
		{ "BOOL", true },
		{ "DOUBLE", 0.66798 },
		{ "FLOAT", 1.345f }
	})->ExecStatement();

## DELETE

    DB::Table("test")->Delete()->Where({ "ID", ValueType(1, false) })->ExecStatement();

## MODEL

We create a model to match the table's schema which we can pass into the `ExecStatement` of a `Select` statement:

    struct TestTable
	{
		MODELSTRUCT(TestTable);

		int ID;
		int PATH;
		int TYPE;
		std::string STRING;
		bool BOOL;
		double DOUBLE;
		float FLOAT;

		bool m_isValid = false;

		void InitStruct()
		{
			MODELSTRUCTMEMBER(TestTable, int32_t, ID);
			MODELSTRUCTMEMBER(TestTable, int32_t, PATH);
			MODELSTRUCTMEMBER(TestTable, int32_t, TYPE);
			MODELSTRUCTMEMBER(TestTable, string, STRING);
			MODELSTRUCTMEMBER(TestTable, bool, BOOL);
			MODELSTRUCTMEMBER(TestTable, double, DOUBLE);
			MODELSTRUCTMEMBER(TestTable, float, FLOAT);
		}

		MODELCALLBACK(TestTable)
			MODELCALLBACKBODY(int32_t, ID);
			MODELCALLBACKBODY(int32_t, PATH);
			MODELCALLBACKBODY(int32_t, TYPE);
			MODELCALLBACKBODY(string, STRING);
			MODELCALLBACKBODY(bool, BOOL);
			MODELCALLBACKBODY(double, DOUBLE);
			MODELCALLBACKBODY(float, FLOAT);
		MODELCALLBACKEND
	};

 