#include "music2d.h"
#include "filesystem/sfmlInputStream.h"
#include "filesystem/fileio.h"
void music2d::loadAudio() {
    playingAudio = new alMusic();
    // this way, even on android we can read from an actual file instead of the internal APK storage
    stream = new sfmlInputStream(std::make_shared<std::ifstream>(path, getOpenMode(false)));
    if (!playingAudio->openFromStream(*stream)) {
        throw "file not found";
    }
}

microseconds music2d::getDuration() {
    if (!audioLoaded()) {
        loadAudio();
    }
    return playingAudio->getDuration().asMicroseconds();
}

music2d::~music2d() {
    //the audio has to be deleted first, as it depends on the stream
    delete playingAudio;
    playingAudio = nullptr;
    delete stream;
}