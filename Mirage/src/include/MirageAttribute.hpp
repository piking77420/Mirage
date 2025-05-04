#pragma once

#include <string_view>

#include "MirageTypeDef.hpp"


#ifdef _WIN32

#include <Windows.h>
// TO DO FIX IT IN CMAKE
#include <../../../../DIA SDK/include/dia2.h>
#include <psapi.h>
#include <atlbase.h>
#include <atlcom.h>
#include <MirageHash.hpp>
#include <locale>
#include <codecvt>
#undef VOID
#endif



namespace mirage
{
#ifdef _WIN32 
	bool ParseAttibute(mirage::MirageContextData* mirageContextData, DWORD symtag, const std::string_view& attributeStart, CComPtr<IDiaSymbol>& parentPSymbol, mirage::MirageType* parsedType);
#endif

}