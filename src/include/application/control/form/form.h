#include "application/control/control.h"

#pragma once
struct form :public control 
{
	form();
	virtual bool close();
    virtual bool wantsTextInput() const override;
};