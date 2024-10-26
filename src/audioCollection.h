#pragma once
#include "interface/idestructable.h"
#include <string>
#include "filesystem/filemanager.h"
struct audioCollection :IDestructable
{
	std::wstring key;
	void addAudioFileName(const stdPath& path);
	virtual void addAudioFile(const stdPath& path);

};