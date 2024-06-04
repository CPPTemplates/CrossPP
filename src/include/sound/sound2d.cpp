#include "sound2d.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <AL/al.h>
void sound2d::loadAudio() {
    playingAudio = new alSound(*buffer);
}

microseconds sound2d::getDuration() {
    if (!buffer) {
        throw "buffer not found";
    }
    return buffer->getDuration().asMicroseconds();
}