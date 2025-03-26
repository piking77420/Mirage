
#include <iostream>

#include "MirageContext.hpp"



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
    mirage::MirageContext mirageContext(s);

    if (auto t = mirageContext.GetType("MyClassVirtual"))
    {
        std::cout << t->typeName << '\n';

        for (auto& it : t->fields)
        {
            std::cout << it.name << " : " <<  it.offset << '\n';

        }
    }


    std::getchar();

    return 0;
}
