#include "control.h"
#include "application/theme.h"
template <typename childListType>
inline void layoutTableCentered(crectanglei2 &rect, const childListType &controls, cveci2 &childSize = buttonSize, cint &margin = buttonMargin)
{
    cint &offsetStep = childSize.y + margin;
    // using end - begin instead of size() because fastlist doesn't have size()
    cveci2 &tablePos = rect.rectPos0Centered(cveci2(childSize.x, childSize.y + offsetStep * ((int)(controls.end() - controls.begin()) - 1)));
    rectanglei2 currentChildRect = rectanglei2(tablePos, childSize);
    for (auto c : controls)
    {
        c->layout(currentChildRect);
        currentChildRect.y += offsetStep;
    }
}
