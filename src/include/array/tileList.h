#include "tile.h"
#pragma once

template<typename t, typename vectn>
struct tileList:IDestructable
{
	typedef tile<t, vectn> listTile;
	//the tiles are sorted
	fastList<listTile*>* tiles;

	//override
	listTile* (*addTile)(vectn position, fastList<t>* elements);

	//WATCH OUT: shouldswap cant convert all listTiles into your derived ones!
	tileList(bool (*shouldSwap)(listTile* const& first, listTile* const& last), listTile* (*addTile)(vectn position, fastList<t>* elements))
	{
		this->tiles = new fastList<listTile*>();
		this->tiles->shouldSwap = shouldSwap;
		this->addTile = addTile;
	}
	//returns the tile this element is added to
	inline tile<t, vectn>* addElement(t value, vectn position, bool update = true)
	{
		//use a temporary tile to find out if there is already a tile or not
		tile<t, vectn>* currentTile = new listTile(position, nullptr);
		int index = tiles->findSortedUpperNeighbor(currentTile);

		delete currentTile;
		tile<t, vectn>* closestTile;
		if (index >= 0)
		{
			closestTile = (*tiles)[index];
			if (closestTile->position == position)
			{
				goto addToTile;
			}
		}
		//add a tile
		currentTile = addTile(position, nullptr);
		tiles->insertAndUpdate(currentTile, index + 1);
		currentTile->elements = new fastList<t>();
		closestTile = currentTile;
	addToTile:
		closestTile->elements->push_back(value);
		if (update) { 
			closestTile->elements->update(); 
		}
		return closestTile;
	}
	inline virtual ~tileList() override
	{
		for (listTile* currentTile : *tiles) 
		{
			delete currentTile;
		}
		delete tiles;
	}
};
