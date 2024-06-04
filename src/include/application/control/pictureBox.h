#include "control.h"
#pragma once
struct pictureBox :public control 
{
public:
	texture* backGroundImage = nullptr;
	pictureBox(texture* const& backGroundImage);
	virtual void renderBackGround(cveci2& position, const texture& renderTarget) override;
	//won't destruct the texture!
};