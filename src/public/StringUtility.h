#pragma once

#include <vector>
#include <string>
#include <sstream>

#include <fmt/format.h>

namespace StringUtility
{
	static std::vector<ValueType> Split(const std::string& string, char delimiter)
	{
		std::vector<ValueType> segments;
		std::string segment;
		std::istringstream segmentStream(string);
		while (std::getline(segmentStream, segment, delimiter))
		{
			segments.push_back(segment);
		}
		return segments;
	}

	static const std::string Join(std::vector<ValueType>& vector, const std::string& glue)
	{
		return[&vector, &glue]() -> const std::string {
			std::string out;
			bool first = true;
			for (const ValueType& string : vector)
			{
				if (first)
				{
					out += fmt::format(string.m_wrap ? "'{}'" : "{}", string.m_value);
					first = false;
				}
				else
				{
					out += glue + fmt::format(string.m_wrap ? "'{}'" : "{}", string.m_value);
				}
			}
			return out;
		}();
	}

	static const std::string Join(std::vector<std::string>& vector, const std::string& glue)
	{
		return[&vector, &glue]() -> const std::string {
			std::string out;
			bool first = true;
			for (const std::string& string : vector)
			{
				if (first)
				{
					out += string.compare("*") == 0 ? string : "`" + string + "`";
					first = false;
				}
				else
				{
					out += glue + (string.compare("*") == 0 ? string : "`" + string + "`");
				}
			}
			return out;
		}();
	}

	static void GenerateKeyValue(std::map<std::string, std::string>& insertMap, std::string& outKey, std::string& outValue)
	{
		bool first = true;
		for (std::map<std::string, std::string>::iterator it = insertMap.begin(); it != insertMap.end(); ++it)
		{
			if (first)
			{
				outKey += it->first;
				outValue += it->second;
				first = false;
			}
			else
			{
				outKey += ", " + it->first;
				outValue += ", " + it->second;
			}
		}
	}
}