#include "MirageContext.hpp"

#include <iostream>

#include "MirageHash.hpp"

#ifdef _WIN32
#include "MirageWindowProcessParser.hpp"
#endif

mirage::MirageContext::MirageContext(const std::vector<std::wstring>& targetReflect)
{
	std::scoped_lock<std::mutex>_(m_Data.lock);

	Init();
	ParseDebugFile(&m_Data, targetReflect);
}


const mirage::MirageType* mirage::MirageContext::GetType(const std::string& className) const
{
	const MirageTypeId id = HashStringToId(className.c_str());

	return GetType(id);
}

const  mirage::MirageType* mirage::MirageContext::GetType(const  mirage::MirageTypeId id) const
{
	auto it = m_Data.mirageUserType.find(id);

	return it != m_Data.mirageUserType.end() ? &it->second : nullptr;
}
