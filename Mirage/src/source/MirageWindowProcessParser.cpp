#include "MirageWindowProcessParser.hpp"

#include "MirageTypeDef.hpp"

#include <cassert>
#include <iostream>
#include <regex>

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
    HRESULT result = globalScope->findChildren(SymTagEnum, nullptr, nsfCaseInsensitive, &enumEnumSymbol);

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
                        enumMember.enumTypeValue = mirage::EnumTypeValue::NONE;

                        switch (var.vt)
                        {
                        case VT_BOOL: // bool v
                            enumMember.enumTypeValue = mirage::EnumTypeValue::INT;
                            enumMember.value.boolv = var.boolVal;
                            break;
                        case VT_I2:
                        case VT_I4:  // Integer value
                            enumMember.enumTypeValue = mirage::EnumTypeValue::INT;
                            enumMember.value.intv = var.intVal;
                            break;
                        case VT_UI2:
                        case VT_UI4:
                            enumMember.enumTypeValue = mirage::EnumTypeValue::UIINT;
                            enumMember.value.uintv = var.uintVal;
                            break;
                        case VT_R4:  // Float value
                            enumMember.enumTypeValue = mirage::EnumTypeValue::FLOAT;
                            enumMember.value.floatv = var.fltVal;
                            break;
                        case VT_R8:  // Double value
                            enumMember.enumTypeValue = mirage::EnumTypeValue::DOUBLE;
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



bool ParseUdtField(mirage::MirageContextData* mirageContextData, CComPtr<IDiaSymbol>& parentPSymbol, mirage::MirageType* MirageTypeParent)
{
    CComPtr<IDiaEnumSymbols> enumSymbolsFields;
    if (FAILED(parentPSymbol->findChildren(SymTagData, NULL, nsNone, &enumSymbolsFields)))
    {
        return false;
    }

    CComPtr<IDiaSymbol> pSymbol;
    ULONG celt = 0;
    while (SUCCEEDED(enumSymbolsFields->Next(1, &pSymbol, &celt)) && celt > 0)
    {
        
        CComPtr<IDiaSymbol> typeSymbol;
        if (pSymbol->get_type(&typeSymbol) != S_OK || !pSymbol) 
        {
            pSymbol.Release();
            continue;
        }

        DWORD typeTag = 0;
        if (typeSymbol->get_symTag(&typeTag) != S_OK)
        {
            releaseIterationResource:
            typeSymbol.Release();
            pSymbol.Release();
            continue;
        }
        
       

        mirage::MirageTypeId typeId;
        mirage::MirageType mirageType;
        bool succes = false;
        wchar_t* tagName = nullptr;

        switch (typeTag)
        {
        case SymTagBaseType:
            // field Data
        {
            if (pSymbol->get_name(&tagName) != S_OK)
            {
            }
            DWORD offset = 0;
            if (pSymbol->get_offsetInUdt(&offset) != S_OK)
            {
            }
        }
            
        break;
        case SymTagUDT:
            {
            // field Data
            if (pSymbol->get_name(&tagName) != S_OK)
            {
            }
            DWORD offset = 0;
            if (pSymbol->get_offsetInUdt(&offset) != S_OK)
            {
            }
                succes = ParseUdt(mirageContextData, pSymbol, &typeId, &mirageType);
            }   
        break;
        case SymTagBaseClass:
            {// field Data
                if (pSymbol->get_name(&tagName) != S_OK)
                {
                }
            }
            break;
        default:
            break;
        }

    

        pSymbol.Release();
    }


    enumSymbolsFields.Release();

}

bool ParseUdt(mirage::MirageContextData* mirageContextData, CComPtr<IDiaSymbol>& symbol, mirage::MirageTypeId* typeId, mirage::MirageType* mirageType)
{
    wchar_t* symbolName = nullptr;

    if (symbol->get_name(&symbolName) != S_OK)
        return false;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string typenName = converter.to_bytes(symbolName);
    mirage::MirageTypeId id = mirage::HashStringToId(typenName.c_str());

    if (typenName == "MyClass")
    {
        __debugbreak();
    }

    if (mirage::ContainType(mirageContextData, id))
        return false;

    *typeId = id;
    mirageType->name = std::move(typenName);
    mirageType->mirageTypeId = *typeId;

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
#endif