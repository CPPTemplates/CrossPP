#pragma once
#include "octreenode.h"
#include "raycastIterator.h"
template<typename t, fsize_t axisCount>
struct treeNodeIterator
{
	typedef octreeNode<t, axisCount> octreeNodeType;

	const octreeNodeType& currentNode;

	rayCastIterator<axisCount> it = rayCastIterator<axisCount>();

	treeNodeIterator* childIt = nullptr;

	vecn<axisCount> directionNormal = vecn<axisCount>();

	vecn<axisCount> startPosition = vecn<axisCount>();

	treeNodeIterator(const octreeNodeType& currentNode, cvecn<axisCount>& position, cvecn<axisCount>& directionNormal) :
		currentNode(currentNode), it(position, directionNormal), directionNormal(directionNormal), startPosition(position)
	{
		if (currentNode.inBounds(it.currentPosition))
		{
			if (currentNode.hasChildNodes())
			{
				vecn<axisCount> relativeChildPosition;
				for (auto it : std::views::zip(relativeChildPosition, position))
				{
					std::get<2>(it) = math::Remainder1(std::get<2>(it)) * fastestOctreenodeSize;
				}

				childIt = new treeNodeIterator(*currentNode.childNodes[singleDimensionalIndex(it.currentPosition, currentNode.size)], relativeChildPosition, directionNormal);
			}
		}
	}

	iteratorInline fp getT() const
	{
		cfp& ownT = it.getT();
		if (childIt)
		{
			return ownT + (childIt->getT() * 0.5);
		}
		else
		{
			return ownT;
		}
	}

	iteratorInline vecn<axisCount> getCurrentPosition() const
	{
		vecn<axisCount> ownPos = it.currentPosition;

		if (childIt)
		{
			return ownPos + (childIt->getCurrentPosition() * 0.5);
		}
		else 
		{
			return ownPos;
		}
	}

	iteratorInline t getCurrentValue() const
	{
		return childIt ? childIt->getCurrentValue() : currentNode.uniformValue;
	}

	// ++prefix operator
	//iteratorInline 
		void operator++()
	{
		if (childIt)
		{
			++(*childIt);
			if ((!childIt->childIt) && !childIt->currentNode.inBounds(childIt->it.currentPosition))
			{
				delete childIt;
				childIt = nullptr;
			}
		}

		if (!childIt)
		{
			++it;

			if (currentNode.inBounds(it.currentPosition) && currentNode.hasChildNodes())
			{
				const auto& currentChildNode = *currentNode.childNodes[singleDimensionalIndex(it.currentPosition, currentNode.size)];
				if (currentChildNode.hasChildNodes())
				{
					fp lastAxisCrossingT = it.nextAxisCrossingT[it.lastChangedAxis] - it.delta[it.lastChangedAxis];

					vecn<axisCount> relativeChildPosition;

					for (size_t i = 0; i < axisCount; i++)
					{
						if (directionNormal[i] == 0)
						{
							relativeChildPosition[i] = math::floor(startPosition[i], (fp)fastestOctreenodeSize);
						}
						else 
						{
							relativeChildPosition[i] = (i == it.lastChangedAxis) ?
								fastestOctreenodeSize :
								((it.nextAxisCrossingT[i] - lastAxisCrossingT) * directionNormal[i]) * fastestOctreenodeSize;

							//invert axis
							if (it.sign[i] == 1)
							{
								relativeChildPosition[i] = fastestOctreenodeSize - relativeChildPosition[i];
							}
						}
					}

					childIt = new treeNodeIterator(currentChildNode, relativeChildPosition, directionNormal);
				}
			}
		}
	}
	~treeNodeIterator()
	{
		if (childIt)
		{
			delete childIt;
		}
	}
};