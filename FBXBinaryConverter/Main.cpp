#include <iostream>
#include "FBXConverterManager.h"
#include <Windows.h>
#include <WinBase.h>
#include <string>

//TODO: Remove AWBXLoader from refrences after testing
#if _DEBUG
#include <AWBXLoader.h>
#endif

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

#if _DEBUG
	AWBX::AWBXLoader loaderTest;

	unsigned int VertexSizes;
	VBuffer* VertexData = nullptr;
	unsigned int IndexSizes;
	unsigned int* IndexData = nullptr;

	loaderTest.LoadAWBXMesh("..\\Assets\\FBXs\\bat.AWBX", VertexSizes, (void**)&VertexData, IndexSizes, &IndexData);

	if (VertexData)
		delete[] VertexData;
	VertexData = nullptr;

	if (IndexData)
		delete[] IndexData;
	IndexData = nullptr;

#endif

	return EXIT_SUCCESS;
}