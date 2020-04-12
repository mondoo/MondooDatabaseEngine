#pragma once

#include <string>
#include <type_traits>
#include <initializer_list>
#include <vector>

#include "Reflection.h"

struct ValueType
{
	template<typename T, std::enable_if_t<std::is_convertible<T, std::string>::value> = 0>
	ValueType(const T& value)
		: m_value(std::string(value))
	{}

	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
	ValueType(const T& value)
		: m_value(std::to_string(value))
		, m_wrap(false)
	{}


	ValueType(const std::string& value)
		: m_value(value)
	{}

	ValueType(const std::string& value, bool wrap)
		: m_value(value)
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

struct TestTable
{
	TestTable() = default;

	MODELSTRUCT(TestTable);

	int ID;
	int PATH;
	int TYPE;

	bool m_isValid = false;

	void InitStruct()
	{
		MODELSTRUCTMEMBER(TestTable, int32_t, ID);
		MODELSTRUCTMEMBER(TestTable, int32_t, PATH);
		MODELSTRUCTMEMBER(TestTable, int32_t, TYPE);
	}

	MODELCALLBACK(TestTable)
		MODELCALLBACKBODY(int32_t, ID);
		MODELCALLBACKBODY(int32_t, PATH);
		MODELCALLBACKBODY(int32_t, TYPE);
	MODELCALLBACKEND

private:
	static bool ColumnCheck(char* column, char* key)
	{
		return strcmp(column, key) == 0;
	}
};
