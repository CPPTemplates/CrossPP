#pragma once
#include "globalFunctions.h"
#include "graphicsMode.h"
namespace settings
{
	namespace videoSettings
	{
		extern graphicsMode currentGraphicsMode;
		extern bool renderTransparency;
		extern bool showShadows;
		extern bool multiplyBiomeColors;
		extern fp maximalFps;
		extern fp guiScale;
	}
	namespace soundSettings
	{
		extern bool playMusic;
		extern fp headScreenDistance;//the head of the player is by default 1.5 times the size of the screen away
		extern fp volume;
	}
	extern bool renderDebugData;
	extern bool renderHUD;
	extern bool renderHitboxes;
}