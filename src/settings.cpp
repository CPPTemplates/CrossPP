#include "optimization/debug.h"
#include "graphicsMode.h"
#include "settings.h"

graphicsMode settings::videoSettings::currentGraphicsMode = isDebugging ? graphicsMode::fast : graphicsMode::fancy;
bool settings::videoSettings::renderTransparency = !isDebugging;
bool settings::videoSettings::showShadows = !isDebugging;
bool settings::videoSettings::multiplyBiomeColors = !isDebugging;
fp settings::videoSettings::maximalFps = 60.0;
fp settings::videoSettings::guiScale = 1;

bool settings::soundSettings::playMusic = true;//!isDebugging;
fp settings::soundSettings::headScreenDistance = 1.5;//the head of the player is by default 1.5 times the size of the screen away
fp settings::soundSettings::volume = 1;

bool settings::renderDebugData = false;
bool settings::renderHUD = true;
bool settings::renderHitboxes = false;