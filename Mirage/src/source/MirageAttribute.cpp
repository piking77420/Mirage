#include <cassert>

#include "MirageAttribute.hpp"

size_t GetSizeOfVariant(VARIANT v)
{
	switch (v.vt)
	{
	case VT_I2:
		return 2;
	case VT_I4:
		return 4;
	case VT_R4:
		return 4;
	case VT_R8:
		return 8;
	case VT_CY:
		break;
	case VT_BSTR:
		break;
	case VT_BOOL:
		return sizeof(bool);
	case VT_VARIANT:
		break;
	case VT_UNKNOWN:
		break;
	case VT_DECIMAL:
		break;
	case VT_I1:
		break;
	case VT_UI1:
		return 1;
	case VT_UI2:
		return 2;
	case VT_UI4:
		return 4;
	case VT_I8:
	case VT_UI8:
		return 8;
		break;
	case VT_INT:
		return sizeof(int);
	case VT_UINT:
		return sizeof(unsigned int);
	case VT_VOID:
	case VT_HRESULT:
	case VT_PTR:
	case VT_SAFEARRAY:
	case VT_CARRAY:
	case VT_USERDEFINED:
	case VT_LPSTR:
	case VT_LPWSTR:
	case VT_RECORD:
	case VT_INT_PTR:
	case VT_UINT_PTR:
	case VT_FILETIME:
	case VT_BLOB:
	case VT_STREAM:
	case VT_STORAGE:
	case VT_STREAMED_OBJECT:
	case VT_STORED_OBJECT:
	case VT_BLOB_OBJECT:
	case VT_CF:
	case VT_CLSID:
	case VT_VERSIONED_STREAM:
	case VT_BSTR_BLOB:
	case VT_VECTOR:
	case VT_ARRAY:
	case VT_BYREF:
	case VT_DATE:
	case VT_DISPATCH:
	case VT_ERROR:
	case VT_EMPTY:
	case VT_NULL:
	default:
		assert(false);
		return -1;
	}

}


bool mirage::ParseAttibute(mirage::MirageContextData* mirageContextData, DWORD symtag, const std::string_view& attributeTypeStart ,CComPtr<IDiaSymbol>& parentPSymbol, mirage::MirageType* parsedType)
{
	size_t endOfAttrivuteStart = attributeTypeStart.find_first_of('_');

	uint32_t attributeInt = mirageContextData->mirageTypeAttributeToString(attributeTypeStart.data(), endOfAttrivuteStart);

	if (attributeInt == -1)
		return false;

	switch (symtag)
	{
	case SymTagBaseType:
		VARIANT v{};
		if (parentPSymbol->get_value(&v) != S_OK)
		{
			return false;
		}
		size_t s = GetSizeOfVariant(v);

		break;
	case SymTagUDT:
 
		break;
	default:
		assert(false);
		return false;
	}


	

	DebugBreak();

	return true;
}
