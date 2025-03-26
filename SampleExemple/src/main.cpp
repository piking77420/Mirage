

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

    Vec2 vec2;

    int field1;

    float field2;

	MyClass() = default;
	~MyClass() = default;
private:

};

class MyClassVirtual
{
public:
    MyClassVirtual() = default;
   virtual ~MyClassVirtual() = default;

   float field3;

   /*
   __forceinline virtual const char* PrintClassName() const final
   {
      return typeid(*this).name();
   }*/
private:

};

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
    MyClassVirtual v;
    MyClassDerived d;
    d.field3 = 3232.f;

 

    return (int)d.field3;
}
