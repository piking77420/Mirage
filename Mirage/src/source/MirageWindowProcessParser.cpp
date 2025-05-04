#include "MirageWindowProcessParser.hpp"

#include "MirageTypeDef.hpp"

#include <cassert>
#include <iostream>
#include <regex>

#include "MirageAttribute.hpp"

#ifdef _WIN32



bool ParseUdt(mirage::MirageContextData* mirageContextData, CComPtr<IDiaSymbol>& symbol, mirage::MirageTypeId* typeId, mirage::MirageType* mirageType);

CComPtr<IDiaDataSource> dataSource;


void OpenPdb(const std::wstring& pdbPath, CComPtr<IDiaSession>& session, CComPtr<IDiaSymbol>& globalScope)
{
    if (dataSource->loadDataFromPdb(pdbPath.c_str()))
    {
        return;
    }

    if (dataSource->openSession(&session))
    {
        return;
    }

    if (session->get_globalScope(&globalScope))
    {
        return;
    }
}

void HandleEnum(mirage::MirageContextData* mirageContextData, CComPtr<IDiaSession> session, CComPtr<IDiaSymbol> globalScope)
{
    CComPtr<IDiaEnumSymbols> enumEnumSymbol;
    HRESULT result = globalScope->findChildren(SymTagEnum, nullptr, nsNone, &enumEnumSymbol);

    HRESULT hr;
    ULONG cout = 0;

    CComPtr<IDiaSymbol> symbol;

    // enum Struct 
    while (SUCCEEDED(hr = enumEnumSymbol->Next(1, &symbol, &cout)) && cout > 0)
    {
        if (!symbol)
            continue;

        wchar_t* name = nullptr;

        if (SUCCEEDED(symbol->get_name(&name)))
        {
            mirage::MirageEnum menum;

            std::wstring ws(name);
            menum.name = std::string(ws.begin(), ws.end());

            CComPtr<IDiaEnumSymbols> enumEnumSymbolMember;

            if (SUCCEEDED(symbol->findChildren(SymTagData, nullptr, nsfCaseInsensitive, &enumEnumSymbolMember)))
            {
                HRESULT resultEnumMember;
                CComPtr<IDiaSymbol> symbolChild;

                ULONG cout2 = 0;
                while (SUCCEEDED(resultEnumMember = enumEnumSymbolMember->Next(1, &symbolChild, &cout2)) && cout2 > 0)
                {
                    if (!symbolChild)
                        continue;

                    wchar_t* childname;
                    VARIANT var;

                    if (SUCCEEDED(symbolChild->get_name(&childname)) && SUCCEEDED(symbolChild->get_value(&var)))
                    {
                        mirage::EnumMember enumMember;
                        enumMember.enumTypeValue = mirage::TrivialType::COUNT;

                        switch (var.vt)
                        {
                        case VT_BOOL: // bool v
                            enumMember.enumTypeValue = mirage::TrivialType::INT;
                            enumMember.value.boolv = var.boolVal;
                            break;
                        case VT_I2:
                        case VT_I4:  // Integer value
                            enumMember.enumTypeValue = mirage::TrivialType::INT;
                            enumMember.value.intv = var.intVal;
                            break;
                        case VT_UI2:
                        case VT_UI4:
                            enumMember.enumTypeValue = mirage::TrivialType::UIINT;
                            enumMember.value.uintv = var.uintVal;
                            break;
                        case VT_R4:  // Float value
                            enumMember.enumTypeValue = mirage::TrivialType::FLOAT;
                            enumMember.value.floatv = var.fltVal;
                            break;
                        case VT_R8:  // Double value
                            enumMember.enumTypeValue = mirage::TrivialType::DOUBLE;
                            enumMember.value.doublev = var.dblVal;
                            break;
                        default:
                            break;
                        }

                        ws = std::wstring(childname);
                        enumMember.name = std::string(ws.begin(), ws.end());
                        menum.enumMember.push_back(enumMember);
                    }

                    symbolChild.Release();
                }
                enumEnumSymbolMember.Release();
            }

            mirageContextData->mirageEnum.push_back(menum);
        }

        symbol.Release();

    }
}


bool ExtractBaseTypeFromSymbol(CComPtr<IDiaSymbol>& fieldType, size_t typeSize, mirage::MirageField* _f)
{
    DWORD baseType;
    if (fieldType->get_baseType(&baseType) != S_OK)
    {
        return false;
    }

    _f->mirageTypeDescriptor.miratageTypeDescriptorFlagBit |= mirage::TRIVIAL;
    mirage::TrivialType& tr = _f->mirageTypeDescriptor.mirageType.trivialType;

    switch (baseType)
    {
    case btVoid:     tr = mirage::TrivialType::VOID; break;
    case btChar:     tr = mirage::TrivialType::CHAR; break;
    case btWChar:    tr = mirage::TrivialType::WCHAR; break;
    case btInt:      tr = mirage::TrivialType::INT; break;
    case btUInt:     tr = mirage::TrivialType::UIINT; break;
    case btFloat:    tr = typeSize == 8 ? mirage::TrivialType::DOUBLE : mirage::TrivialType::FLOAT; break;
    case btBCD:; break;
    case btBool:     tr = mirage::TrivialType::BOOL; break;
        //case btLong:     fieldtypeName = "long"; break;
        //case btULong:    fieldtypeName = "unsigned long"; break;
        //case btComplex: fieldtypeName = "complex"; break;

        // wtf are those ?? exept variant
    case btCurrency:
    case btDate:
    case btVariant:
    case btBit:
    case btBSTR:
    case btHresult:
    case btNoType:
    default: break;
    }

    return true;
}




bool ExtractDataFromSymbol(mirage::MirageContextData* _data, CComPtr<IDiaSymbol>& fieldTypePtr, std::string* _outFieldtypeName, mirage::MirageTypeId* _outId)
{
    wchar_t* fieldTypeName = nullptr;
    if (fieldTypePtr->get_name(&fieldTypeName) != S_OK)
    {
        return false;
    }

    *_outFieldtypeName = mirage::GetStringFromWchart(fieldTypeName);
    *_outId = mirage::HashStringToId(_outFieldtypeName->c_str());
}

bool ExtractUDTOrBaseClassFromSymbol(mirage::MirageContextData* _data, CComPtr<IDiaSymbol>& fieldType, mirage::MirageField* _f)
{
    mirage::MirageTypeId id;
    std::string fieldtypeName;

    if (!ExtractDataFromSymbol(_data, fieldType, &fieldtypeName, &id))
        return false;

    _f->mirageTypeDescriptor.mirageType.mirageTypeId = mirage::HashStringToId(fieldtypeName.c_str());
    _f->name = std::move(fieldtypeName);
}


bool ParseUdtSymbol(mirage::MirageContextData* mirageContextData, CComPtr<IDiaSymbol>& fieldType, DWORD symtag, 
    std::string* _fieldName, CComPtr<IDiaSymbol>& pSymbolfield, mirage::MirageType* parsedType)
{
    LONG fieldOffSet = 0;
    if (pSymbolfield->get_offset(&fieldOffSet) != S_OK)
        return false;

    ULONGLONG typeSize = 0;
    if (fieldType->get_length(&typeSize) != S_OK)
        return false;


    mirage::MirageField f = {};

    switch (symtag)
    {
    case SymTagBaseType:
    {
        if (!ExtractBaseTypeFromSymbol(fieldType, typeSize, &f))
            return false;
    }
    break;

    case SymTagUDT:
    {
        if (!ExtractUDTOrBaseClassFromSymbol(mirageContextData, fieldType, &f))
            return false;

        // seems to work when herited but not in member (so it's ok )
        BOOL inhertied;
        if (!(f.mirageTypeDescriptor.miratageTypeDescriptorFlagBit & mirage::TRIVIAL) && pSymbolfield->get_virtualBaseClass(&inhertied) == S_OK)
        {
            parsedType->inhertedType.push_back(f.mirageTypeDescriptor.mirageType.mirageTypeId);
        }
    }
    break;
    case SymTagPointerType:
    {
        if (!ExtractUDTOrBaseClassFromSymbol(mirageContextData, fieldType, &f))
            return false;

        f.mirageTypeDescriptor.miratageTypeDescriptorFlagBit |= mirage::PTR;
    }
    break;
    case SymTagArrayType:
    {
        f.mirageTypeDescriptor.miratageTypeDescriptorFlagBit |= mirage::ARRAY;
    }
    break;
    default:
        break;
    }
    f.offset = static_cast<size_t>(fieldOffSet);
    f.name = std::move(*_fieldName);

    parsedType->fields.push_back(f);
    return true;
}


bool ParseUdtField(mirage::MirageContextData* mirageContextData, CComPtr<IDiaSymbol>& parentPSymbol, mirage::MirageType* parsedType)
{
    CComPtr<IDiaEnumSymbols> enumSymbolsFields;
    if (FAILED(parentPSymbol->findChildren(SymTagNull, NULL, nsfCaseInsensitive, &enumSymbolsFields)))
    {
        return false;
    }

    CComPtr<IDiaSymbol> pSymbolfield;
    ULONG celt = 0;
    while (SUCCEEDED(enumSymbolsFields->Next(1, &pSymbolfield, &celt)) && celt > 0)
    {
        wchar_t* fieldNamew = nullptr;
        if (pSymbolfield->get_name(&fieldNamew) != S_OK)
        {
            RealeaseSymbol:
            pSymbolfield.Release();
            continue;
        }
        std::string fieldName = mirage::GetStringFromWchart(fieldNamew);
        std::string_view sv(fieldName);

        CComPtr<IDiaSymbol> fieldType;
        if (pSymbolfield->get_type(&fieldType) != S_OK)
        {
            ReleasefieldType:
            fieldType.Release();
            goto RealeaseSymbol;
        }

        DWORD typeTag = 0;
        if (fieldType->get_symTag(&typeTag) != S_OK)
            goto ReleasefieldType;


        if (sv.substr(0, 2) == START_ATTRIBUTE)
        {
            std::string_view sv = std::string_view(fieldName);
            ParseAttibute(mirageContextData, typeTag, sv.data() + strlen(START_ATTRIBUTE), pSymbolfield, parsedType);
            goto ReleasefieldType;
            
        }
        else
        {
            //if (ParseUdtSymbol(mirageContextData, fieldType, typeTag, &fieldName, pSymbolfield, parsedType))
              //  goto ReleasefieldType;
        }

      
        pSymbolfield.Release();
    }

    
    enumSymbolsFields.Release();

}

bool ParseUdt(mirage::MirageContextData* mirageContextData, CComPtr<IDiaSymbol>& symbol, mirage::MirageTypeId* typeId, mirage::MirageType* mirageType)
{
    wchar_t* symbolName = nullptr;

    if (symbol->get_name(&symbolName) != S_OK)
        return false;

    ULONGLONG lenght = 0;
    if (symbol->get_length(&lenght) != S_OK)
        return false;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string typenName = converter.to_bytes(symbolName);
    mirage::MirageTypeId id = mirage::HashStringToId(typenName.c_str());

#if 1
    if (typenName == "MyClass::Vec2")
    {
        __debugbreak();
    }
#endif
    if (mirage::ContainType(mirageContextData, id))
        return false;

    *typeId = id;
    mirageType->mirageTypeId = *typeId;
    mirageType->size = static_cast<size_t>(lenght);
    mirageType->name = std::move(typenName);

    ParseUdtField(mirageContextData, symbol, mirageType);
    return true;
}


void HandleUserType(mirage::MirageContextData* mirageContextData, CComPtr<IDiaSession> session, CComPtr<IDiaSymbol> globalScope)
{
    CComPtr<IDiaEnumSymbols> pEnumSymbols;
    if (FAILED(globalScope->findChildren(SymTagUDT, nullptr, nsNone, &pEnumSymbols)))
        return;

    HRESULT hr;
    ULONG cout = 0;

    CComPtr<IDiaSymbol> pSymbol;
    ULONG celt = 0;

    while (SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1)) {

        mirage::MirageTypeId typeId;
        mirage::MirageType mirageType;
        
        if (ParseUdt(mirageContextData, pSymbol, &typeId, &mirageType))
        {
            mirageContextData->mirageUserType.emplace(typeId, mirageType);
        }
        pSymbol.Release();
    }

    pEnumSymbols.Release();
}

void ParsePdb(mirage::MirageContextData* mirageContextData, const std::wstring& pdbPath)
{
    CComPtr<IDiaSession> session;
    CComPtr<IDiaSymbol> globalScope;
    OpenPdb(pdbPath, session, globalScope);

    //std::thread t = std::thread([&]() {HandleEnum(mirageContextData, session, globalScope); });
    HandleUserType(mirageContextData, session, globalScope);

}


void mirage::Init()
{

    if (FAILED(CoInitialize(nullptr))) {
        std::cerr << "Failed to initialize COM.\n";
        return;
    }

    switch (CoCreateInstance(__uuidof(DiaSource), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IDiaDataSource), reinterpret_cast<void**>(&dataSource)))
    {

    case REGDB_E_CLASSNOTREG:
        CoUninitialize();
        break;

    case CLASS_E_NOAGGREGATION:
        CoUninitialize();
        break;

    case E_NOINTERFACE:
        CoUninitialize();
        break;

    case E_POINTER:
        CoUninitialize();
        break;

    case S_OK:
    default:
        break;
    }
}

void mirage::ParseDebugFile(MirageContextData* mirageContextData, const std::vector<std::wstring>& targetReflect)
{
    for (auto& it : targetReflect)
    {
        ParsePdb(mirageContextData, it);
    }

}

void mirage::Destroy()
{
    CoUninitialize();
}
#endif // _WIN32
