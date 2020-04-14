#pragma once

#include <string>
#include <vector>

#include "DB.h"
#include "Table.h"

template<class TModel>
class Model
{
public:
	Model() {}

	static std::vector<TModel*> All()
	{
		std::vector<TModel*> models;
		DB::Table(m_table)->Select()->ExecStatement(true, TModel::Callback, &models);
		return models;
	}

protected:
	inline static std::string m_table = "test";
	inline static std::string m_primaryKey = "ID";
	inline static bool m_isAutoIncrementing = true;
	inline static bool m_hasTimestamps = true;

	inline static std::string m_createdAt = "CREATED_AT";
	inline static std::string m_updatedAt = "UPDATED_AT";
};