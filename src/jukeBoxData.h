#pragma once
#include "blockData.h"
#include "rectangularSlotContainer.h"

struct jukeBoxData : blockData
{
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;

	rectangularSlotContainer* recordSlot = nullptr;
	//std::shared_ptr<struct music2d> musicPlaying = nullptr;

	jukeBoxData() : recordSlot(new rectangularSlotContainer(cveci2(1))) {}
	virtual void onBlockRemove(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual ~jukeBoxData() override;
	void playMusic(tickableBlockContainer* containerIn, cveci2& position);
};