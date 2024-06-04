#include "filemanager.h"
#include <string>
inline std::wstring findFileRecursive(const std::wstring& fileNameWithExtionsion, const std::wstring& folderIn)
{
	for (const auto& fileIterator : stdFileSystem::recursive_directory_iterator(folderIn))
	{
		const std::wstring currentFileNameWithExtension = fileIterator.path().filename().wstring();
		if (stdPath(currentFileNameWithExtension).compare(stdPath(fileNameWithExtionsion)) == 0)
		{
			return fileIterator.path().wstring();
		}
	}
	return std::wstring();
}

//only creates last directory
inline void createFolderIfNotExists(const stdPath& path)
{
	if (!stdFileSystem::is_directory(path) || !stdFileSystem::exists(path))
	{
		stdFileSystem::create_directory(path); // create folder
	}
}

//creates the whole path to this directory
inline void createFoldersIfNotExists(const stdPath& path)
{
	if (!stdFileSystem::is_directory(path) || !stdFileSystem::exists(path))
	{
		stdFileSystem::create_directories(path); // create folder
	}
}