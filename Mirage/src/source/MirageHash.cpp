#include "MirageHash.hpp"

#include <iostream>
#include <regex>







namespace mirage
{
    std::string CleanUpTypeName(const std::string& typeName)
    {
        std::regex classStructRegex(R"(class\s+|struct\s+)");

        return std::regex_replace(typeName, classStructRegex, "");
    }
}
