#include "meshfunctions.h"

uint* getPlaneIndices(cint& planeCount)
{
	uint* indices = new uint[planeCount * 6];
	uint* indptr = indices;
	int vertcount = planeCount * 4;
	for (int i = 0; i < vertcount; i += 4)
	{
		*indptr++ = i;
		*indptr++ = i + 1;
		*indptr++ = i + 2;
		*indptr++ = i + 3;
		*indptr++ = i + 2;
		*indptr++ = i + 1;
	}
	return indices;
}
