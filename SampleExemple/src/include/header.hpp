
enum UserAttribute
{
    REFLECT,
    SERIALIZE,
    MIN_MAX,

    COUT
};

#define REFLECT(x) \
static constexpr bool __REFLECT__##x##_ = true; \

#define SERIALIZE(x) \
static constexpr bool __SERIALIZE__##x##_ = true; \

#define MIN_MAX(x, min, max) \
struct  Pair##x##_\
{ \
    decltype(x) first;\
    decltype(x) second;\
};\
static constexpr Pair##x##_ MIN_MAX__##x##_ = {min, max}; \


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
    SERIALIZE(x1);

    int* ptr = nullptr;

    float v;
    MIN_MAX(v, 0.f, 10.f);

    char s[24];
};

struct Vec2
{
    double x;
        
    double y;
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