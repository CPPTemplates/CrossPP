#include "math/graphics/resolutiontexture.h"
#include <optional>
#include "math/graphics/brush/brushes.h"
#pragma once

//0x10 letters in a row
constexpr size_t asciiRowWidth = 0x10;
constexpr size_t asciiColumnHeight = 0x10;
//the amount of letters in an ascii charachter set
constexpr size_t asciiLetterCount = 0x100;

constexpr veci2 getAsciiOffset(byte letterIndex) {

	return cveci2(letterIndex % asciiRowWidth,//the x index of the letter texture
		letterIndex / asciiRowWidth);//the y index of the letter texture
}

class fontFamily
{
public:
	resolutionTexture* tex;
	fp letterWidths[asciiLetterCount];
	rectangle2 croppedLetterRects[asciiLetterCount];

	//the texture has to have a transparent background with black letters.
	//Be careful: the actual texture will be modified!
	fontFamily(resolutionTexture* tex, cbool& flipRows = true);
	void DrawLetter(cletter& l, cvec2& position, cfp& letterSize, const texture& renderTarget) const;
	void DrawLetter(cletter& l, cmat3x3& transformFrom1x1, const texture& renderTarget) const;
	//template <typename textBrush>
	//void DrawLetter(cletter& l, cvec2& position, cfp& letterSize, const std::optional<mat3x3>& matrix, const texture& renderTarget, const textBrush& b) const;
	template<typename textBrush, typename = std::enable_if_t<is_brush_v<textBrush>>>
	inline void DrawLetter(cletter& l, cvec2& position, cfp& letterSize, const texture& renderTarget, const textBrush& b, const std::optional<mat3x3>& matrix = std::nullopt) const
	{
		cmat3x3& transform = mat3x3::fromRectToRect(crectangle2(0, 0, 1, 1), crectangle2(position, cvec2(letterSize)));
		return matrix.has_value() ?
			//rect(1) -> position, size -> world
			DrawLetter(l, mat3x3::cross(matrix.value(), transform), renderTarget, b)
			:
			DrawLetter(l, transform, renderTarget, b);
	}
	//transform: from 
	template<typename textBrush>
	void DrawLetter(cletter& l, cmat3x3& transformFrom1x1, const texture& renderTarget, const textBrush& b) const;

private:

};


template<typename textBrush>
inline void fontFamily::DrawLetter(cletter& l, cmat3x3& transformFrom1x1, const texture& renderTarget, const textBrush& b) const
{
	//cint& letterIndex = l & 0xff;
	cveci2& asciiOffset = getAsciiOffset((byte)l);

	cveci2& texLetterSize = cveci2((int)(tex->defaultSize.x / asciiRowWidth));

	crectanglei2& texMaskRect = crectanglei2(asciiOffset * texLetterSize, texLetterSize);

	cmat3x3& multipliedTransform = mat3x3::cross(transformFrom1x1, mat3x3::fromRectToRect(crectangle2(texMaskRect), crectangle2(cvec2(1))));

	//auto drawer = transformBrush<texture>(multipliedTransform, *tex);
	fillTransparentRectangle(croppedLetterRects[(byte)l], multipliedTransform, b, renderTarget);
}
