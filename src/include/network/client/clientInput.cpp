#include "clientInput.h"
#include "array/arrayFunctions/arrayFunctions.h"

void clientInput::addClientInput(const clientInput &newInput)
{
	mousePositionPixels = newInput.mousePositionPixels;
	scrollDelta += newInput.scrollDelta;
	for (size_t i = 0; i < mb::ButtonCount; i++)
	{
		clicked[i] |= newInput.clicked[i];
		clickReleased[i] |= newInput.clickReleased[i];
		holding[i] = newInput.holding[i];
	}
	for (size_t i = 0; i < newInput.keysDown.size(); i++)
	{
		cvk &key = newInput.keysDown[i];
		if (std::find(keysDown.begin(), keysDown.end(), key) == keysDown.end())
		{
			keysDown.push_back(key);
		}
	}
	for (size_t i = 0; i < newInput.keysUp.size(); i++)
	{
		cvk &key = newInput.keysUp[i];
		if (std::find(keysUp.begin(), keysUp.end(), key) == keysUp.end())
		{
			keysUp.push_back(key);
		}
	}
	keysHolding = newInput.keysHolding;
	textEntered = newInput.textEntered;
}

bool clientInput::holdingDownKey(cvk &key) const
{
	return std::find(keysHolding.begin(), keysHolding.end(), key) != keysHolding.end();
}
bool clientInput::pressedKey(cvk &key) const
{
	return std::find(keysDown.begin(), keysDown.end(), key) != keysDown.end();
}

void clientInput::clearTemporaryData()
{
	fillAllElements(clicked, false);
	fillAllElements(clickReleased, false);
    eventHistory.clear();
	keysDown.clear();
	keysUp.clear();
	scrollDelta = 0;
	textEntered.clear();
}
