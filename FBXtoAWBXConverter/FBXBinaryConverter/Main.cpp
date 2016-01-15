#include <iostream>
#include "FBXConverterManager.h"
#include <Windows.h>
#include <WinBase.h>
#include <string>
#include <vector>

int wmain(int argc, wchar_t** argv)
{
	FBXLoaderManager fbxParser;

	for (int path = 1; path < argc; path++)
	{
		if (wcsstr(argv[path], L".fbx"))
		{
			fbxParser.Initilize(argv[path]);
		}
		else
		{
			std::wstring filePath = argv[path];
			filePath.append(L"\\*.fbx");
			WIN32_FIND_DATA findFileData;
			HANDLE findHandle = FindFirstFile(filePath.c_str(), &findFileData);
			if (findHandle == INVALID_HANDLE_VALUE)
				return EXIT_FAILURE;
			do
			{
				filePath = argv[path];
				filePath.append(L"\\");
				filePath.append(findFileData.cFileName);
				fbxParser.Initilize(filePath.c_str());
			} while (FindNextFile(findHandle, &findFileData));
			FindClose(findHandle);
		}
	}

	return EXIT_SUCCESS;
}