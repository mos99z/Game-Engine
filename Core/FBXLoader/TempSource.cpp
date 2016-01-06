#include <iostream>
#include "FBXLoaderManager.h"
#include <Windows.h>
#include <WinBase.h>
#include <string>

int main(int argc, char** argv)
{
	FBXLoaderManager fbxParser;

	WIN32_FIND_DATA findFileData;
	std::wstring fileNames = FBXLoaderManager::GetFilePathAndTypes();
	HANDLE findHandle = FindFirstFile(fileNames.c_str(), &findFileData);
	if (findHandle == INVALID_HANDLE_VALUE)
		return EXIT_FAILURE;

	do
	{
		fbxParser.Initilize(findFileData.cFileName);
	} while (FindNextFile(findHandle, &findFileData));
	FindClose(findHandle);

	return EXIT_SUCCESS;
}