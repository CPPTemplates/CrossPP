#pragma once
#include "nbt/nbtSerializer.h"
struct soundPacket {
	vec2 position;
	std::wstring key;
	int soundIndex;
	fp volume;
	fp pitch;
	soundPacket(cvec2& position = vec2(), const std::wstring& key = std::wstring(), cint& soundIndex = 0, cfp& volume = 1, cfp& pitch = 1) :
		position(position), key(key), soundIndex(soundIndex), volume(volume), pitch(pitch) {}
	bool serialize(nbtSerializer& s);
};