#include "include/filesystem/textfile.h"
#include "debug.h"
#include "handleError.h"
eventHandler<std::wstring> errorHandler = eventHandler<std::wstring>();
void handleError(const std::wstring &message)
{
    if (errorHandler.eventList.size)
        errorHandler.invoke(message);
    else
    {
        const std::string &messageToString = WStringToString(message);
        if constexpr (isDebugging)
        {
            throw std::runtime_error(messageToString);
        }
        else
        {
            // do nothing, this is release mode
        }
    }
}
[[noreturn]] void handleCrash(const std::wstring &message)
{
    // create crash log
    const std::string& strMessage = WStringToString(message);
    writealltext(stdPath(L"crashlog.txt"), strMessage);
    throw std::runtime_error("crash log saved in crashlog.txt");
}