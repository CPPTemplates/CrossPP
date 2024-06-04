#pragma once
#include "globalFunctions.h"

constexpr int octaveNoteCount = 12;


//returns 1 if the relative note is 0, 2 if it is 12 and so on
fp getNotePitch(cfp& relativeNote);
constexpr fp getHearingRange2D(cfp& visibleScreenRange){
	// you can hear four times as far as you can see (visiblerangex is from left to right corner, hearing range = 2d radius)
    return visibleScreenRange * 2;
}