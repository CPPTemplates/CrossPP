#pragma once
#include "lastPowerLevelData.h"
#include "noteTypeID.h"
constexpr int maxNoteBlockNote = 24;

struct noteBlockData : lastPowerLevelData
{
	int note = 0;

	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeValue(nbtSerializer& s) override;
	void playNote(tickableBlockContainer* containerIn, cveci2& position);
	
};