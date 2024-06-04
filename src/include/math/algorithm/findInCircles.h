#include "math/vector/vectn.h"
#include "math/mathFunctions.h"
template <typename checkFunction>
constexpr bool findInCircles(int maxRadius, checkFunction check, veci2 &result)
{
    cint &maxRadiusSquared = math::squared(maxRadius);

    std::multiset<std::pair<int, veci2>> positions = std::multiset<std::pair<int, veci2>>();
    for (int x = 0; x < maxRadius; x++)
    {
        cint &distanceXSquared = x * x;
        // cint& maxY = sqrt((fp)(maxRadiusSquared - distanceXSquared)); we need to find a faster way

        for (int y = x; ; y++)
        {
            cint &distanceSquaredPlusY = y * y + distanceXSquared;
            if (distanceSquaredPlusY < maxRadiusSquared)
            {
                positions.emplace(std::make_pair(distanceSquaredPlusY, veci2(x, y)));
            }
            else
                break;
        }
        for (auto it = positions.begin();
             it != positions.end() && it->first <= distanceXSquared; it = positions.erase(it))
        {
            cveci2 &position = it->second;
            if (position.x == position.y)
            {
                if (position.x == 0)
                {
                    if (check(position))
                    {
                        result = position;
                        return true;
                    }
                }
                else
                {
                    cveci2 checkArray[4] = {
                        position,
                        cveci2(position.x, -position.y),
                        cveci2(-position.x, position.y),
                        cveci2(-position.x, -position.y)};
                    for (cveci2 &pos : checkArray)
                    {
                        if (check(pos))
                        {
                            result = pos;
                            return true;
                        }
                    }
                }
            }
            else if (position.x == 0)
            {
                //no -x, because x == -x
                cveci2 checkArray[4] = {
                    position,
                    cveci2(position.x, -position.y),
                    cveci2(position.y, position.x),
                    cveci2(-position.y, position.x),
                };
                for (cveci2 &pos : checkArray)
                {
                    if (check(pos))
                    {
                        result = pos;
                        return true;
                    }
                }
            }
            else if (position.y == 0)
            {
                //no -y, because y == -y
                cveci2 checkArray[4] = {
                    position,
                    cveci2(-position.x, position.y),
                    cveci2(position.y, position.x),
                    cveci2(position.y, -position.x),
                };
                for (cveci2 &pos : checkArray)
                {
                    if (check(pos))
                    {
                        result = pos;
                        return true;
                    }
                }
            }
            else
            {
                cveci2 checkArray[8] = {
                    position,
                    cveci2(position.x, -position.y),
                    cveci2(-position.x, position.y),
                    cveci2(-position.x, -position.y),
                    cveci2(position.y, position.x),
                    cveci2(position.y, -position.x),
                    cveci2(-position.y, position.x),
                    cveci2(-position.y, -position.x),
                };
                for (cveci2 &pos : checkArray)
                {
                    if (check(pos))
                    {
                        result = pos;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
