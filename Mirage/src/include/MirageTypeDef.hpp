#pragma once

#include <mutex>
#include <vector>
#include <string>
#include <unordered_map>


namespace mirage
{
	using MirageTypeId = size_t;

	union TypeValue
	{
		bool boolv;
		int intv;
		uint32_t uintv;
		float floatv;
		double doublev;
	};

	enum EnumTypeValue
	{
		NONE,
		BOOL,
		INT,
		UIINT,
		FLOAT,
		DOUBLE,
	};

	struct EnumMember
	{
		std::string name;
		EnumTypeValue enumTypeValue;
		TypeValue value;
	};

	struct MirageEnum
	{
		std::string name;
		std::vector<EnumMember> enumMember;
	};

	struct Field
	{
		EnumTypeValue enumTypeValue;
		uint32_t offset;
	};


	struct MirageContextData
	{
		std::mutex lock;
		size_t mirageIdCount = 0;

		std::vector<MirageEnum> mirageEnum;
	};
}