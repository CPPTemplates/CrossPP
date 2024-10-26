#include "soundPacket.h"
#include <string>
#include "nbt/nbtSerializer.h"
#include "nbt/serializeColor.h"

bool soundPacket::serialize(nbtSerializer& s)
{
	serializeNBTValue(s, L"position", position);
	s.serializeValue(L"key", key);
	s.serializeValue(L"sound index", soundIndex);
	s.serializeValue(L"volume", volume);
	return s.serializeValue(L"pitch", pitch);
}
