#include "globalFunctions.h"
#pragma once
enum class nbtDataTag :sbyte
{
	tagEnd,
	tagSignedInt8,
	tagSignedInt16,
	tagSignedInt32,
	tagSignedInt64,
	tagFP32,
	tagFP64,
	tagSignedInt8Array,
	tagUTF8String,
	tagList,//childrenCount: signedint32 then nbtTag[childrencount] 
	tagCompound,//nbtTag[childrenCount] then tagEnd
	tagSignedInt32Array,
	tagSignedInt64Array,
	//not a miscellaneous tag or extra tags, to be compatible with minecraft
};