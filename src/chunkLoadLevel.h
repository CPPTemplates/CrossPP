#pragma once
enum class chunkLoadLevel : int
{
	notLoaded,//used to get data from chunks without loading them completely
	worldGenerationLoaded,//this chunk is loaded for world generation
	updateLoaded,//this chunk has blockupdates or is at the border of chunks with blockupdates; the structures are generated
	entityLoaded//this chunk is within the 'load range' of the player.
};