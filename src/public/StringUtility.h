#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace StringUtility
{
	static std::vector<std::string> Split(const std::string& string, char delimiter)
	{
		std::vector<std::string> segments;
		std::string segment;
		std::istringstream segmentStream(string);
		while (std::getline(segmentStream, segment, delimiter))
		{
			segments.push_back(segment);
		}
		return segments;
	}

	static const std::string Join(std::vector<std::string>& vector, char glue)
	{
		return[&vector, &glue]() -> const std::string {
			std::string out;
			bool first = true;
			for (const std::string& string : vector)
			{
				if (first)
				{
					out += string;
					first = false;
				}
				else
				{
					out += glue + string;
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