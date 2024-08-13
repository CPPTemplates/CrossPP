#include "math/vector/vectn.h"
template <typename functionType>
constexpr veci2 iterate2Dto1D(veci2 position, int scale, directionID direction, functionType functionToCall, int flip = false)
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
        position = iterate2Dto1D(position, deeperScale, up, functionToCall, 1 - flip);
        // fill in the gaps
        movePos(up);
        position = iterate2Dto1D(position, deeperScale, direction, functionToCall, flip);
        movePos(direction);
        position = iterate2Dto1D(position, deeperScale, direction, functionToCall, flip);
        movePos(down);
        position = iterate2Dto1D(position, deeperScale, down, functionToCall, 1 - flip);
    }
    else
    {
        movePos(up);
        movePos(direction);
        movePos(down);
    }
    return position;
}