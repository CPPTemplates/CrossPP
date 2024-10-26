#include "collisionEdgeData.h"
#include <list>
#include "math/collisions.h"
collisionEdgeData collisionEdgeData::substractCoveringEdges(const collisionEdgeData& edgesCovering) const
{
	//just like the fitexpandinghitbox method
	collisionEdgeData substracted = collisionEdgeData();
	std::list<vec2> edgesToCheck = std::list<vec2>(edges.begin(), edges.end());
	while (edgesToCheck.size())
	{
		vec2 currentEdge = *edgesToCheck.begin();
		edgesToCheck.pop_front();
		cfp currentPos11 = currentEdge.x + currentEdge.y;

		for (cvec2& coveringEdge : edgesCovering.edges)
		{
			cfp coveringPos11 = coveringEdge.x + coveringEdge.y;
			if (collides1d(currentEdge.x, currentEdge.y, coveringEdge.x, coveringEdge.y))
			{
				//cut at both axes
				//-
				if (coveringEdge.x > currentEdge.x)
				{
					edgesToCheck.push_back(cvec2(currentEdge.x, coveringEdge.x - currentEdge.x));
				}
				//+
				if (coveringPos11 < currentPos11)
				{
					edgesToCheck.push_back(cvec2(coveringPos11, currentPos11 - coveringPos11));
				}
				//this rectangle is cut already, the pieces can cut themselves
				goto collidingPosition;
			}
		}
		substracted.addEdge(currentEdge.x, currentEdge.y);
	collidingPosition:;
	}
	return substracted;
}

bool collisionEdgeData::edgeInRange(cvec2& edge)
{
	cfp edgePos1 = edge.x + edge.y;
	for (cvec2& checkEdge : edges)
	{
		if ((checkEdge.x + checkEdge.y) > edge.x)
		{
			if (checkEdge.x > edgePos1)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

void collisionEdgeData::addEdge(fp edgeStart, fp edgeLength)
{
	size_t insertPosition = edges.size();
	for (size_t i = 0; i < edges.size(); i++)
	{
		if ((edgeStart + edgeLength) >= edges[i].x)
		{

			size_t mergeIndex;
			//this is the insert position
			if (edgeStart <= edges[i].x + edges[i].y)
			{
				//merge backwards
				if (edges[i].x < edgeStart)
				{
					edgeLength = (edgeStart + edgeLength) - edges[i].x;
					edgeStart = edges[i].x;
				}
				mergeIndex = i;
			}
			else
			{
				//not touching
				mergeIndex = i + 1;
			}
			//don't increment nextindex, because all merging edges will be removed
			while (mergeIndex < edges.size() && (edgeStart + edgeLength) >= edges[mergeIndex].x)
			{
				//merge forwards
				edgeLength = math::maximum(edgeLength, (edges[mergeIndex].x + edges[mergeIndex].y) - edgeStart);

				edges.erase(edges.begin() + mergeIndex);
			}
			insertPosition = mergeIndex;
			break;
		}
	}
	edges.insert(edges.begin() + insertPosition, cvec2(edgeStart, edgeLength));
}