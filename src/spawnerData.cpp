#include "spawnerData.h"
#include "entity.h"
#include "idConverter.h"
#include "include/math/random/random.h"
void spawnerData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	//spawn mobs
	constexpr fp spawnBoxSize = 4;
	trySummonEntity(entityToSpawn, containerIn, cvec2(position) + vec2(randFp(currentRandom, -spawnBoxSize, spawnBoxSize), randFp(currentRandom, -spawnBoxSize, spawnBoxSize)));
}

void spawnerData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	s.serializeValue(std::wstring(L"spawn cooldown"), spawnCoolDown);
	idConverter::serializeID(s, std::wstring(L"mob type"), entityToSpawn, s.converter ? &s.converter->entityIDConverter : nullptr);
}