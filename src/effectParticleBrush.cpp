
#include "effectParticleBrush.h"
#include "math/graphics/brush/brushes.h"
#include "math/graphics/resolutiontexture.h"
#include "math/mattnxn.h"
#include "math/rectangle/rectangletn.h"
#include "math/graphics/brush/brushes/colorMultiplier.h"
#include "gameRenderData.h"

void effectParticleBrush::render(crectangle2& rect, const gameRenderData& targetData) const
{
	const resolutionTexture& tex = getTexture();
	fillTransparentRectangle(tex.getClientRect(), mat3x3::cross(targetData.worldToRenderTargetTransform, mat3x3::fromRectToRect(tex.getClientRect(), rect)),
		colorMultiplier<solidColorBrush, resolutionTexture>(solidColorBrush(effectColor), tex), targetData.renderTarget);
}
