#include "videoSettingsForm.h"
#include "settings.h"
#include "include/application/theme.h"
videoSettingsForm::videoSettingsForm()
{
	addChildren({ graphicsModeButton, transparencyButton, showShadowsButton, biomeColorsButton, fullScreenButton, guiScaleSlider });
}

void videoSettingsForm::layout(crectanglei2& newRect)
{
	form::layout(newRect);
	int currentY = rect.h / 2 + buttonSize.y * 2;
	graphicsModeButton->layout(crectanglei2((rect.w - buttonSize.x) / 2, currentY, buttonSize.x, buttonSize.y));
	currentY -= buttonSize.y;
	transparencyButton->layout(crectanglei2((rect.w - buttonSize.x) / 2, currentY, buttonSize.x, buttonSize.y));
	currentY -= buttonSize.y;
	showShadowsButton->layout(crectanglei2((rect.w - buttonSize.x) / 2, currentY, buttonSize.x, buttonSize.y));
	currentY -= buttonSize.y;
	biomeColorsButton->layout(crectanglei2((rect.w - buttonSize.x) / 2, currentY, buttonSize.x, buttonSize.y));
	currentY -= buttonSize.y;
	fullScreenButton->layout(crectanglei2((rect.w - buttonSize.x) / 2, currentY, buttonSize.x, buttonSize.y));
	currentY -= buttonSize.y;
	guiScaleSlider->layout(crectanglei2((rect.w - buttonSize.x) / 2, currentY, buttonSize.x, buttonSize.y));
}

void videoSettingsForm::mouseDown(cveci2& position, cmb& button)
{
	form::mouseDown(position, button);
	control* highestChild = getHighestChild(position);
	if (highestChild == graphicsModeButton)
	{
		settings::videoSettings::currentGraphicsMode = (graphicsMode)(((int)settings::videoSettings::currentGraphicsMode + 1) % (int)graphicsMode::count);
	}
	//else if (highestChild == fullScreenButton) TODO: implement fullscreen in another way
	//{
	//	currentApplication->switchFullScreen();
	//}
	else if (highestChild == showShadowsButton)
	{
		settings::videoSettings::showShadows = !settings::videoSettings::showShadows;
	}
	else if (highestChild == transparencyButton)
	{
		settings::videoSettings::renderTransparency = !settings::videoSettings::renderTransparency;
	}
	else if (highestChild == biomeColorsButton)
	{
		settings::videoSettings::multiplyBiomeColors = !settings::videoSettings::multiplyBiomeColors;
	}
	else if (highestChild == guiScaleSlider)
	{
		settings::videoSettings::guiScale = guiScaleSlider->value;
	}
}

void videoSettingsForm::render(cveci2& position, const texture& renderTarget)
{
	graphicsModeButton->text = std::wstring(L"Graphics: ") + (settings::videoSettings::currentGraphicsMode == graphicsMode::fast ? std::wstring(L"Fast") : std::wstring(L"Fancy"));
	transparencyButton->text = std::wstring(L"Transparency: ") + (settings::videoSettings::renderTransparency ? std::wstring(L"On") : std::wstring(L"Off"));
	showShadowsButton->text = std::wstring(L"Shadows: ") + (settings::videoSettings::showShadows ? std::wstring(L"On") : std::wstring(L"Off"));
	biomeColorsButton->text = std::wstring(L"Biome Colors: ") + (settings::videoSettings::multiplyBiomeColors ? std::wstring(L"On") : std::wstring(L"Off"));
	//fullScreenButton->text = std::wstring(L"Fullscreen: ") + (currentApplication->isFullScreen ? std::wstring(L"On") : std::wstring(L"Off"));
	renderChildren(position, renderTarget);
}