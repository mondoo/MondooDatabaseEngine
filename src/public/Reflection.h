#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <sstream>
#include <vector>

#include "Types.h"

#define MODELSTRUCTMEMBER(modelStruct, type, name)																												\
if (Reflection::Struct* structMap = Reflection::StructHelpers::_getStruct(#modelStruct)) {																		\
	structMap->InsertStructMapItem(#name, new Reflection::MemberTypePair(Reflection::MemberType::Type##type, (size_t)offsetof(struct modelStruct, name) ));	\
}

#define MODELCALLBACK(modelStruct)																						\
static int Callback(void* out, int count, char** data, char** column){													\
	if (count > 0){																										\
		std::vector<modelStruct*>* outVector = (std::vector<modelStruct*>*)out;											\
		modelStruct* dataOut = new modelStruct();																		\
		outVector->push_back(dataOut);																					\
		for (size_t i = 0; i < count; i++) {

#define MODELCALLBACKBODY(type, columnName)																				\
			if (strcmp(#columnName, column[i]) == 0) {																	\
				if (type* member = Reflection::Struct::GetStructMember<type>(dataOut, column[i])) {						\
					Reflection::ModelMemberHelpers::LexicalCast(*member, data[i]); continue;							\
				}																										\
			}

#define MODELCALLBACKEND																								\
		}																												\
		dataOut->m_isValid = true;																						\
		return 0;																										\
	}																													\
	return 1;																											\
}

#define MODELSTRUCT(modelStruct)																						\
typedef std::string string;																								\
modelStruct() { Initialise(); InitStruct(); }																			\
std::string m_structType = #modelStruct;																				\
void Initialise() { Reflection::StructHelpers::_insertStruct(#modelStruct); }

namespace Reflection
{
	enum class MemberType : uint8_t
	{
		Typeint32_t,
		Typeint64_t,
		Typeuint32_t,
		Typeuint64_t,
		Typebool,
		Typestring,
		Typefloat,
		Typedouble
	};

	struct MemberTypePair
	{
		MemberTypePair() = default;

		MemberTypePair(MemberType type, size_t offset)
			: m_type(type)
			, m_offset(offset)
		{}

		MemberType m_type;
		size_t m_offset;
	};

	template<typename T>
	class MemberTypeData
	{
	public:
		static T* GetMember(void* data, size_t offset)
		{
			return reinterpret_cast<T*>(((size_t)data) + offset);
		}
	};

	class Struct
	{
	public:
		Struct() = default;

		std::map<std::string, MemberTypePair*> s_structMap;

		void InsertStructMapItem(std::string key, MemberTypePair* value)
		{
			s_structMap.insert_or_assign(key, value);
		}

		template<typename valueType, typename structType>
		static valueType* GetStructMember(structType* modelStruct, const std::string& member)
		{
			if (Struct* foundStruct = StructHelpers::_getStruct(modelStruct->m_structType))
			{
				return _getStructMember<valueType>(modelStruct, member, foundStruct->s_structMap);
			}

			return nullptr;
		}

	private:
		template<typename valueType, typename structType>
		static valueType* _getStructMember(structType* modelStruct, std::string member, std::map<std::string, MemberTypePair*>& map)
		{
			if (map.find(member) == map.end())
			{
				return nullptr;
			}
			else
			{
				valueType* str = MemberTypeData<valueType>::GetMember(modelStruct, map[member]->m_offset);
				return str;
			}
		}
	};

	class StructHelpers
	{
	public:
		StructHelpers() = default;

		static void _insertStruct(std::string key)
		{
			s_typeMaps.insert_or_assign(key, new Struct());
		}

		static Struct* _getStruct(std::string key)
		{
			if (s_typeMaps.find(key) != s_typeMaps.end())
			{
				return s_typeMaps[key];
			}

			return nullptr;
		}

		static std::unordered_map<std::string, Struct*> s_typeMaps;
	};

	class ModelMemberHelpers
	{
	public:
		template<typename To, typename From>
		static void LexicalCast(To& outVal, const From* from)
		{
			if (from != NULL)
			{
				std::stringstream sstream;

				sstream << from;
				sstream >> outVal;
			}
		}
	};
}
