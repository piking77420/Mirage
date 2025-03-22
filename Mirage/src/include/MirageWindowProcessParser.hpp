#pragma once


#include <cstdint>
#include <vector>
#include <string>



namespace mirage
{
	struct MirageContextData;

	void Init();

	void ParseDebugFile(MirageContextData* mirageContextData, const std::vector<std::wstring>& targetReflect);

	void Destroy();
}