#include "creditsForm.h"
#include "gameForm.h"
#include "resourcePack.h"
#include "colors.h"
#include "keyID.h"
#include "gameControl.h"
#include "math/collisions.h"
#include "minecraftFont.h"
creditsForm::creditsForm()
{

}

void creditsForm::render(cveci2& position, const texture& renderTarget)
{
	renderOptionsBackGround(crectanglei2(position, rect.size), renderTarget);

	//font<> creditsFont = font<>(*currentFont, &brushes::black);
	//creditsFont.fontSize = 0x18;
	//
	//crectangle2 renderRect = crectangle2(cvec2(position), cvec2(rect.size));
	//crectangle2 textRect = crectangle2(renderRect.expanded(-0x20));
	////begin in the bottom left corner, so the text comes up
	//vec2 offset = textRect.pos0;
	////4 seconds each line, spaced out to 8 seconds each line
	//offset.y += microsectosec(getmicroseconds() - timeStartedReading) * creditsFont.fontSize * 0.25;
	//
	//color currentTextColor = colorPalette::white;
	//const auto& shadowBrush = colorMultiplier<resolutionTexture, solidColorBrush>(*creditsFont.family.tex, brushes::black);
	//bool obfuscated = false;
	//int obfuscateLength = 0;
	//
	//const std::wstring obfuscateLetters = std::wstring(L"!@#$%^&*()}{[]\\/.,;'`~");
	//
	////color ranges
	//for (int i = 0; i < creditsText.size(); i++)
	//{
	//	letter currentLetter = creditsText[i];
	//	const auto& currentTextBrush = colorMultiplier<texture, solidColorBrush>(*creditsFont.family->tex, brushes::white);
	//
	//	if (currentLetter == L'�')
	//	{
	//		if (obfuscated)
	//		{
	//			obfuscated = false;
	//			if (obfuscateLength == 0)//pick a random length
	//			{
	//				obfuscateLength = rand(currentRandom, 0x8, 0xc);
	//			}
	//
	//			creditsFont.fontBrush = &currentTextBrush;
	//			for (int i = 0; i < obfuscateLength; i++)
	//			{
	//				currentLetter = obfuscateLetters[randIndex(currentRandom, obfuscateLetters.size())];
	//				if (collides2d(renderRect, crectangle2(offset, cvec2(creditsFont.fontSize))))
	//				{
	//					if (currentTextColor == colorPalette::white)
	//					{
	//						creditsFont.family->DrawLetter(currentLetter, offset, creditsFont.fontSize, renderTarget);
	//					}
	//					else
	//					{
	//
	//						creditsFont.family->DrawLetter(currentLetter, offset - 0x2, creditsFont.fontSize, renderTarget, shadowBrush);
	//						creditsFont.family->DrawLetter(currentLetter, offset, creditsFont.fontSize, renderTarget, currentTextBrush);
	//					}
	//				}
	//				offset = creditsFont.MeasureLetterOffset(textRect, offset, currentLetter);
	//			}
	//		}
	//
	//		letter formatCodeLetter = creditsText[i + 1];
	//		int colorCode;
	//		if (convertToInt(std::wstring(1, formatCodeLetter), colorCode, 0x10))
	//		{
	//			currentTextColor = rgbFormatColors[colorCode];
	//		}
	//		else if (formatCodeLetter == L'k')
	//		{
	//			obfuscated = true;
	//			obfuscateLength = 0;
	//		}
	//		else if (formatCodeLetter == L'r')
	//		{
	//			//reset
	//			currentTextColor = colorPalette::white;
	//		}
	//		else
	//		{
	//			handleError(std::wstring(L"invalid sign behind \"�\""));
	//		}
	//		i++;
	//	}
	//	else
	//	{
	//		if (obfuscated)
	//		{
	//			obfuscateLength++;
	//		}
	//		else if (font::drawable(currentLetter))
	//		{
	//			if (collides2d(renderRect, crectangle2(offset, cvec2(creditsFont.fontSize))))
	//			{
	//				if (currentTextColor == colorPalette::white)
	//				{
	//					creditsFont.family->DrawLetter(currentLetter, offset, creditsFont.fontSize, renderTarget);
	//				}
	//				else
	//				{
	//
	//					creditsFont.family->DrawLetter(currentLetter, offset - 0x2, creditsFont.fontSize, renderTarget, shadowBrush);
	//					creditsFont.family->DrawLetter(currentLetter, offset, creditsFont.fontSize, renderTarget, currentTextBrush);
	//				}
	//			}
	//		}
	//		offset = creditsFont.MeasureLetterOffset(textRect, offset, currentLetter);
	//	}
	//}
}

void creditsForm::keyDown(cvk& keycode)
{
	if (keycode == (vk)keyID::escape)
	{
		parent->switchVisibleChild(nullptr);
	}
}