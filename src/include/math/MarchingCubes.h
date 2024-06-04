#pragma once

#include "triangulationtable.h"
//http://paulbourke.net/geometry/polygonise/
typedef struct {
    vec3 p[3];
} triangle3;

typedef struct {
    vec3 p[8];
    fp val[8];
} gridCell;

/*mpVector LinearInterp(mp4Vector p1, mp4Vector p2, float value)
{
    if (p2 < p1)
    {
        mp4Vector temp;
        temp = p1;
        p1 = p2;
        p2 = temp;
    }

    mpVector p;
    if (fabs(p1.val - p2.val) > 0.00001)
        p = (mpVector)p1 + ((mpVector)p2 - (mpVector)p1) / (p2.val - p1.val) * (value - p1.val);
    else
        p = (mpVector)p1;
    return p;
}

bool leftlessthanright(const mp4Vector& left, const mp4Vector& right) const
{
    if (x < right.x)
        return true;
    else if (x > right.x)
        return false;

    if (y < right.y)
        return true;
    else if (y > right.y)
        return false;

    if (z < right.z)
        return true;
    else if (z > right.z)
        return false;

    return false;
}*/

/*
   Linearly interpolate the position where an isosurface cuts
   an edge between two vertices, each with their own scalar value
*/
inline vec3 VertexInterp(fp isolevel, vec3 p1, vec3 p2, fp valp1, fp valp2)
{
    fp mu;
    vec3 p;

    if (abs(isolevel - valp1) < 0.00001)
        return(p1);
    if (abs(isolevel - valp2) < 0.00001)
        return(p2);
    if (abs(valp1 - valp2) < 0.00001)
        return(p1);
    mu = (isolevel - valp1) / (valp2 - valp1);
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return(p);
}

/*
   Given a grid cell and an isolevel, calculate the triangular
   facets required to represent the isosurface through the cell.
   Return the number of triangular facets, the array "triangles"
   will be loaded up with the vertices at most 5 triangular facets.
    0 will be returned if the grid cell is either totally above
   of totally below the isolevel.
*/

const veci3 indexConvention[0x8]
{ 
    veci3(0,0,0),
    veci3(1,0,0),
    veci3(1,1,0),
    veci3(0,1,0),
    veci3(0,0,1),
    veci3(1,0,1),
    veci3(1,1,1),
    veci3(0,1,1),
};
inline int Polygonise(gridCell grid, fp isolevel, triangle3* triangles)
{
    int i;
    int cubeindex;
    vec3 vertlist[12];    

    /*
       Determine the index into the edge table which
       tells us which vertices are inside of the surface
    */
    cubeindex = 0;
    if (grid.val[0] < isolevel) cubeindex |= 0b1;
    if (grid.val[1] < isolevel) cubeindex |= 0b10;
    if (grid.val[2] < isolevel) cubeindex |= 0b100;
    if (grid.val[3] < isolevel) cubeindex |= 0b1000;
    if (grid.val[4] < isolevel) cubeindex |= 0b10000;
    if (grid.val[5] < isolevel) cubeindex |= 0b100000;
    if (grid.val[6] < isolevel) cubeindex |= 0b1000000;
    if (grid.val[7] < isolevel) cubeindex |= 0b10000000;

    /* Cube is entirely in/out of the surface */
    if (edgeTable[cubeindex] == 0)
        return(0);

    /* Find the vertices where the surface intersects the cube */
    if (edgeTable[cubeindex] & 0b1)
        vertlist[0] =
        VertexInterp(isolevel, grid.p[0], grid.p[1], grid.val[0], grid.val[1]);
    if (edgeTable[cubeindex] & 0b10)
        vertlist[1] =
        VertexInterp(isolevel, grid.p[1], grid.p[2], grid.val[1], grid.val[2]);
    if (edgeTable[cubeindex] & 0b100)
        vertlist[2] =
        VertexInterp(isolevel, grid.p[2], grid.p[3], grid.val[2], grid.val[3]);
    if (edgeTable[cubeindex] & 0b1000)
        vertlist[3] =
        VertexInterp(isolevel, grid.p[3], grid.p[0], grid.val[3], grid.val[0]);
    if (edgeTable[cubeindex] & 0b10000)
        vertlist[4] =
        VertexInterp(isolevel, grid.p[4], grid.p[5], grid.val[4], grid.val[5]);
    if (edgeTable[cubeindex] & 0b100000)
        vertlist[5] =
        VertexInterp(isolevel, grid.p[5], grid.p[6], grid.val[5], grid.val[6]);
    if (edgeTable[cubeindex] & 0b1000000)
        vertlist[6] =
        VertexInterp(isolevel, grid.p[6], grid.p[7], grid.val[6], grid.val[7]);
    if (edgeTable[cubeindex] & 0b10000000)
        vertlist[7] =
        VertexInterp(isolevel, grid.p[7], grid.p[4], grid.val[7], grid.val[4]);
    if (edgeTable[cubeindex] & 0b100000000)
        vertlist[8] =
        VertexInterp(isolevel, grid.p[0], grid.p[4], grid.val[0], grid.val[4]);
    if (edgeTable[cubeindex] & 0b1000000000)
        vertlist[9] =
        VertexInterp(isolevel, grid.p[1], grid.p[5], grid.val[1], grid.val[5]);
    if (edgeTable[cubeindex] & 0b10000000000)
        vertlist[10] =
        VertexInterp(isolevel, grid.p[2], grid.p[6], grid.val[2], grid.val[6]);
    if (edgeTable[cubeindex] & 0b100000000000)
        vertlist[11] =
        VertexInterp(isolevel, grid.p[3], grid.p[7], grid.val[3], grid.val[7]);

    /* Create the triangle */
    int triangleCount = 0;
    for (i = 0; triTable[cubeindex][i] != -1; i += 3) {
        triangle3* cur = &triangles[triangleCount];
        cur->p[0] = vertlist[triTable[cubeindex][i]];
        cur->p[1] = vertlist[triTable[cubeindex][i + 1]];
        cur->p[2] = vertlist[triTable[cubeindex][i + 2]];
        triangleCount++;
    }

    return triangleCount;
}