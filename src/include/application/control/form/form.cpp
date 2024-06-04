#include "form.h"
form::form():control()
{
}

bool form::close()
{
    return true;
}

bool form::wantsTextInput() const {
    return focusedChild && focusedChild->wantsTextInput();
}
