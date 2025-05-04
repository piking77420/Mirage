#pragma once

#include <mutex>
#include <vector>
#include <string>
#include <unordered_map>

#include <guiddef.h>


namespace mirage
{
	using MirageTypeId = uint32_t;
	constexpr MirageTypeId NullMirageTypeId = static_cast<MirageTypeId>(-1);
	using MirageTypeHashFunction = uint32_t(*)(const char*);

	union TypeValue
	{
		bool boolv;
		int intv;
		uint32_t uintv;
		float floatv;
		double doublev;
	};

	enum TrivialType
	{
		VOID,
		BOOL,
		CHAR,
		WCHAR,
		INT,
		UIINT,
		FLOAT,
		DOUBLE,

		COUNT,
	};

	struct EnumMember
	{
		std::string name;
		TrivialType enumTypeValue;
		TypeValue value;
	};

	struct MirageEnum
	{
		std::string name;
		std::vector<EnumMember> enumMember;
	};

	struct MirageType;

	struct MirageTypeDescriptor
	{
		bool isTrivial;
		union
		{
			TrivialType trivialType;
			MirageTypeId mirageTypeId;
		}mirageType;
	};
	struct MirageField
	{
		MirageTypeDescriptor mirageTypeDescriptor;
		std::string name;
		uint32_t offset;
	};

	struct MirageType
	{
		// TO DO CAHNGE FOR TYPE ID
		MirageTypeId mirageTypeId;
		size_t size;
		std::string name;
		std::vector<MirageField> fields;

		std::vector<MirageTypeId> inhertedType;
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