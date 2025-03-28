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

inline std::string GetStringFromWchart(wchar_t* ptr)
{
    std::wstring ws(ptr);
    return std::string(ws.begin(), ws.end());
}


std::string cleanUpTypeName(const std::string& typeName) 
{
    std::regex classStructRegex(R"(class\s+|struct\s+)");

    return std::regex_replace(typeName, classStructRegex, "");
}


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

void HandleUserType(mirage::MirageContextData* mirageContextData, CComPtr<IDiaSession> session, CComPtr<IDiaSymbol> globalScope)
{
    CComPtr<IDiaEnumSymbols> enumUserTypeSymbol;
    HRESULT result = globalScope->findChildren(SymTagUDT, nullptr, nsfCaseInsensitive, &enumUserTypeSymbol);

    HRESULT hr;
    ULONG cout = 0;

    CComPtr<IDiaSymbol> symbol;
    while (SUCCEEDED(hr = enumUserTypeSymbol->Next(1, &symbol, &cout)) && cout > 0)
    {
        if (!symbol)
            continue;

        wchar_t* typeName = nullptr;
  

        if (symbol->get_name(&typeName) == S_OK)
        {
            std::string typeNameS = GetStringFromWchart(typeName);


            if (mirageContextData->mirageUserType.find(typeNameS) != mirageContextData->mirageUserType.end())
            {
                symbol.Release();
                continue;
            }


#if 0
            // SHOUDL USE THIS TO LINK APP RUNTIME TO DATA
                std::string sname = cleanUpTypeName(typeid(std::vector<std::wstring>).name());
                        std::wstring wname = name;

            std::string converted = std::string(wname.begin(), wname.end());
            if (converted == sname)
            {
                //__debugbreak();
            }
#endif
      


            CComPtr<IDiaEnumSymbols> enumfieldSymbolMember;
            if (SUCCEEDED(symbol->findChildren(SymTagData, nullptr, nsfCaseInsensitive, &enumfieldSymbolMember)))
            {
                std::vector<mirage::MirageField> fields;
                
                HRESULT resultFiel;
                CComPtr<IDiaSymbol> symbolFieldChild;

                ULONG cout2 = 0;
                while (SUCCEEDED(resultFiel = enumfieldSymbolMember->Next(1, &symbolFieldChild, &cout2)) && cout2 > 0)
                {
                    if (!symbolFieldChild)
                        continue;

                    wchar_t* fieldNamew = nullptr;
                    LONG offset = 0;

                    if (symbolFieldChild->get_name(&fieldNamew) == S_OK && symbolFieldChild->get_offset(&offset) == S_OK)
                    {
                        std::string fieldName = GetStringFromWchart(fieldNamew);
                        fields.push_back({ fieldName, (uint32_t)offset });

                    }

             
                    

                    symbolFieldChild.Release();
                }
                enumfieldSymbolMember.Release();

                if (!fields.empty())
                    mirageContextData->mirageUserType.insert({ typeNameS,{ typeNameS,fields } });
            }
               
        }

        /*
        unsigned long size = 0;

        if (symbol->get_sizeInUdt(&size) == S_OK)
        {
            __debugbreak();
        }

        DWORD buffer[1024];
        IDiaSymbol* symbols;

        if (symbol->get_types(sizeof(buffer), buffer, &symbols) == S_OK)
        {

            __debugbreak();

        }
        */

         symbol.Release();

    }

}

void ParsePdb(mirage::MirageContextData* mirageContextData, const std::wstring& pdbPath)
{
    CComPtr<IDiaSession> session;
    CComPtr<IDiaSymbol> globalScope;
    OpenPdb(pdbPath, session, globalScope);

    //std::thread t = std::thread([&]() {HandleEnum(mirageContextData, session, globalScope); });
    std::thread t2 = std::thread([&]() {HandleUserType(mirageContextData, session, globalScope); });   

    //t.join();
    t2.join();
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