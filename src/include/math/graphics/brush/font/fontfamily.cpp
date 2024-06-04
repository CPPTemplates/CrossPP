#include "fontfamily.h"
#include "math/graphics/texture.h"
#include "math/graphics/brush/brushes.h"
#include "math/graphics/brush/transformbrush.h"
#include "math/rectangle/rectangletn.h"

//truct letterDrawer : public colorBrush
//
//	const colorBrush* bottomBrush = nullptr;
//	const colorBrush* fontBrush = nullptr;
//	rectangle2 letterRect = vec2();
//	rectangle2 maskBrushRect = vec2();
//	virtual inline color getValue(cvec2& pos) const override
//	{
//		const color c = fontBrush->getValue((pos - letterRect.pos0) / letterRect.size * maskBrushRect.size + maskBrushRect.pos0);
//		return colorMixer<>::getColor(c, bottomBrush->getValue(pos));
//	}
//;

fontFamily::fontFamily(resolutionTexture* tex, cbool& flipRows)
{
	this->tex = tex;
	texture& topTextureToEdit = *tex->scaledTextures[0];
	cvect2<size_t>& texLetterSize = topTextureToEdit.size / asciiRowWidth;
	if (flipRows)
	{
		for (size_t yRow0 = 0; yRow0 < asciiColumnHeight / 2; yRow0++)
		{
			size_t yRow1 = (asciiColumnHeight - yRow0 - 1);
			for (size_t aX = 0; aX < topTextureToEdit.size.x; aX++)
			{
				for (size_t yIndex = 0; yIndex < texLetterSize.y; yIndex++)
				{
					size_t aY0 = yRow0 * texLetterSize.y + yIndex;
					size_t aY1 = yRow1 * texLetterSize.y + yIndex;
					//swap colors
					std::swap(topTextureToEdit.getValueReferenceUnsafe(cvect2<size_t>(aX, aY0)), topTextureToEdit.getValueReferenceUnsafe(cvect2<size_t>(aX, aY1)));
				}
			}
		}
		tex->recalculateScaledTextures();
	}
	
	int fontScaleMultiplier = (int)(topTextureToEdit.size.x / asciiRowWidth);
	for (size_t i = 0; i < asciiLetterCount; i++) {

		cveci2& asciiOffset = getAsciiOffset((byte)i);
		cveci2& multipliedOffset = asciiOffset * fontScaleMultiplier;
		crectanglei2& rectToSearch = rectanglei2(multipliedOffset, veci2(fontScaleMultiplier));
		int width = 0;
		rectanglei2 pixelRect = rectanglei2();
		for (int y = 0; y < fontScaleMultiplier; y++){
			for (int x = 0; x < fontScaleMultiplier; x++) {
				veci2 realPos = veci2(x, y) + multipliedOffset;
				if (topTextureToEdit.getValueUnsafe(realPos).a())
				{
					if (!width) {
						pixelRect = rectanglei2(realPos, veci2(1));
					}
					else {
						pixelRect.expandToContain(crectanglei2(realPos, veci2(1)));
					}
					width = math::maximum(width, x);
				}
			}
		}
		letterWidths[i] = width / (fp)fontScaleMultiplier;
		croppedLetterRects[i] = crectangle2(pixelRect).multiplied(1.0 / tex->getScaleModifier());
	}
}

void fontFamily::DrawLetter(cletter& l, cvec2& position, cfp& letterSize, const texture& renderTarget) const
{
	DrawLetter(l, position, letterSize, renderTarget, *tex);
}

void fontFamily::DrawLetter(cletter& l, cmat3x3& transformFrom1x1, const texture& renderTarget) const
{
	DrawLetter(l, transformFrom1x1, renderTarget, *tex);
}
