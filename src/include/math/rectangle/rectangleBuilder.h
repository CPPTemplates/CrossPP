#include "rectangletn.h"
#include <map>
#include <list>
template <typename t>
struct rectangleBuilder
{
    struct rectPair
    {
        rectanglei2 rect;
        t value;
    };
    // typedef std::pair<rectanglei2, t> rectPair;
    std::vector<rectPair *>
        finishedRectangles = std::vector<rectPair *>();
    // the height of currentRectBuilding is always 1!!
    rectPair *currentRectBuilding = nullptr;
    rectPair *currentRectOnTop = nullptr;
    // rects sorted by X coordinate
    std::map<int, rectPair *> sortedXRects = std::map<int, rectPair *>();
    std::vector<rectPair *> currentRowRects = std::vector<rectPair *>();

    int lastY = 0;

private:
    inline void finishRectangle()
    {
        if (currentRectBuilding)
        {
            // we assume that:
            //  currentRectOnTop is right above the rect we're building
            //  the starting X is the same
            //  the value is the same
            if (currentRectOnTop &&
                currentRectBuilding->rect.w == currentRectOnTop->rect.w &&
                currentRectBuilding->value == currentRectOnTop->value)
            {
                // merge this rectangle with rectontop
                currentRectOnTop->rect.h++;
                delete currentRectBuilding;
            }
            else
            {
                currentRowRects.push_back(currentRectBuilding);
            }
            currentRectOnTop = nullptr;
            currentRectBuilding = nullptr;
        }
    }
    inline void checkYChange(cveci2 &pos)
    {

        if (pos.y != lastY)
        {
            finishRectangle();
            // delete redundant rectangles
            for (auto it = sortedXRects.begin(); it != sortedXRects.end();)
            {
                // check if the rectangle got expanded into the last y row
                if ((it->second->rect.y + it->second->rect.h) == pos.y)
                {
                    //keep it in
                    it++;
                }
                else
                {
                    //this rectangle has gone out of reach for the current line. it's finished
                    finishedRectangles.push_back(it->second);
                    it = sortedXRects.erase(it);
                }
            }

            // add the rectangles of the row we just looped over
            for (rectPair *const &p : currentRowRects)
            {
                // index of map is the starting X of the rectangle
                sortedXRects[p->rect.x] = p;
            }
            currentRowRects.clear();
            lastY = pos.y;
        }
    }

public:
    inline void addNextPosition(cveci2 &pos, const t &value)
    {
        checkYChange(pos);
        if (currentRectBuilding && currentRectBuilding->value == value)
        {
            currentRectBuilding->rect.w++;
        }
        else
        {
            finishRectangle();
            // search in the rectangles on this x coordinate for a rectangle which expands to here
            // only access if it contains this position, to prevent creating elements
            if (sortedXRects.contains(pos.x))
            {
                rectPair *const &r = sortedXRects[pos.x];
                if (r->value == value)
                {
                    currentRectOnTop = r;
                }
            }

            currentRectBuilding = new rectPair(rectanglei2(pos, veci2(1)), value);
        }
    }
    inline void discardNextPosition(cveci2 &pos)
    {
        checkYChange(pos);
        finishRectangle();
    }
    inline void finish()
    {
        finishRectangle();
        for (auto const &rect : sortedXRects)
        {
            finishedRectangles.push_back(rect.second);
        }
        finishedRectangles.insert(finishedRectangles.end(), currentRowRects.begin(), currentRowRects.end());
    }
    ~rectangleBuilder()
    {
        for (const auto &rect : finishedRectangles)
        {
            delete rect;
        }
    }
};