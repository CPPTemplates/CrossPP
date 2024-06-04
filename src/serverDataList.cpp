#include "serverDataList.h"
#include "folderList.h"
#include "filesystem/filemanager.h"
#include "include/filesystem/fileFunctions.h"
#include "nbt/serializeList.h"

void serverDataList::serializeValue(nbtSerializer& s)
{
	serializeListOfSerializables(s, L"servers", servers);
}

bool serverDataList::serialize(cbool& write)
{
	createFoldersIfNotExists(serversFolder);
	stdPath serverListPath = serversFolder / std::wstring(L"serverlist" + nbtFileExtension);
	return nbtSerializable::serialize(L"server list", serverListPath, write);
}
