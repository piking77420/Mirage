#pragma once
#include "SimpleTypeName.hpp"
#include "MirageHash.hpp"

// This don't take into account the nul char
#define COMPILE_TIME_CRC32_STR(x) (mirage::HashStringToId(x.data(), x.size()))


#define COMPILE_TIME_TYPE_KEY(T) COMPILE_TIME_CRC32_STR(skydown::short_type_name<T>)


#define RUNTIME_TIME_TYPE_KEY()