#pragma once
#include "arraynd.h"

constexpr int fastestOctreenodeSize = 4;

template<typename t, fsize_t axisCount>
struct octreeNode
{
	t uniformValue = t();

	static constexpr vectn<size_t, axisCount> size = vectn<size_t, axisCount>(fastestOctreenodeSize);

	static constexpr size_t childNodeCount = size.volume();

	//octrees or quad trees
	//if they are nullptr, skip them
	octreeNode* childNodes[childNodeCount]{};

	constexpr bool inBounds(cvecin<axisCount>& position) const
	{
		return position.indexInBounds(size);
	}

	constexpr bool hasChildNodes() const
	{
		return childNodes[0];
	}

	constexpr octreeNode()
	{

	}

	inline constexpr void deleteRecursive()
	{
		if (hasChildNodes())
		{
			for (auto& it : childNodes)
			{
				it->deleteRecursive();
				delete it;
				it = nullptr;
			}
		}
	}

	//recursiondepth = 1-> no child nodes
	constexpr octreeNode(cfsize_t& recursionDepth, cvecin<axisCount>& position, const arraynd<t, axisCount>& values)
	{
		if (recursionDepth > 1)
		{
			cfsize_t& scale = math::powSizeTSimple(fastestOctreenodeSize, recursionDepth - 1);

			const auto itRect = crectangletn<size_t, axisCount>(cvectn<size_t, axisCount>(fastestOctreenodeSize));

			for (auto nodeIt : std::views::zip(childNodes, itRect))
			{
				nodeIt.val<0>() = new octreeNode(recursionDepth - 1, position + (cvecin<axisCount>)(nodeIt.val<1>() * scale), values);
			}

			uniformValue = childNodes[0]->uniformValue;

			bool keepChildNodes = false;

			for (size_t i = 0; i < childNodeCount; i++)
			{
				if (childNodes[i]->hasChildNodes())
				{
					keepChildNodes = true;
					break;
				}
				if (i)
				{
					if (childNodes[i]->uniformValue != uniformValue)
					{
						keepChildNodes = true;
						break;
					}
				}
			}
			if (!keepChildNodes)
			{
				for (auto& it : childNodes)
				{
					delete it;
					it = nullptr;
				}
			}
		}
		else
		{
			uniformValue = values.getValue(position);
		}
	}
};

template<typename t>
using octreeNode3 = octreeNode<t, 3>;