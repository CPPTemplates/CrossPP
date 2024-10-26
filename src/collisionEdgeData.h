#pragma once
#include <vector>
#include "math/vector/vectn.h"
struct collisionEdgeData
{
	//sorted from start to end, colliding edges merged
	//x = start, y = length
	std::vector<vec2> edges;
	collisionEdgeData(const std::vector<vec2>& edges = std::vector<vec2>()) :edges(edges) {}
	collisionEdgeData substractCoveringEdges(const collisionEdgeData& edgesCovering) const;
	collisionEdgeData substractSingleEdges(const collisionEdgeData& edgesCovering);
	bool edgeInRange(cvec2& edge);

	void addEdge(fp edgeStart, fp edgeLength);
};