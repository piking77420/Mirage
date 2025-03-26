#pragma once

#include "MirageTypeDef.hpp"

namespace mirage
{
	class MirageContext
	{
	public:
		MirageContext(const std::vector<std::wstring>& targetReflect);

		~MirageContext() = default;

		const MirageType* GetType(const std::string& className);

	private:
		MirageContextData m_Data;
	};

}


