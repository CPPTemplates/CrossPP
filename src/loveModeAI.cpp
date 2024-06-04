#include "loveModeAI.h"
#include "nbt/serializeUUID.h"
bool loveModeAI::shouldExecute()
{
	mob* connectedMob = (mob*)connectedEntity;
	if (connectedMob)
	{

	}

	return false;
}

void loveModeAI::serializeValue(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"partner found"), partnerFound);
}