#pragma once
#include "audioCollection.h"
#include "sound/music2d.h"
#include "filesystem/filemanager.h"
struct tickableBlockContainer;
struct musicCollection : audioCollection
{
	std::vector<stdPath> audioPaths = std::vector<stdPath>();
	void addAudioFile(const stdPath& path) override;
	musicCollection(const stdPath& path);
	musicCollection() {}
	std::shared_ptr<music2d> playRandomAudio(cfp& volume = 1, cfp& pitch = 1) const;
	std::shared_ptr<music2d> playAudio(csize_t& index, cfp& volume = 1, cfp& pitch = 1) const;
	std::shared_ptr<music2d> playAudio(csize_t& index, tickableBlockContainer* containerIn, cvec2& position, cfp& volume = 1, cfp& pitch = 1) const;
	std::shared_ptr<music2d> playRandomAudio(tickableBlockContainer* containerIn, cvec2& position, cfp& volume = 1, cfp& pitch = 1) const;
	void addMusic(std::shared_ptr<musicCollection> music);
};