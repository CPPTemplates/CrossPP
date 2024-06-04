#pragma once
#include "audio2d.h"
#include <SFML/Audio/Music.hpp>
#include "filesystem/filemanager.h"
class sfmlInputStream;
class alMusic : public sf::Music
{
public:
	using Music::Music;

	uint getSource() const
	{
		return m_source;
	}
};
struct music2d : audio2dt<alMusic>
{
	stdPath path;
	sfmlInputStream *stream;

	inline music2d(const stdPath &path, cvec2 &pos, cfp &volume, cfp &pitch, cbool &isSpatial) : audio2dt(pos, volume, pitch, isSpatial), path(path)
	{
	}

	virtual void loadAudio() override;
	virtual microseconds getDuration() override;
	~music2d();
};