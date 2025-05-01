#include "MirageTypeDef.hpp"
#include <MirageHash.hpp>

using namespace mirage;


void mirage::AddMirageType(MirageContextData* _mirageContext, const std::string& _s, MirageTypeId _typeId, MirageType mirageType)
{
    if (_s == "MyClassDerived")
    {
        __debugbreak();
    }

    _mirageContext->mirageUserType.insert({ _typeId, mirageType });
}

bool mirage::ContainType(MirageContextData* _mirageContext, mirage::MirageTypeId id)
{
    return _mirageContext->mirageUserType.find(id) != _mirageContext->mirageUserType.end();
}

std::string mirage::GetStringFromWchart(wchar_t* ptr)
{
    std::wstring ws(ptr);
    return std::string(ws.begin(), ws.end());
}
