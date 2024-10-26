#include "saveFileControl.h"
#include "folderList.h"
#include "constants.h"
#include "math/graphics/texture.h"

saveFileControl::saveFileControl(const std::wstring& gameName) : gameTile(gameName)
{
	this->gameName = gameName;
	const stdPath& currentSaveFolder = savesFolder / gameName;
	const stdPath& currentSaveFolderScreenShot = currentSaveFolder / saveFolderScreenShot;
	if (stdFileSystem::exists(currentSaveFolderScreenShot))
	{
		screenShot->backGroundImage = new texture(currentSaveFolderScreenShot, true);
	}
}