#pragma once
#include "baseFont.h"
#include <string>
#include "globalFunctions.h"
#include "fontfamily.h"
#include "math/graphics/texture.h"
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectn.h"
#include "math/graphics/brush/brushes/solidColorBrush.h"
#include "math/graphics/brush/brushes/colorMultiplier.h"

template <typename fontBrushType = solidColorBrush>
struct font : baseFont
{
	const fontBrushType *fontBrush = nullptr;

	font(fontFamily &family, fp fontSize = 0x10, const fontBrushType *fontBrush = &brushes::white) : baseFont(family, fontSize), fontBrush(fontBrush) {}
	font(const baseFont &f, const fontBrushType *fontBrush = &brushes::white) : baseFont(f.family, f.fontSize), fontBrush(fontBrush) {}

	// returns the position for the next letter like measurestring
	inline vec2 DrawString(const std::wstring &text, crectangle2 &rect, const texture &renderTarget) const
	{
		return baseFont::DrawString(text, rect, renderTarget, *fontBrush);
	}
	inline vec2 DrawString(const std::wstring &text, crectangle2 &rect, cvec2 &offset, const texture &renderTarget, const std::optional<mat3x3> &matrix = std::nullopt) const override
	{
		if (fontBrush != &brushes::white)
		{
			return baseFont::DrawString(text, rect, offset, renderTarget, *family.tex, matrix);
		}
		else{
			return baseFont::DrawString(text, rect, offset, renderTarget, colorMultiplier(*fontBrush, *family.tex), matrix);
		}
	}
};