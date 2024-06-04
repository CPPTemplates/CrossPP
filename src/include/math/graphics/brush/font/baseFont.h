#pragma once
#include "fontfamily.h"

constexpr fp defaultFontSize = 0x10;

struct baseFont
{
	fontFamily& family;
	fp fontSize = defaultFontSize;

	//this includes the font size!
	fp lineSpacing = 1;

	baseFont(fontFamily& family, fp fontSize = 0x10) :
		family(family), fontSize(fontSize) {}

	constexpr fp getLineHeight() const
	{
		return fontSize + lineSpacing;
	}

	constexpr static bool drawable(const letter& l)
	{
		return l != L'\n' && l != L'\t' && l != L'\r';
	}

	inline vec2 MeasureLetterOffset(crectangle2& bounds, cvec2 position, const letter& l) const
	{
		if (l == L'\n')
		{
			return vec2(bounds.x, position.y - getLineHeight());
		}
		else
		{
			constexpr fp relativeLetterSpacing = 0.2;
			cfp& letterWidth = (std::wstring(L" ").find(l) != std::wstring::npos) ? 
				fontSize : 
				fontSize * (family.letterWidths[(byte)l] + relativeLetterSpacing);

			cfp& newX = position.x + letterWidth;
			if (position.x > bounds.w + bounds.x)
			{
				//new line
				return vec2(bounds.x + letterWidth, position.y - getLineHeight());
			}
			else
			{
				return vec2(newX, position.y);
			}
		}

	}
	inline vec2 MeasureStringOffset(crectangle2& bounds, const std::wstring& text = std::wstring()) const
	{
		//start at top left
		vec2 offset = vec2(bounds.pos0.x, bounds.pos0.y + bounds.h - fontSize);
		for (const auto& c : text)
		{
			offset = MeasureLetterOffset(bounds, offset, c);
		}
		return offset;
	}
	inline vec2 measureStringSize(cvec2& maximumSize, const std::wstring& text)
	{
		//start at top left
		fp widestX = 0;
		vec2 offset = vec2();
		crectangle2 maximalBounds = crectangle2(0, maximumSize.y, maximumSize.x, maximumSize.y);
		for (const auto& c : text)
		{
			offset = MeasureLetterOffset(maximalBounds, offset, c);
			widestX = math::maximum(offset.x, widestX);
		}
		return vec2(widestX, -offset.y + getLineHeight());
	}

	template<typename fontBrushType, typename = std::enable_if_t < is_brush_v<fontBrushType>>>
	inline vec2 DrawString(const std::wstring& text, crectangle2& rect, vec2 offset, const texture& renderTarget, const fontBrushType& b, const std::optional<mat3x3>& matrix = std::nullopt) const
	{
		if (text.length())
		{
			const letter* textptr = text.c_str();
			const letter* textendptr = textptr + text.length();

			//top left
			while (textptr < textendptr)
			{
				if (drawable(*textptr))
				{
					family.DrawLetter(*textptr, vec2(offset.x, offset.y), fontSize, renderTarget, b, matrix);
				}
				offset = MeasureLetterOffset(rect, offset, *textptr);
				textptr++;
			}
		}
		return offset;
	}
	inline vec2 DrawString(const std::wstring& text, crectangle2& rect, const texture& renderTarget, const std::optional<mat3x3>& matrix = std::nullopt) const
	{
		return DrawString(text, rect, MeasureStringOffset(rect, std::wstring()), renderTarget, matrix);
	}
	//returns the position for the next letter like measurestring

	template<typename fontBrushType, typename = std::enable_if_t<is_brush_v<fontBrushType>>>
	inline vec2 DrawString(const std::wstring& text, crectangle2& rect, const texture& renderTarget, const fontBrushType& b, const std::optional<mat3x3>& matrix = std::nullopt) const
	{
		return DrawString(text, rect, MeasureStringOffset(rect, std::wstring()), renderTarget, b, matrix);
	}
	inline virtual vec2 DrawString(const std::wstring& text, crectangle2& rect, cvec2& offset, const texture& renderTarget, const std::optional<mat3x3>& matrix = std::nullopt) const
	{
		return DrawString(text, rect, offset, renderTarget, *family.tex, matrix);
	}
};