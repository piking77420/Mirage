#include "MirageContext.hpp"

#include <iostream>

#include "MirageHash.hpp"

#ifdef _WIN32
#include "MirageWindowProcessParser.hpp"
#endif

mirage::MirageContext::MirageContext(const CreateMirageContext& _createMirageContext)
{
	std::scoped_lock<std::mutex>_(m_Data.lock);

	m_Data.mirageTypeAttributeToString = _createMirageContext.mirageTypeAttributeToString;

	Init();
	ParseDebugFile(&m_Data, _createMirageContext.targetReflect);
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