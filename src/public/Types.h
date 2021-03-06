#pragma once

#include <string>
#include <type_traits>
#include <initializer_list>
#include <vector>
#include <stdio.h>

#include "Reflection.h"

struct ValueType
{
	ValueType(const std::string& value)
		: m_value(value)
	{}

	ValueType(const std::string& value, bool wrap)
		: m_value(value)
		, m_wrap(wrap)
	{}

	ValueType(int value, bool wrap = false)
		: m_value(std::to_string(value))
		, m_wrap(wrap)
	{}

	ValueType(double value, bool wrap = false)
		: m_value(std::to_string(value))
		, m_wrap(wrap)
	{}

	ValueType(float value, bool wrap = false)
		: m_value(std::to_string(value))
		, m_wrap(wrap)
	{}

	ValueType() = default;

	std::string m_value;
	bool m_wrap = true;
};

struct KeyValuePair
{
	KeyValuePair() = default;

	template<typename T = ValueType>
	KeyValuePair(const std::string& key, const T& value)
		: m_key(key)
		, m_value(ValueType(value))
	{}

	template<typename T = ValueType>
	KeyValuePair(std::pair<const std::string, const T&> pair)
		: m_key(pair.first)
		, m_value(ValueType(pair.second))
	{}

	std::string m_key;
	ValueType m_value;
};

namespace Comparison
{
	static std::string Equal = "=";
	static std::string NotEqual = "!=";
	static std::string GreaterThan = ">";
	static std::string GreaterThanOrEqual = ">=";
	static std::string LessThan = "<";
	static std::string LessThanOrEqual = "<=";
	static std::string In = "IN";
	static std::string Not = "NOT";
	static std::string Between = "BETWEEN";
	static std::string IsNull = "IS NULL";
	static std::string NotNull = "IS NOT NULL";
	static std::string Like = "LIKE";
	static std::string Exists = "EXISTS";
}

struct WhereValue
{
	WhereValue() = default;

	WhereValue(const std::string& key, const std::string& comparison, const ValueType& value)
		: m_key(key)
		, m_comparison(comparison)
		, m_value(value)
	{}

	std::string m_key;
	std::string m_comparison;
	ValueType m_value;
};

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
