#include "overWorldGenerationData.h"
void overworldGenerationData::serializeValue(nbtSerializer& s)
{
	s.serializeArray(std::wstring(L"heights"), heights, (int)chunkSize.x);
	s.serializeArray(std::wstring(L"biomes"), (int*)biomes, (int)chunkSize.x);
}

overworldGenerationData::~overworldGenerationData()
{
	delete[] heights;
	delete[] biomes;
}