#include "textBox.h"
#include "math/timemath.h"
#include "application/interaction.h"
#include "math/graphics/brush/font/baseFont.h"
#include "math/graphics/graphicsFunctions.h"

textBox::textBox(cbool &multiLine) : control(), multiLine(multiLine)
{
}

void textBox::render(cveci2 &position, const texture &renderTarget)
{
	control::render(position, renderTarget);

	// draw cursor
	if (focused && GetSecond() % 2)
	{
		checkCursorIndex();
		rectangle2 textRect = rectangle2(position, rect.size).expanded(-borderSize);
		vec2 cursorPosition = currentFont->MeasureStringOffset(textRect, text.substr(0, cursorIndex));
		fillRectangle(renderTarget, crectangle2(cursorPosition, vec2(1, currentFont->fontSize)), brushes::white);
	}
}

void textBox::enterText(cuint &uniCode)
{
	checkCursorIndex();
	const wchar_t& charEntered = (wchar_t)uniCode;
	constexpr wchar_t pasteChar = L'\x0016';
	constexpr wchar_t copyChar = L'\x0003';
	if (!is_in(charEntered, L'\b', copyChar, pasteChar) &&
		(multiLine || !is_in((wchar_t)uniCode, L'\n', L'\r')))
	{
		text.insert(cursorIndex, 1, (wchar_t)uniCode);
		cursorIndex++;
	}
}

void textBox::keyDown(cvk &keyCode)
{
	checkCursorIndex();
	if (keyCode == vk::Delete && cursorIndex < text.length())
	{
		text = text.erase(cursorIndex, 1);
	}
	if (keyCode == vk::BackSpace && cursorIndex > 0)
	{
		text = text.erase(cursorIndex - 1, 1);
	}
	else if (keyCode == vk::Left && cursorIndex > 0)
	{
		cursorIndex--;
	}
	else if (keyCode == vk::Right && cursorIndex < text.length())
	{
		cursorIndex++;
	}
}

// void textBox::keyUp(cvk &keyCode)
//{
//	shift &= (keyCode != VK_SHIFT);
//	capsLock &= (keyCode != VK_CAPITAL);
// }

void textBox::lostFocus()
{
	shift = false;
	capsLock = false;
}

void textBox::paste(const std::wstring &text)
{
	checkCursorIndex();
	this->text.insert(cursorIndex, text);
	cursorIndex += text.length();
	// std::wstring clipBoardText;
	// if (GetClipboardText(clipBoardText))
	//{
	//	text.insert(cursorIndex, clipBoardText);
	//	cursorIndex += clipBoardText.length();
	// }
}

std::wstring textBox::copy()
{
    return text;
}

void textBox::mouseDown(cveci2 &position, cmb &button)
{
	// check position of cursor

	crectangle2 &relativeTextRect = crectangle2(rectanglei2(rect.size).expanded(-borderSize));
	vec2 offset = currentFont->MeasureStringOffset(relativeTextRect, std::wstring());
	fp closestDistance = INFINITY;
	fsize_t closestIndex = 0;
	for (fsize_t i = 0;; i++, offset = currentFont->MeasureLetterOffset(relativeTextRect, offset, text[i]))
	{
		if (position.y > offset.y && position.y < offset.y + currentFont->fontSize)
		{
			cfp &distance = math::absolute(position.x - offset.x);
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestIndex = i;
			}
		}
		if (i == text.length())
		{
			break;
		}
	}
	if (closestDistance != INFINITY)
	{
		cursorIndex = closestIndex;
	}
	else
	{
		if (position.y < offset.y + currentFont->fontSize)
		{
			cursorIndex = text.length();
		}
		else
		{
			cursorIndex = 0;
		}
	}
}

void textBox::checkCursorIndex()
{
	if (cursorIndex > text.length())
	{
		cursorIndex = text.length();
	}
}

void textBox::focus()
{
	control::focus();
}

bool textBox::wantsTextInput() const
{
	return focused;
}
