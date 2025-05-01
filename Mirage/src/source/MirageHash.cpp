#include "MirageHash.hpp"

#include <iostream>
#include <regex>





//https://stackoverflow.com/questions/2111667/compile-time-string-hashing
uint32_t cx_hash(const char* input) {
    uint32_t hash = sizeof(uint32_t) == 8 ? 0xcbf29ce484222325 : 0x811c9dc5;
    const uint32_t prime = sizeof(uint32_t) == 8 ? 0x00000100000001b3 : 0x01000193;

    while (*input) {
        hash ^= static_cast<uint32_t>(*input);
        hash *= prime;
        ++input;
    }

    return hash;
}


namespace mirage
{
    std::string CleanUpTypeName(const std::string& typeName)
    {
        std::regex classStructRegex(R"(class\s+|struct\s+)");

        return std::regex_replace(typeName, classStructRegex, "");
    }

    MirageTypeId mirage::HashStringToId(const char* _wc)
    {
        return cx_hash(_wc);
    }
}
