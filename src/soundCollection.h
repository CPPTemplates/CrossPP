#pragma once
#include "audioCollection.h"
#include <SFML/Audio/Sound.hpp>
#include <memory>
#include "sound/audio2d.h"
#include "sound/sound2d.h"
#include <unordered_map>
struct tickableBlockContainer;

struct soundCollection : audioCollection
{
	std::vector<std::shared_ptr<sf::SoundBuffer>> audioToChooseFrom = std::vector<std::shared_ptr<sf::SoundBuffer>>();

	soundCollection();
	soundCollection(const stdPath& path);
	void addSoundCollection(const soundCollection& collection);
	void addAudioFile(const stdPath& path) override;
	std::shared_ptr<audio2d> playRandomSound(tickableBlockContainer* containerIn, cvec2& position, cfp& volume = 1, cfp& pitch = 1);
	std::shared_ptr<audio2d> playRandomSound(cfp& volume = 1, cfp& pitch = 1);
	std::shared_ptr<sound2d> playSound(csize_t& index, tickableBlockContainer* containerIn, cvec2& position, cfp& volume = 1, cfp& pitch = 1);
	std::shared_ptr<sound2d> playSound(csize_t& index, cfp& volume = 1, cfp& pitch = 1);
};
extern std::unordered_map<std::wstring, audioCollection*> globalSoundCollectionList;