#include "MirageContext.hpp"

#include <iostream>

#ifdef _WIN32
#include "MirageWindowProcessParser.hpp"
#endif

mirage::MirageContext::MirageContext(const std::vector<std::wstring>& targetReflect)
{
	std::lock_guard<std::mutex>_(m_Data.lock);

	Init();
	ParseDebugFile(&m_Data, targetReflect);
	return;

	for (auto& it : m_Data.mirageEnum)
	{
		std::cout << it.name << '\n';
		for (auto& itm : it.enumMember)
		{
			
			std::cout << itm.name;
			switch (itm.enumTypeValue)
			{
			case EnumTypeValue::INT:
				std::cout << " : " <<  itm.value.intv;
				break;
			case EnumTypeValue::UIINT:
				std::cout << " : " <<  itm.value.uintv;
				break;
			case EnumTypeValue::FLOAT:
				std::cout << " : " <<  itm.value.floatv;
				break;
			case EnumTypeValue::DOUBLE:
				std::cout << " : " <<  itm.value.doublev;
				break;
			case EnumTypeValue::NONE:
			default:
				break;
			}
			std::cout << '\n';
		}
	}


}


const mirage::MirageType* mirage::MirageContext::GetType(const std::string& className)
{
	auto it = m_Data.mirageUserType.find(className);

	return it != m_Data.mirageUserType.end() ? &it->second : nullptr;
}
