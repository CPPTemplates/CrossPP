#include "math/vector/vectn.h"
// the functions here provide a scalable way to convert 2d to 1d, for example used when converting images to audio.

/// @brief this function iterates a 2d square-formed grid by splitting it into smaller pieces to iterate. its trajectory looks a bit like a puzzle.
/// @tparam functionType
/// @param position
/// @param scale
/// @param direction
/// @param functionToCall
/// @param flip
/// @return
template <typename functionType>
constexpr veci2 iterate2Dto1DPuzzle(veci2 position, int scale, directionID direction, functionType functionToCall, int flip = false)
{
    cint &deeperScale = scale / 2;
    const directionID &up = rotate2DDegrees(direction, 270 + flip * -180);
    const directionID &down = flipDirection(up);

    const auto movePos = [&functionToCall, &position](const directionID &direction)
    {
        functionToCall(position);
        position += directionVectors2D[(int)direction];
    };

    if (deeperScale > 1)
    {
        // flipped, not flipped, not flipped, flipped
        position = iterate2Dto1DPuzzle(position, deeperScale, up, functionToCall, 1 - flip);
        // fill in the gaps
        movePos(up);
        position = iterate2Dto1DPuzzle(position, deeperScale, direction, functionToCall, flip);
        movePos(direction);
        position = iterate2Dto1DPuzzle(position, deeperScale, direction, functionToCall, flip);
        movePos(down);
        position = iterate2Dto1DPuzzle(position, deeperScale, down, functionToCall, 1 - flip);
    }
    else
    {
        movePos(up);
        movePos(direction);
        movePos(down);
    }
    return position;
}

template <typename functionType>
constexpr void iterate2Dto1D(veci2 position, int scale, functionType functionToCall)
{
    cint &deeperScale = scale / 2;
    constexpr veci2 toAdd[4]{
        veci2(0, 0),
        veci2(1, 0),
        veci2(0, 1),
        veci2(1, 1)};
    if (deeperScale == 1)
        for (cveci2 &vec : toAdd)
            functionToCall(position + vec);
    else
        for (cveci2 &vec : toAdd)
            iterate2Dto1D(position + vec * deeperScale, deeperScale, functionToCall);
}