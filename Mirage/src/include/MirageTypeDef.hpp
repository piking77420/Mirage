#pragma once

#include <mutex>
#include <vector>
#include <string>


namespace mirage
{
	enum EnumTypeValue
	{
		NONE,
		INT,
		UIINT,
		FLOAT,
		DOUBLE,
	};

	struct EnumMember
	{
		std::string name;
		EnumTypeValue enumTypeValue;
		union
		{
			int intv;
			uint32_t uintv;
			float floatv;
			double doublev;
		}value;
	};

	struct MirageEnum
	{
		std::string name;
		std::vector<EnumMember> enumMember;
	};


	struct MirageContextData
	{
		std::mutex lock;
		std::vector<MirageEnum> mirageEnum;
	};
}