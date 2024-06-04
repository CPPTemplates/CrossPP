#pragma once
#include "control.h"
struct textBox :public control {
public:
	textBox(cbool& multiLine = true);
	size_t cursorIndex = 0;
	bool capsLock = false;
	bool shift = false;
	bool multiLine;
	virtual void render(cveci2& position, const texture& renderTarget) override;
	virtual void enterText(cuint& uniCode) override;
	virtual void keyDown(cvk& keyCode) override;
	//virtual void keyUp(cvk& keyCode) override;
	virtual void lostFocus() override;
    virtual void focus() override;
	virtual void paste(const std::wstring& text) override;
	virtual std::wstring copy() override;
	virtual void mouseDown(cveci2& position, cmb& button) override;
	void checkCursorIndex();
    virtual bool wantsTextInput() const override;
};