#include "fireworkRocket.h"
#include "itemData.h"
#include "nbt/nbtSerializer.h"
#include "constants/vectorConstants.h"
#include "include/math/graphics/brush/brushes.h"
void fireworkRocket::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"ticks to exist"), ticksToExist);
	data.serialize(s, std::wstring(L"item data"));
}
void fireworkRocket::render(const gameRenderData& targetData) const
{
	fillTransparentRectangle(blockTextureRect, calculateHitBox(), targetData.worldToRenderTargetTransform, *itemList[itemID::firework_rocket]->tex, targetData.renderTarget);
}