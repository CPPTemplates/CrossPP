#pragma once
#include "arraynd.h"
template <typename t>
inline void floodFill(const array2d<t> &array, cveci2 &pos, const t &value)
{
    //	1. If target is equal to replacement, return.
    t target = array.getValue(pos);
    if (target == value)
        return;
    // 3. Set the value of node to replacement.
    array.setValue(pos, value);
    // 4. Set Q to the empty queue.
    std::list<veci2> Q = std::list<veci2>();
    // 5. Add node to the end of Q.
    Q.push_back(pos);
    // 6. While Q is not empty:
    while (Q.size() > 0)
    {
        // 7. Set n equal to the first element of Q.
        veci2 n = *Q.begin();
        // 8.     Remove first element from Q.
        Q.pop_front();
        // 9.     If the value of the node to the west of n is target,
        if (n.x > 0 && array.getValue(n.x - 1, n.y) == target)
        {
            // set the value of that node to replacement and add that node to the end of Q.
            array.setValue(n.x - 1, n.y, value);
            Q.push_back(veci2(n.x - 1, n.y));
        }
        // 10.     If the value of the node to the east of n is target,
        if (n.x < array.size.x - 1 && array.getValue(n.x + 1, n.y) == target)
        {
            // set the value of that node to replacement and add that node to the end of Q.
            array.setValue(n.x + 1, n.y, value);
            Q.push_back(veci2(n.x + 1, n.y));
        }
        // 11.     If the value of the node to the north of n is target,
        if (n.y > 0 && array.getValue(n.x, n.y - 1) == target)
        {
            // set the value of that node to replacement and add that node to the end of Q.
            array.setValue(n.x, n.y - 1, value);
            Q.push_back(veci2(n.x, n.y - 1));
        }
        // 12.     If the value of the node to the south of n is target,
        if (n.y < array.size.y - 1 && array.getValue(n.x, n.y + 1) == target)
        {
            // set the value of that node to replacement and add that node to the end of Q.
            array.setValue(n.x, n.y + 1, value);
            Q.push_back(veci2(n.x, n.y + 1));
        }
        // 13. Continue looping until Q is exhausted.
    }
    // 14. Return.
}

// template <typename t, fsize_t n>
// inline void copyArrayUnsafe(const array2d<t> &array, const array2d<t> &otherArray, crectanglet2<fsize_t> &destinationRect, cfsize_t &otherArrayWidth)
//{
//     cvect2<fsize_t> &pos11 = destinationRect.pos1();
//
//     t *const &xPtr = baseArrayPointer + destinationRect.x;
//     const t *const &endPtr = xPtr + pos11.y * array.size.x;
//
//     for (
//         t *ptr = xPtr + (destinationRect.pos0.y * array.size.x);
//         ptr < endPtr;
//         ptr += array.size.x, otherArrayPointer += otherArrayWidth)
//     {
//         std::copy(otherArrayPointer, otherArrayPointer + destinationRect.size.x, ptr);
//     }
// }

template <typename t>
inline void copyArrayUnsafe(const array2d<t> &source, const array2d<t> &destination, crectanglet2<fsize_t> &sourceRect, cvect2<fsize_t> &destinationPosition)
{
    t *sourceYPtr = &source.getValueReferenceUnsafe(sourceRect.pos0);
    t *destYPtr = &destination.getValueReferenceUnsafe(destinationPosition);

    for (const t *const &sourceEndPtr = sourceYPtr + source.size.x * sourceRect.size.y;
         sourceYPtr < sourceEndPtr; sourceYPtr += source.size.x, destYPtr += destination.size.x)
    {
        std::copy(sourceYPtr, sourceYPtr + sourceRect.size.x, destYPtr);
    }
}

template <typename valueType, fsize_t n>
inline void copyArray(const arraynd<valueType, n> &source, const arraynd<valueType, n> &destination, crectanglei2 &sourceRect, cveci2 &destinationPosition)
{
    //crop the rectangle both by source and destination
    //we can do this by cropping the sourcerect, then moving it to the destination, and then cropping it by the destination as well.
    //afterwards, we calculate the sourcerect again
    rectanglei2 croppedSourceRect = sourceRect;
    if (crectanglei2(source.getClientRect()).cropClientRect(croppedSourceRect))
    {
        cveci2& sourceRectCropOffset = croppedSourceRect.pos0 - sourceRect.pos0;
        crectanglei2& destinationRect = crectanglei2(
            destinationPosition + sourceRectCropOffset,
            croppedSourceRect.size);
        rectanglei2 croppedDestinationRect = destinationRect;

        crectanglei2(destination.getClientRect()).cropClientRect(croppedDestinationRect);

        croppedSourceRect = rectanglei2(croppedSourceRect.pos0 + (croppedDestinationRect.pos0 - destinationRect.pos0), croppedDestinationRect.size);

        copyArrayUnsafe(source, destination, crectanglet2<fsize_t>(croppedSourceRect), croppedDestinationRect.pos0);
    }
}
template <typename t, fsize_t n>
inline void copyArray(const array2d<t> &source, const arraynd<t, n> &destination, cveci2 &destinationPosition = veci2())
{
    copyArray(source, destination, source.getClientRect(), destinationPosition);
}

template <typename valueType, fsize_t n>
inline arraynd<valueType, n> expanded(const arraynd<valueType, n> &array, crectanglein<n> &newRect)
{
    const arraynd<valueType, n> &resizedArray = arraynd<valueType, n>(newRect.size, true);
    // copy all that can be copied
    copyArray(array, resizedArray, crectanglei2(newRect.size), -newRect.pos0);
    return resizedArray;
}

template <typename valueType, fsize_t n>
inline void expandToContain(arraynd<valueType, n> &array, cvecin<n> &relativePosition)
{
    if (!array.inBounds(relativePosition))
    {
        rectanglein<n> newRelativeRect = rectanglein<n>(array.getClientRect());
        newRelativeRect.expandToContain(crectanglein<n>(relativePosition, cvecin<n>(1)));
        array = expanded(array, newRelativeRect);
    }
}

template <typename t, typename compareFunction>
inline bool findRayCast(const array2d<t> &array, cvec2 &p0, cvec2 &p1, veci2 &resultingBlockPos, veci2 &adjacentBlockPosition, vec2 &exactIntersection, const compareFunction &&function)
{
    // https://www.redblobgames.com/grids/line-drawing.html
    // http://www.cse.yorku.ca/~amana/research/grid.pdf
    cvec2 d0to1 = p1 - p0; // dx
    cveci2 sign = veci2(d0to1.x > 0 ? 1 : -1, d0to1.y > 0 ? 1 : -1);

    // not dx * sign_x, because you can get -0 and -inf
    cvec2 absd0to1 = d0to1.absolute(); // nx
    veci2 currentPosition = veci2((int)floor(p0.x), (int)floor(p0.y));
    int dimension = -1;
    cvec2 delta = {1 / absd0to1.x, 1 / absd0to1.y}; // deltax
    // vx
    vec2 progress = {
        d0to1.x > 0 ? currentPosition.axis[0] + 1 - p0.x : p0.x - currentPosition.axis[0],
        d0to1.y > 0 ? currentPosition.axis[1] + 1 - p0.y : p0.y - currentPosition.axis[1]};
    progress *= delta; //'divide' by total length
    int count = 0;
    bool looping = false; // initialize to get the warning to shut up
    t hit = t();
    do
    {
        // check point
        hit = array.getValue(currentPosition);
        if (function(hit))
        {
            break;
        }
        dimension = progress.x < progress.y ? 0 : 1;
        currentPosition.axis[dimension] += sign.axis[dimension]; // step in the active dimenstion
        looping = progress.axis[dimension] <= 1;
        progress.axis[dimension] += delta.axis[dimension]; // update progress
    } while (looping);
    // hit =array.getValue(currentPosition);
    bool visible = function(hit);
    resultingBlockPos = currentPosition;
    adjacentBlockPosition = currentPosition;
    if (dimension != -1)
    {
        adjacentBlockPosition.axis[dimension] -= sign.axis[dimension];
        if (!visible)
        {
            resultingBlockPos = adjacentBlockPosition;
        }
    }
    if (visible)
    {
        if (dimension == -1)
        {
            exactIntersection = p0;
        }
        else
        {
            // calculate the exact intersection point.
            exactIntersection.axis[dimension] = currentPosition.axis[dimension];
            if (sign.axis[dimension] == -1)
            {
                exactIntersection.axis[dimension]++; // this axis has to be increased because the ray hit on the plus side of the block.
            }
            cfp distance = abs(exactIntersection.axis[dimension] - p0.axis[dimension]);
            int otherAxis = dimension == 0 ? 1 : 0;
            fp part = distance / absd0to1.axis[dimension]; // the part of the ray that has been marched
            fp exactCoordinate = p0.axis[otherAxis] + part * d0to1.axis[otherAxis];
            exactIntersection.axis[otherAxis] = exactCoordinate;
        }
    }
    else
    {
        exactIntersection = p1;
    }
    return visible;
}
// to is NOT compared compared!
// the 'elements' outside the border are not compared
template <typename t, typename compareFunction>
inline bool find2dBox(const array2d<t> &array, cveci2 &pos00, cveci2 &pos11, const compareFunction &&function)
{
    for (veci2 position = pos00; position.y < pos11.y; position.y++)
    {
        for (position.x = pos00.x; position.x < pos11.x; position.x++)
        {
            if (function(array.getValue(position.x, position.y)))
            {
                return true;
            }
        }
    }
    return false;
}
