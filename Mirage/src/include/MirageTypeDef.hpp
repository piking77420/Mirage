#pragma once

#include <mutex>
#include <vector>
#include <string>
#include <unordered_map>

#include <guiddef.h>

#define START_ATTRIBUTE "__"
#define MAX_ATTRIBUTE 64;

namespace mirage
{
	using MirageTypeId = uint32_t;
	constexpr MirageTypeId NullMirageTypeId = static_cast<MirageTypeId>(-1);
	using MirageTypeHashFunction = uint32_t(*)(const char*);

	using MirageTypeAttributeToString = uint32_t(*)(const char*);

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

	enum MiratageTypeDescriptorFlags : uint8_t
	{
		NONE = 0,
		TRIVIAL = 0 << 1,
		PTR = 0 << 2,
		ARRAY = 0 << 2,
	};
	using MiratageTypeDescriptorFlagBit = uint8_t;

	struct MirageArray
	{
		MirageTypeId mirageTypeId;
		// can add 4 byte
		size_t size;
	};

	struct MirageTypeDescriptor
	{
		MiratageTypeDescriptorFlagBit miratageTypeDescriptorFlagBit;
		union
		{
			MirageTypeId mirageTypeId;
			TrivialType trivialType;
			MirageArray mirageArray;
		}mirageType;
	};

	struct MirageAttributeHeader
	{
		uint64_t attributeType;
		size_t dataSize;
	};

	struct MirageField
	{
		MirageTypeDescriptor mirageTypeDescriptor;
		std::string name;
		uint32_t offset;
		std::vector<uint8_t> attributeData;
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
		MirageTypeAttributeToString mirageTypeAttributeToString;
	};

	void AddMirageType(MirageContextData* _mirageContext,const std::string& _s, MirageTypeId _typeId, MirageType mirageType);

	bool ContainType(MirageContextData* _mirageContext, mirage::MirageTypeId id);

	std::string GetStringFromWchart(wchar_t* ptr);

}