#pragma once
#include "musicCollection.h"
#include <memory>
void updateMusic(const musicCollection* const& musicIfFinished);
void stopMusic();

void replaceMusic(const musicCollection* const& newMusic);
void playMusic(const musicCollection* const& newMusic);

extern std::shared_ptr<music2d> currentlyPlayingMusic;
extern const musicCollection* currentlyPlayingCollection;