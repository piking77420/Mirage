#include <cstdint>
#include "../../../Mirage/src/include/MirageReflect.hpp"
#include <tuple>

/// Start

template <int T, int T2>
struct MinMaxI
{
    static_assert(T < T2, "T should be less than T2");
};

enum class EditorVisibility
{
    EditAnywhere,
    EditDefaultsOnly,
    EditInstanceOnly,
};


/// 
/// END OF EXEMPLE ATTRIBUTE
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
    MIRAGE_ATTRIBUTES(x1, Clamp(10, 100), EditEnywhere);

    int* ptr = nullptr;

    float v;

    char s[24];
};
MIRAGE_CLASS(TestStruct)

class MyClass
{
public:
    struct Vec2
    {
        double x;

        double y;
    };

    Vec2 vec2;

    int field1;

    float field2;

    MyClass() = default;
    ~MyClass() = default;
private:

};

//typed InheritedClass MyClass;

class MyClassVirtual : public MyClass
{
public:
    MyClassVirtual() = default;
    virtual ~MyClassVirtual() = default;

    float field3;

    virtual  int GetInt()
    {
        return 3;
    }

  
private:

};

// with this class paring with DIA SymTagData
// YOU will get no child beacause MyClassDerived has no field 
// On the other hand using SymTagUDT or SymTagNull you get MyClass
// so you can iterate over this intead
class MyClassDerived : public MyClassVirtual 
{
public:

    

    MyClassDerived() = default;

    ~MyClassDerived()= default;

    int GetInt()
    {
        return 8;
    }
private:

};