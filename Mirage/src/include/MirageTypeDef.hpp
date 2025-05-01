#pragma once

#include <mutex>
#include <vector>
#include <string>
#include <unordered_map>

#include <guiddef.h>


namespace mirage
{
	using MirageTypeId = uint32_t;
	using MirageTypeHashFunction = uint32_t(*)(const char*);

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
		MirageTypeId typeId;
		std::string name;
		uint32_t offset;
	};

	struct MirageType;

	struct MirageMethod
	{
		uint64_t procAddress;
	};

	struct MirageType
	{
		std::string name;
		MirageTypeId mirageTypeId;
		std::vector<MirageField> fields;
	};


	struct MirageContextData
	{
		std::mutex lock;
		std::vector<MirageEnum> mirageEnum;

		std::unordered_map<MirageTypeId, MirageType> mirageUserType;
	};

	void AddMirageType(MirageContextData* _mirageContext,const std::string& _s, MirageTypeId _typeId, MirageType mirageType);

	bool ContainType(MirageContextData* _mirageContext, mirage::MirageTypeId id);

	std::string GetStringFromWchart(wchar_t* ptr);

}