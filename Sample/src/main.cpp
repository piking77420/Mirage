
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

    if (f.mirageTypeDescriptor.miratageTypeDescriptorFlagBit & mirage::PTR)
    {
        auto it = TrivialTypeToStringSize(f.mirageTypeDescriptor.mirageType.trivialType);
        std::cout << "field type name : " << it.first << '\n';
        std::cout << "field type size : " << it.second << '\n';
    }
    else
    {
         auto type = _contex->GetType(f.mirageTypeDescriptor.mirageType.mirageTypeId);
         if (!type)
             return;

         std::cout << "field type name : " << type->name << '\n';
         std::cout << "field type id : " << type->mirageTypeId << '\n';
         std::cout << "field type size : " << type->size << '\n';

    } 

}

void PrintIneritence(const mirage::MirageContext* _contex, const mirage::MirageType& _mirageType)
{
    std::cout << "Inherited type" << '\n';

    for (auto& it : _mirageType.inhertedType)
    {
        const  mirage::MirageType* mtype = _contex->GetType(it);
        if (mtype)
        {
            std::cout << mtype->name << '\n';
            PrintIneritence(_contex, *mtype);
        }
        else
        {
            assert(false && "that means there is no order for type deduction or missin type");
        }
    }
  
}

void PrintInfoOfType(const mirage::MirageContext* _contex, const mirage::MirageType& _mirageType)
{

    std::cout << "Type : " << _mirageType.name << '\n';
    std::cout << "size : " << _mirageType.size << '\n';
    std::cout << "id : " << _mirageType.mirageTypeId << '\n';

    if (!_mirageType.inhertedType.empty())
        PrintIneritence(_contex, _mirageType);


    for (auto& it : _mirageType.fields)
    {
        std::cout << '\n';
        PrintFieldInfo(_contex, it);
    }
    std::cout << '\n';

   
}

void Print(mirage::MirageContext& _context)
{
    auto it = _context.GetType<MyClass::Vec2>();
    PrintInfoOfType(&_context, *it);

    std::string_view sv = "Vec2";
    it = _context.GetType(mirage::HashStringToId(sv.data(), sv.size()));
    assert(it == nullptr);

    it = _context.GetType<MyClass>();
    PrintInfoOfType(&_context, *it);


    it = _context.GetType<MyClassVirtual>();
    PrintInfoOfType(&_context, *it);

    it = _context.GetType<MyClassDerived>();
    PrintInfoOfType(&_context, *it);
}

int main(int arc, char** arcv)
{
#ifdef _DEBUG
    std::vector<std::wstring> s = {
       { L"SampleExemple/Debug/SampleExemple.pdb" }
    };
#else
    std::vector<std::wstring> s = {
       { L"SampleExemple/Release/SampleExemple.pdb" }
    };
#endif // DEBUG
    mirage::CreateMirageContext createMirageContext =
    {
        s,
        &AttributeNameToInt
    };

    mirage::MirageContext mirageContext(createMirageContext);

#if 1
    Print(mirageContext);
#endif
    auto it = mirageContext.GetType<TestStruct>();


    std::getchar();

    return 0;
}
