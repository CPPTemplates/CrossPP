#pragma once
#include "array/fastlist.h"
#include "interface/idestructable.h"
#include "blockContainer.h"
struct structure :IDestructable, blockContainer
{
	array2d<blockID> blockIDArray = array2d<blockID>();
	array2d<blockData*> blockDataArray = array2d<blockData*>();
	fastList<entity*> entities = fastList<entity*>();
    //for example: c:/path/to/structure (without the .dat)
	stdPath pathWithoutExtension = stdPath();

	mat3x3i getBlocksToWorldTransform(cveci2& pos00, cbool& flipX) const;
	mat3x3 getEntitiesToWorldTransform(cveci2& pos00, cbool& flipX) const;
	structure(const stdPath pathWithoutExtension = stdPath()) :blockIDArray(array2d<blockID>()), blockDataArray(array2d<blockData*>()), entities(fastList<entity*>()), pathWithoutExtension(pathWithoutExtension) {}
	bool serialize(const stdPath& path, cbool& write);
	void createStructure(dimension& dimensionIn, crectanglei2& rect, cbool& includeEntities);

	std::vector<veci2> findBlocks(const blockID& id);
	virtual ~structure() override;

	virtual void* getArrayValuePointerUnsafe(cveci2& position, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel) override;
	virtual bool inBounds(cveci2& position) const override;
};

structure* getStructureByPath(const stdPath& path);
std::vector<structure*> getStructuresByName(const stdPath& seekFolder, const std::wstring& name);
extern std::vector<structure*> structureList;