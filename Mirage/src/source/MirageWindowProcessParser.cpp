#include "MirageWindowProcessParser.hpp"

#include "MirageTypeDef.hpp"

#include <cassert>
#include <iostream>

#ifdef _WIN32

#include <Windows.h>
// TO DO FIX IT IN CMAKE
#include <../../../../DIA SDK/include/dia2.h>
#include <psapi.h>
#include <atlbase.h>
#include <atlcom.h>

CComPtr<IDiaDataSource> dataSource;

static uint64_t symhandle = 42; // no need to be handle

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

void IterateOverTag(int symtagEnum , DWORD compareFlags)
{

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

                while (SUCCEEDED(resultEnumMember = enumEnumSymbolMember->Next(1, &symbolChild, &cout)) && cout > 0)
                {
                    if (!symbolChild)
                        continue;

                    wchar_t* childname = nullptr;
                    VARIANT var = {};

                    if (SUCCEEDED(symbolChild->get_name(&childname)) && SUCCEEDED(symbolChild->get_value(&var)))
                    {
                        mirage::EnumMember enumMember;

                        switch (var.vt)
                        {
                        case VT_I4:  // Integer value
                            enumMember.enumTypeValue = mirage::EnumTypeValue::INT;
                            enumMember.value.intv = var.intVal;
                            ws = std::wstring(childname);
                            menum.enumMember.push_back(enumMember);
                            break;
                        case VT_UI4:
                            enumMember.enumTypeValue = mirage::EnumTypeValue::UIINT;
                            enumMember.value.uintv = var.uintVal;
                            ws = std::wstring(childname);
                            menum.enumMember.push_back(enumMember);
                            break;
                        case VT_R4:  // Float value
                            enumMember.enumTypeValue = mirage::EnumTypeValue::FLOAT;
                            enumMember.value.floatv = var.fltVal;
                            ws = std::wstring(childname);
                            menum.enumMember.push_back(enumMember);
                            break;
                        case VT_R8:  // Double value
                            enumMember.enumTypeValue = mirage::EnumTypeValue::DOUBLE;
                            enumMember.value.doublev = var.dblVal;
                            ws = std::wstring(childname);
                            menum.enumMember.push_back(enumMember);
                            break;
                        default:
                            break;
                        }

                       
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

void ParsePdb(mirage::MirageContextData* mirageContextData, const std::wstring& pdbPath)
{
    CComPtr<IDiaSession> session;
    CComPtr<IDiaSymbol> globalScope;
    OpenPdb(pdbPath, session, globalScope);
    HandleEnum(mirageContextData, session, globalScope);
    

   

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