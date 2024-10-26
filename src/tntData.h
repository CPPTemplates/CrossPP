#pragma once
#include "blockData.h"
struct tntData :blockData
{
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
};