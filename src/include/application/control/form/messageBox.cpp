#include "messageBox.h"
#include "application/control/button.h"
#include "application/control/controlFunctions.h"
messageBox::messageBox(const std::wstring message, const std::vector<std::wstring> &buttonTexts)
{
    text = message;
    for (const std::wstring &buttonText : buttonTexts)
    {
        button *b = new button(buttonText);
        addChildren({b});
        b->onClick.hook(&messageBox::onButtonClick, this);
    }
}

void messageBox::layout(crectanglei2 &rect)
{
    control::layout(rect);
    layoutTableCentered(rect, children);
}

void messageBox::onButtonClick(const controlEventArgs &args)
{
    onMessageBoxResponse.invoke(messageBoxEventArgs(*this, children.find(&args.sender)));
    if (parent->focusedChild == this)
    {
        parent->focusedChild = nullptr;
    }
    parent->children.erase(this);
    parent->children.update();
    delete this;
}

void showMessageBox(const std::wstring message, const std::vector<std::wstring> &buttonTexts, form *parent, messageBoxEventHandler::functionType onMessageBoxResponse)
{
    messageBox *const &m = new messageBox(message, buttonTexts);
    parent->addChildren({m});
    m->layout(parent->rect);
    m->onMessageBoxResponse.hook(onMessageBoxResponse);
}
