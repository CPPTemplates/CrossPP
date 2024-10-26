#include "math/graphics/brush/font/baseFont.h"
#include "math/collisions.h"
#include "colors.h"
#include <string>
#include "math/graphics/texture.h"
#include "math/graphics/color/color.h"
#include "math/graphics/brush/brushes.h"
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectn.h"
#include "globalFunctions.h"
#include "constants.h"
#pragma once

extern fontFamily* currentMinecraftFontFamily;
struct minecraftFont : public baseFont {
	minecraftFont(cfp& fontSize = defaultFontSize) : baseFont(*currentMinecraftFontFamily, fontSize) {}
	virtual vec2 DrawString(const std::wstring& text, crectangle2& rect, cvec2& offset, const texture& renderTarget, const std::optional<mat3x3>& matrix = std::nullopt) const override;
	
	using baseFont::DrawString;
};
extern minecraftFont* currentMinecraftFont;