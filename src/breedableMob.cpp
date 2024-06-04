#include "breedableMob.h"
#include "nbt/nbtSerializer.h"
void breedableMob::tick()
{
	age++;
}

void breedableMob::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"age"), age);
}