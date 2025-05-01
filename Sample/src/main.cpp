
#include <iostream>

#include "MirageContext.hpp"
#include "../../SampleExemple/src/include/header.hpp"
#include <typeinfo>

uint32_t MyHashFun(const char* ptr)
{
    return {};
}

void PrintInfoOfType(mirage::MirageContext* _context, const std::string& _s)
{
    if (auto t = _context->GetType(_s))
    {
        std::cout << _s << '\n';

        for (auto& it : t->fields)
        {
            std::cout << it.name << " , offset = " << it.offset << '\n';

        }
        std::cout << '\n';
    }
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

    auto it = mirageContext.GetType("MyClassDerived");


    std::getchar();

    return 0;
}
