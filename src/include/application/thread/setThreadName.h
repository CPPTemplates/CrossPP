#include "globalFunctions.h"
#if onWindows
#include "windowsIncluder.h"
#elif onLinux
#include <sys/prctl.h>
#include "include/array/wstringFunctions.h"
#endif
#include <string>
/// @brief
/// @param threadName
/// @return true if the thread name was set successfully
inline bool setCurrentThreadName(const std::wstring &threadName)
{
#if onWindows
    HRESULT hr = SetThreadDescription(GetCurrentThread(), threadName.c_str());
    return !FAILED(hr);
#elif onAndroid
    const std::string &threadNameString = WStringToString(threadName);
    pthread_setname_np(pthread_self(), threadNameString.c_str());
#elif onLinux
    const std::string &threadNameString = WStringToString(threadName);
    prctl(PR_SET_NAME, threadNameString.c_str(), 0, 0, 0);
    return true;
#endif
    return false;
}