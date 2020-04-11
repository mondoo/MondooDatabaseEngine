#pragma once

#include <string>

class Model
{
public:
	Model(const std::string& table)
		: m_table(table)
	{}

protected:
	std::string m_table;
};
