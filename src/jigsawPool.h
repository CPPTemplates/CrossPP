#pragma once
#include "interface/inamable.h"
#include <vector>
#include "structure.h"
#include "filesystem/filemanager.h"
struct jigsawPool
{
	std::vector<structure*> targetedStructures;
	std::vector<fp> structureWeights;
	stdPath path;
	/// @brief 
	/// @param path for example dungeon/lava_moat
	/// @return 
	static jigsawPool* create(const stdPath& path);
	jigsawPool(const stdPath& path);
	void addTargetStructure(const stdPath& seekFolder, const std::wstring& structureName, cfp& weight);
	structure* getRandomStructure(std::mt19937& randomToUse);
};

void reloadJigsawPools();
jigsawPool* getJigsawPoolByPath(const stdPath& path);
extern std::vector<jigsawPool*> jigsawPoolList;