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

	struct MirageField
	{
		std::string name;
		uint32_t offset;
	};

	struct MirageType
	{
		std::string typeName;
		std::vector<MirageField> fields;
	};


	struct MirageContextData
	{
		std::mutex lock;
		std::vector<MirageEnum> mirageEnum;

		// string from typeid().name
		std::unordered_map<std::string, MirageType> mirageUserType;
	};
}