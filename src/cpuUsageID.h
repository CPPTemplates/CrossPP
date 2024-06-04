#pragma once
enum class cpuUsageID :int
{
	drawing,
	blockUpdates,
	lightUpdates,
	entities,
	physics,
	miscellaneous,
	chunkLoading,
	networking,
	count
};