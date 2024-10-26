#include "minecraftFont.h"
#include "math/graphics/brush/brushes/colorMultiplier.h"
#include "include/optimization/handleError.h"

vec2 minecraftFont::DrawString(const std::wstring& text, crectangle2 &rect, cvec2 &offset, const texture &renderTarget, const std::optional<mat3x3> &matrix) const
{
    color currentTextColor = colorPalette::white;
    bool obfuscated = false;
    int obfuscateLength = 0;

    const std::wstring obfuscateLetters = std::wstring(L"!@#$%^&*()}{[]\\/.,;'`~");
    const auto &currentTextBrush = brushes::white;

    vec2 currentOffset = offset;

    // color ranges
    for (size_t i = 0; i < text.size(); i++)
    {
        letter currentLetter = text[i];

        // const auto& currentTextBrush = colorMultiplier<texture, solidColorBrush>(*creditsFont.family->tex, brushes::white);

        if (currentLetter == colorCodeChar)
        {
            if (obfuscated)
            {
                obfuscated = false;
                if (obfuscateLength == 0) // pick a random length
                {
                    obfuscateLength = rand(currentRandom, 0x8, 0xc);
                }

                // creditsFont.fontBrush = &currentTextBrush;
                for (int i = 0; i < obfuscateLength; i++)
                {
                    currentLetter = obfuscateLetters[randIndex(currentRandom, obfuscateLetters.size())];
                    if (collides2d(rect, crectangle2(currentOffset, cvec2(fontSize))))
                    {
                        // mat3x3 t = mat3x3::cross(transform, mat3x3::fromRectToRect(crectangle2()));
                        if (currentTextColor == colorPalette::white)
                        {
                            family.DrawLetter(currentLetter, currentOffset, fontSize, renderTarget, *family.tex, matrix);
                        }
                        else
                        {
                            family.DrawLetter(currentLetter, currentOffset, fontSize, renderTarget, colorMultiplier(*family.tex, currentTextBrush), matrix);
                        }
                    }
                    currentOffset = MeasureLetterOffset(rect, currentOffset, currentLetter);
                }
            }

            letter formatCodeLetter = text[i + 1];
            int colorCode;
            if (convertToInt(std::wstring(1, formatCodeLetter), colorCode, 0x10))
            {
                currentTextColor = rgbFormatColors[colorCode];
            }
            else if (formatCodeLetter == L'k')
            {
                obfuscated = true;
                obfuscateLength = 0;
            }
            else if (formatCodeLetter == L'r')
            {
                // reset
                currentTextColor = colorPalette::white;
            }
            else
            {
                handleError(std::wstring(L"invalid sign behind \"") + colorCodeChar + L"\"");
            }
            i++;
        }
        else
        {
            if (obfuscated)
            {
                obfuscateLength++;
            }
            else if (baseFont::drawable(currentLetter))
            {
                if (currentTextColor == colorPalette::white)
                {
                    family.DrawLetter(currentLetter, currentOffset, fontSize, renderTarget, *family.tex, matrix);
                }
                else
                {
                    family.DrawLetter(currentLetter, currentOffset, fontSize, renderTarget, colorMultiplier<resolutionTexture, solidColorBrush>(*family.tex, currentTextBrush), matrix);
                }
            }
            currentOffset = MeasureLetterOffset(rect, currentOffset, currentLetter);
        }
    }
    return currentOffset;
}
