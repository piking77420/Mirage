#pragma once

#include "MirageTypeDef.hpp"

namespace mirage
{
	std::string CleanUpTypeName(const std::string& typeName);

    constexpr static inline MirageTypeId HashStringToId(const char* _wc, size_t _size)
    {
        uint32_t hash = sizeof(uint32_t) == 8 ? 0xcbf29ce484222325 : 0x811c9dc5;
        const uint32_t prime = sizeof(uint32_t) == 8 ? 0x00000100000001b3 : 0x01000193;

        for (size_t i = 0; i < _size; i++)
        {
            hash ^= static_cast<uint32_t>(*_wc);
            hash *= prime;
            ++_wc;
        }

        return hash;

    }
    //https://stackoverflow.com/questions/2111667/compile-time-string-hashing
    constexpr static inline MirageTypeId HashStringToId(const char* _wc)
    {
        return HashStringToId(_wc, std::strlen(_wc));
    }


}