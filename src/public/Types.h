#pragma once

#include <string>
#include <type_traits>

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