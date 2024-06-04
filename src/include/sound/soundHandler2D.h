// #include "soundHandler.h"
#include "math/timemath.h"
#include "math/vector/vectn.h"
#include "array/fastlist.h"
#include "sound/audio2d.h"

#pragma once

struct soundHandler2d : IDestructable
{
	fastList<std::shared_ptr<audio2d>> currentlyPlayIngAudio = fastList<std::shared_ptr<audio2d>>();
	int playingSoundCount = 0;
	fp hearingRange = 0;
	fp globalVolume = 1;
	vec3 earPosition = vec3();
	vec3 earSpeed = vec3();
	// earPosition.z = the distance of the player to the screen in real life converted to 'ingame distance'
	void update();
	soundHandler2d();

	void playAudio(const std::shared_ptr<audio2d> &audioToPlay);
	void stopAudio(const std::shared_ptr<audio2d> &audioToStop);
	void visualize(const struct texture& renderTarget);
	~soundHandler2d();
};

extern soundHandler2d handler;