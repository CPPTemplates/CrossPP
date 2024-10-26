#pragma once
#include "itemID.h"
#include "blockData.h"
struct blockArrayContainer {
	array2d<blockID> blockIDArray = array2d<blockID>();
	array2d<blockData*> blockDataArray = array2d<blockData*>();
	array2d<powerLevel> powerLevelArray = array2d<powerLevel>();
	blockArrayContainer(cvect2<fsize_t>& size) :
		blockIDArray(array2d<blockID>(size, true)),
		blockDataArray(array2d<blockData*>(size, true)),
		powerLevelArray(array2d<powerLevel>(size, true))
	{}
};