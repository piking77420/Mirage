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

struct TestStruct
{
    int x1;
};

struct Vec2
{
    double x, y;
};

class MyClass
{
public:

    Vec2 vec2;

    int field1;

    float field2;

    MyClass() = default;
    ~MyClass() = default;
private:

};

class MyClassVirtual : public MyClass
{
public:
    MyClassVirtual() = default;
    virtual ~MyClassVirtual() = default;

    float field3;

    int GetInt()
    {
        return 3;
    }

  
private:

};

// with this class paring with DIA SymTagData
// YOU will get no child beacause MyClassDerived has no field 
// On the other hand using SymTagUDT or SymTagNull you get MyClass
// so you can iterate over this intead
class MyClassDerived : public MyClass
{
public:
    MyClassDerived() = default;

    ~MyClassDerived()= default;
private:

};