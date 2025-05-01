#pragma once

#include "MirageTypeDef.hpp"

namespace mirage
{
	std::string CleanUpTypeName(const std::string& typeName);

	MirageTypeId HashStringToId(const char* _wc);

}