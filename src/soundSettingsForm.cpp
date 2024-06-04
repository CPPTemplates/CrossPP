#include "soundSettingsForm.h"
#include "settings.h"
#include "keyID.h"
#include "gameControl.h"
soundSettingsForm::soundSettingsForm()
{
	addChildren({ volumeLabel, volumeSlider, headScreenDistanceLabel, headScreenDistanceSlider, musicButton });
}

void soundSettingsForm::layout(crectanglei2& newRect)
{
	form::layout(newRect);
	volumeLabel->layout(crectanglei2((rect.w - buttonSize.x) / 2, rect.h / 2, buttonSize.x, buttonSize.y));
	volumeSlider->layout(crectanglei2((rect.w - buttonSize.x) / 2, rect.h / 2 - buttonSize.y, buttonSize.x, buttonSize.y));
	headScreenDistanceLabel->layout(crectanglei2((rect.w - buttonSize.x) / 2, rect.h / 2 - buttonSize.y * 2, buttonSize.x, buttonSize.y));
	headScreenDistanceSlider->layout(crectanglei2((rect.w - buttonSize.x) / 2, rect.h / 2 - buttonSize.y * 3, buttonSize.x, buttonSize.y));
	musicButton->layout(crectanglei2((rect.w - buttonSize.x) / 2, rect.h / 2 - buttonSize.y * 4, buttonSize.x, buttonSize.y));
}

void soundSettingsForm::render(cveci2& position, const texture& renderTarget)
{
	volumeLabel->text = std::wstring(L"Volume: ") + std::to_wstring(settings::soundSettings::volume * 100);
	headScreenDistanceLabel->text = std::wstring(L"distance of your ears to the screen divided by the width of your screen: ") + std::to_wstring(settings::soundSettings::headScreenDistance * 100);
	musicButton->text = std::wstring(L"Music: ") + (settings::soundSettings::playMusic ? std::wstring(L"On") : std::wstring(L"Off"));
	renderChildren(position, renderTarget);
}

void soundSettingsForm::mouseDown(cveci2& position, cmb& button)
{
	form::mouseDown(position, button);
	control* highestChild = getHighestChild(position);
	if (highestChild == musicButton)
	{
		settings::soundSettings::playMusic = !settings::soundSettings::playMusic;
	}
	else if (highestChild == volumeSlider)
	{
		settings::soundSettings::volume = volumeSlider->value;
	}
	else if (highestChild == headScreenDistanceSlider)
	{
		settings::soundSettings::headScreenDistance = headScreenDistanceSlider->value;
	}
}

void soundSettingsForm::keyDown(cvk& keycode)
{
	if (keycode == (vk)keyID::escape)
	{
		parent->switchVisibleChild(((gameControl*)parent)->options);
	}
}
