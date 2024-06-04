#include "../event/eventhandler.h"
#include <string>
extern eventHandler<std::wstring> errorHandler;
//call this if you want to report an error, but the code is able to continue
void handleError(const std::wstring &message);
//call this if the code isn't able to continue after this
[[noreturn]] void handleCrash(const std::wstring &message);