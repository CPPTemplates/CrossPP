#include "rectParticleBrush.h"
#include "nbt/serializeRectangle.h"
#include "math/graphics/brush/brushes.h"
void rectParticleBrush::serializeValue(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"brush rect"), brushRect);
}

void rectParticleBrush::render(crectangle2& rect, const gameRenderData& targetData) const
{
	cmat3x3& transForm = mat3x3::combine({ mat3x3::fromRectToRect(brushRect,rect),targetData.worldToRenderTargetTransform });
	fillTransparentRectangle(brushRect, transForm, getTexture(), targetData.renderTarget);
}