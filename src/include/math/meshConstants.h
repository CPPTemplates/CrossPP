#pragma once
#include "globalFunctions.h"
constexpr int rectangle3sidecount = 6;
constexpr int planevertcount = 4;
constexpr int vertsize = 3 + 2 + 3;//8 fps for 1 vert
const uint cubeindices[rectangle3sidecount * 2 * 3] =
{
	// bottom, z0, 0123, opposite
	0, 2, 1,
	3, 1, 2,
	// top, z1, 4567
	4, 5, 6,
	7, 6, 5,
	// back, y0, 0145, opposite
	0, 1, 4,
	5, 4, 1,
	// forward, y1, 2367
	2, 6, 3,
	7, 3, 6,
	// left, x0, 0246, opposite
	0, 4, 2,
	6, 2, 4,
	// right, x1, 1357
	1, 3, 5,
	7, 5, 3,
};
//x y z
cfp cubeverts[rectangle3sidecount * planevertcount * vertsize] =
{
	0,0,0,//bottom
	1,0,0,
	0,1,0,
	1,1,0,
	0,0,1,//top
	1,0,1,
	0,1,1,
	1,1,1,
};
//const bufferobject<fp>* cubelight = mesh::CalculateLightLevels(cubevb0, cubeibo);
constexpr fp planeverts[4 * 8] =
{
	0,0,0,0,0,1,1,1,
	1,0,0,1,0,1,1,1,
	0,1,0,0,1,1,1,1,
	1,1,0,1,1,1,1,1,
};
