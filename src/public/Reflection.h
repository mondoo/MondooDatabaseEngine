#pragma once

#include <map>
#include <unordered_map>
#include <string>

#include "Types.h"

#define MODELSTRUCTMEMBER(modelStruct, type, name)																												\
if (Reflection::Struct* structMap = Reflection::StructHelpers::_getStruct(#modelStruct)) {																		\
	structMap->_insertStructMapItem(#name, new Reflection::MemberTypePair(Reflection::MemberType::Type##type, (size_t)offsetof(struct modelStruct, name) ));	\
}

#define MODELCALLBACK(modelStruct)																						\
static int Callback(void* out, int count, char** data, char** column){													\
	if (count > 0){																										\
		modelStruct* dataOut = (modelStruct*)out;																		\
		for (size_t i = 0; i < count; i++) {																			\


#define MODELCALLBACKBODY(type, columnName)																				\
			if (strcmp(#columnName, column[i]) != 0) {																	\
				if (type* member = Reflection::Struct::GetStructMember<type>(dataOut, column[i])) {						\
					*member = (type)atoi(data[i]); continue;																	\
				}																										\
			}																											\

#define MODELCALLBACKEND																								\
		}																												\
		dataOut->m_isValid = true;																						\
		return 0;																										\
	}																													\
	return 1;																											\
}																														\


#define MODELSTRUCT(modelStruct)																						\
std::string m_structType = #modelStruct;																				\
void Initialise() { Reflection::StructHelpers::_insertStruct(#modelStruct); }


namespace Reflection
{
	enum class MemberType : uint8_t
	{
		Typeint8_t,
		Typeint16_t,
		Typeint32_t,
		Typeint64_t,
		Typeuint8_t,
		Typeuint16_t,
		Typeuint32_t,
		Typeuint64_t,
		Typebool,
		TypeString
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
		void _insertStructMapItem(std::string key, MemberTypePair* value)
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

		template<typename valueType, typename structType>
		static valueType* _getStructMember(structType* modelStruct, std::string member, std::map<std::string, MemberTypePair*>& map)
		{
			if (map.find(member) == map.end())
			{
				return nullptr;
			}
			else
			{
				return MemberTypeData<valueType>::GetMember(modelStruct, map[member]->m_offset);
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
}
