#pragma once
#include "audio2d.h"
#include <SFML/Audio/Sound.hpp>
class alSound : public sf::Sound
{
public:
	using Sound::Sound;
	uint getSource() const
	{
		return m_source;
	}
};
struct sound2d : audio2dt<alSound>
{
	std::shared_ptr<sf::SoundBuffer> buffer;

	inline sound2d(const std::shared_ptr<sf::SoundBuffer> &buffer, cvec2 &pos, cfp &volume, cfp &pitch, cbool &isSpatial)
		: audio2dt(pos, volume, pitch, isSpatial), buffer(buffer)
	{
	}
	virtual void loadAudio() override;
	virtual microseconds getDuration() override;
};
