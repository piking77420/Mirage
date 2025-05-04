#pragma once

#include "MirageTypeDef.hpp"
#include "CompiletimeKey.hpp"

namespace mirage
{
	class MirageContext
	{
	public:
		MirageContext(const std::vector<std::wstring>& targetReflect);

		~MirageContext() = default;

		template <typename T>
		const MirageType* GetType() const;

		const MirageType* GetType(const std::string& className) const;

		const MirageType* GetType(const MirageTypeId id) const;

	private:
		MirageContextData m_Data;

	};

	
	template<typename T>
	inline const MirageType* MirageContext::GetType() const
	{	
		return GetType(COMPILE_TIME_TYPE_KEY(T));
	}

}


