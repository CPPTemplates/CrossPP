#include "array/fastlist.h"
#include "event/eventhandler.h"
#include "event/eventargs.h"
#include <initializer_list>
#include <SFML/Window.hpp>
#include "math/vector/vectn.h"
#include "math/rectangle/rectangletn.h"
#include "math/graphics/color/color.h"
#include "math/graphics/color/colorPalette.h"

struct control;
struct texture;

typedef eventArgs<control> controlEventArgs;
typedef eventHandler<controlEventArgs> controlEventHandler;
struct mouseEventArgs : controlEventArgs
{
	veci2 position = veci2();
	mouseEventArgs(control& sender, cveci2& position) : controlEventArgs(sender), position(position) 
	{

	}
};
typedef eventHandler<mouseEventArgs> mouseEventHandler;

struct mouseButtonEventArgs : mouseEventArgs{
	mb button;
	mouseButtonEventArgs(control& sender, cveci2& position, cmb& button) : mouseEventArgs(sender, position), button(button)
	{

	}
};
typedef eventHandler<mouseButtonEventArgs> mouseButtonEventHandler;

struct dragEventArgs : mouseButtonEventArgs {
	veci2 originalPosition = veci2();
	dragEventArgs(control& sender, cveci2& originalPosition, cveci2& position, cmb& button) : mouseButtonEventArgs(sender, position, button), originalPosition(originalPosition)
	{

	}
};
typedef eventHandler<dragEventArgs> dragEventHandler;

struct mouseWheelEventArgs : mouseEventArgs
{
	int scrollDelta = 0;
	mouseWheelEventArgs(control& sender, cveci2& position, cint& scrollDelta):mouseEventArgs(sender, position), scrollDelta(scrollDelta)
	{
	
	}
};
typedef eventHandler<mouseWheelEventArgs> mouseWheelEventHandler;

struct keyEventArgs : controlEventArgs
{
	vk keyCode;
	keyEventArgs(control& sender, cvk& keyCode) : controlEventArgs(sender), keyCode(keyCode)
	{

	}
};
typedef eventHandler<keyEventArgs> keyEventHandler;

struct layoutEventArgs : controlEventArgs
{
	rectanglei2 newRect = rectanglei2();

	layoutEventArgs(control& sender, crectanglei2& newRect) : controlEventArgs(sender), newRect(newRect){}
};
typedef eventHandler<layoutEventArgs> layoutEventHandler;

#pragma once
struct control :IDestructable
{
public:
	//pointer functions

	keyEventHandler onKeyDown = keyEventHandler();
	keyEventHandler onKeyPress = keyEventHandler();
	keyEventHandler onKeyUp = keyEventHandler();

	controlEventHandler onClick = controlEventHandler();

	mouseWheelEventHandler onScroll = mouseWheelEventHandler();

    mouseButtonEventHandler onMouseDown = mouseButtonEventHandler();
    mouseButtonEventHandler onMouseUp = mouseButtonEventHandler();
    mouseButtonEventHandler onMouseMove = mouseButtonEventHandler();
	//mouseEventHandler onHover = mouseEventHandler();

	controlEventHandler onFocus = controlEventHandler();
	controlEventHandler onLostFocus = controlEventHandler();

	dragEventHandler onDrag = dragEventHandler();
	dragEventHandler onDrop = dragEventHandler();

	//void (*onFocusChild)(control* newFocusedChild) = nullptr;
	//void (*onLayout)(crectanglei2& newRect) = nullptr;
	//void (*onPaste)() = nullptr;

	control();


	//function invokers
	//position includes the position of this element
	virtual void render(cveci2& position, const texture& renderTarget);
	virtual void renderBorder(cveci2& position, const texture& renderTarget);
	virtual void renderBackGround(cveci2& position, const texture& renderTarget);
	virtual void renderText(cveci2& position, const texture& renderTarget);
	virtual void renderChildren(cveci2& position, const texture& renderTarget);
    virtual bool wantsTextInput() const;

	virtual void keyDown(cvk& keyCode);
	virtual void keyUp(cvk& keyCode);
	virtual void keyPress(cvk& keyCode);
	virtual void enterText(cuint& keyCode);

	virtual void mouseMove(cveci2& position, cmb& button);
	virtual void mouseDown(cveci2& position, cmb& button);
	virtual void mouseUp(cveci2& position, cmb& button);
	virtual void drag(cveci2& originalPosition, cveci2& position, cmb& button);
	virtual void drop(cveci2& originalPosition, cveci2& position, cmb& button);
	virtual void click();
	virtual void scroll(cveci2& position, cint& scrollDelta);
	virtual void layout(crectanglei2& newRect);
	void addChildren(std::initializer_list<control*> children);

    template<typename mostDerivedType, typename returnValueType, typename eventArgsType, typename eventHandlerType, typename ...eventHandlerTypes>
    inline void addEventHandlers( returnValueType(mostDerivedType::*memberFunction)(eventArgsType), eventHandlerType& handler, eventHandlerTypes&&... handlers)
    {
        //typename eventHandlerType::functionType f = std::bind(memberFunction, (mostDerivedType*)this, std::placeholders::_1);
        //handler.eventList.push_back(f);
        handler.hook(memberFunction, (mostDerivedType*)this);
        if constexpr (sizeof...(handlers)){
            addEventHandlers(memberFunction, std::forward<eventHandlerTypes>(handlers)...);
        }
    }

	//called when this control gains focus
	virtual void focus();
	//called when this control looses focus
	virtual void lostFocus();

	virtual void paste(const std::wstring& text);
	virtual std::wstring copy();

	//called when a child gains focus
	void focusChild(control* newFocusedChild);

	void switchVisibleChild(control* newFocusedChild);

	virtual ~control() override;
	constexpr bool hasBeenLayout() const {
		return rect.size.x;
	}

	//raycasts from top to bottom to check which child is hit
	control* getHighestChild(cveci2& pos) const;

	control* parent = nullptr;

	bool visible = true;
	//if this control has focus, regardless of one of its child controls having focus
	bool focused = false;

	rectanglei2 rect = rectanglei2();

	//pointer to the child that has the focus
	control* focusedChild = nullptr;
	fastList<control*> children = fastList<control*>();

	struct baseFont* currentFont = nullptr;

	std::wstring text = std::wstring();
	verticalAlignment textVerticalAlignment = verticalAlignment::top;
	horizontalAlignment textHorizontalAlignment = horizontalAlignment::left;
	int borderSize = 2;

	color backGroundColor = colorPalette::black;
	color borderColor = colorPalette::white;
};

struct theme& defaultTheme();
