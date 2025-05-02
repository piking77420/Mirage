
#include <iostream>

#include "MirageContext.hpp"
#include "../../SampleExemple/src/include/header.hpp"
#include <typeinfo>

uint32_t MyHashFun(const char* ptr)
{
    return {};
}

std::pair<const char*, size_t> TrivialTypeToStringSize(mirage::TrivialType trivialType)
{
    switch (trivialType)
    {
    case mirage::VOID:
        return { "void", 0 };
            break;
    case mirage::BOOL:
        return {"bool", sizeof(bool)};
            break;
    case mirage::CHAR:
        return {"char", sizeof(char)};
            break;
    case mirage::WCHAR:
        return {"wchar_t", sizeof(wchar_t)};
            break;
    case mirage::INT:
        return {"int", sizeof(int)};
            break;
    case mirage::UIINT:
        return {"uint", sizeof(uint32_t)};
            break;
    case mirage::FLOAT:
        return {"float", sizeof(float)};
            break;
    case mirage::DOUBLE:
        return {"double", sizeof(double)};
            break;
    case mirage::COUNT:
    default:
        break;
    }
}

void PrintFieldInfo(const mirage::MirageContext* _contex, const mirage::MirageField& f)
{
    std::cout << "field name : " << f.name << '\n';
    std::cout << "field offset : " << f.offset << '\n';

    if (f.mirageTypeDescriptor.isTrivial)
    {
        auto it = TrivialTypeToStringSize(f.mirageTypeDescriptor.mirageType.trivialType);
        std::cout << "field type name : " << it.first << '\n';
        std::cout << "field type size : " << it.second << '\n';
    }
    else
    {
        auto type = _contex->GetType(f.mirageTypeDescriptor.mirageType.mirageTypeId);
         std::cout << "field type name : " << type->name << '\n';
         std::cout << "field type id : " << type->mirageTypeDescriptor.mirageType.mirageTypeId << '\n';
         std::cout << "field type size : " << type->size << '\n';

    } 

}

void PrintInfoOfType(const mirage::MirageContext* _contex, const mirage::MirageType& _mirageType)
{
    if (_mirageType.mirageTypeDescriptor.isTrivial)
        return;

    std::cout << "Type : " << _mirageType.name << '\n';
    std::cout << "size : " << _mirageType.size << '\n';
    std::cout << "id : " << _mirageType.mirageTypeDescriptor.mirageType.mirageTypeId << '`\n';

    for (auto& it : _mirageType.fields)
    {
        std::cout << '\n';
        PrintFieldInfo(_contex, it);
    }
    std::cout << '\n';

   
}

int main(int arc, char** arcv)
{
    mirage::MirageTypeHashFunction hash(&MyHashFun);

#ifdef _DEBUG
    std::vector<std::wstring> s = {
       { L"SampleExemple/Debug/SampleExemple.pdb" }
    };
#else
    std::vector<std::wstring> s = {
       { L"SampleExemple/Release/SampleExemple.pdb" }
    };
#endif // DEBUG
    mirage::MirageContext mirageContext(s);

   
    /*
    PrintInfoOfType(&mirageContext, "Vec2");
    PrintInfoOfType(&mirageContext, "MyClassVirtual");
    PrintInfoOfType(&mirageContext, "MyClass");*/

    auto it = mirageContext.GetType("Vec2");
    PrintInfoOfType(&mirageContext, *it);

    it = mirageContext.GetType("MyClass");
    PrintInfoOfType(&mirageContext, *it);

    std::getchar();

    return 0;
}
