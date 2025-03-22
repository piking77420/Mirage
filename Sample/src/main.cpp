
#include <iostream>

#include "MirageContext.hpp"

enum MyEnum
{
    ONE,
    TWO,
    COUNT
};

enum class MyEnumClass
{
    ONE,
    TWO,
    COUNT
};


struct Vec2
{
    float x, y;
};

class MyClass
{
public:
	MyClass() = default;
	~MyClass() = default;
private:

};

class MyClassVirtual
{
public:
    MyClassVirtual() = default;
   virtual ~MyClassVirtual() = default;
private:

};

static_assert(std::is_polymorphic_v<MyClassVirtual>, "Should be polymorphic");

class MyClassDerived : public MyClassVirtual
{
public:
    MyClassDerived() = default;

    ~MyClassDerived() override = default;
private:

};

int main(int arc, char** arcv)
{
    MyEnum myEnu;

#ifdef _DEBUG
    std::vector<std::wstring> s = {
       { L"Debug/Sample.pdb" }
    };
#else
    std::vector<std::wstring> s = {
       { L"Release/Sample.pdb" }
    };
#endif // DEBUG
    mirage::MirageContext mirage(s);

    std::getchar();

    return 0;
}
