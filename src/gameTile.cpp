#include "gameTile.h"
#include "folderList.h"
#include "constants.h"
#include "math/graphics/texture.h"

gameTile::gameTile(const std::wstring& gameName) : form()
{
	this->gameName = gameName;
	screenShot = new pictureBox(nullptr);

	infoLabel = new label(gameName);

	addChildren({ screenShot,infoLabel });
}

gameTile::~gameTile()
{
	if (screenShot->backGroundImage)
	{
		delete screenShot->backGroundImage;
	}

}

void gameTile::layout(crectanglei2& newRect)
{
	form::layout(newRect);
	cint pictureBoxWidth = screenShot->backGroundImage ?
		(int)((rect.h * screenShot->backGroundImage->size.x) / screenShot->backGroundImage->size.y) :
		rect.h;

	screenShot->layout(crectanglei2(0, 0, pictureBoxWidth, rect.h));

	cint horizontalRoom = rect.w - pictureBoxWidth;

	infoLabel->layout(crectanglei2(pictureBoxWidth, 0, horizontalRoom, rect.h));
}