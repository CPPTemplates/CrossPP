#include "form.h"
#include <vector>
#include <string>

struct messageBoxEventArgs : controlEventArgs{
    size_t buttonIndex = 0;
    messageBoxEventArgs(control& sender, csize_t& buttonIndex) : controlEventArgs(sender), buttonIndex(buttonIndex){}
};

typedef eventHandler<messageBoxEventArgs> messageBoxEventHandler;

struct messageBox : public form
{
    messageBox(const std::wstring message, const std::vector<std::wstring>& buttonTexts);
    void layout(crectanglei2& rect) override;
    void onButtonClick(const controlEventArgs& args);
    messageBoxEventHandler onMessageBoxResponse = messageBoxEventHandler();
};

void showMessageBox(const std::wstring message, const std::vector<std::wstring>& buttonTexts, form* parent, messageBoxEventHandler::functionType onMessageBoxResponse = nullptr);