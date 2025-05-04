
#if 0

#include "../../Mirage/src/include/CompiletimeKey.hpp"
#include <typeinfo>
#include <memory>

#endif
#include "include/header.hpp"



int main(int arc, char** arcv)
{
    MyEnum myEnu;
    MyClass myClass;
    myClass.field1 = 2.;
    myClass.field2 = 2.f;
    myClass.vec2 = { 0.f, 0.f };


    MyClassVirtual v;
    int c = v.GetInt();

    MyClassDerived d = {};
    d.field2 = 3232.f + c;

    TestStruct destStruct = {};
    destStruct.ptr = &c;
    *destStruct.ptr = *destStruct.ptr + 1;
#if 0
    std::shared_ptr<MyClassVirtual> sharedPtr = std::make_shared<MyClassDerived>();

    std::string name = skydown::get_type_name(sharedPtr.get());
#endif
    
    return (int)d.field2;
}
