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

	int numMeshes = 0;
	unsigned int* VertexSizes = nullptr;
	VBuffer** VertexData = nullptr;
	unsigned int* IndexSizes = nullptr;
	unsigned int** IndexData = nullptr;

	loaderTest.LoadAWBXMeshes("D:\\Game-Engine\\Assets\\FBXs\\bat.AWBX", numMeshes, &VertexSizes, (void***)&VertexData, &IndexSizes, &IndexData);

	if (numMeshes)
	{
		for (int mesh = 0; mesh < numMeshes; mesh++)
		{
			delete[] VertexData[mesh];
			VertexData[mesh] = nullptr;
			delete[] IndexData[mesh];
			IndexData[mesh] = nullptr;
		}
		delete[] VertexSizes;
		VertexSizes = nullptr;
		delete[] IndexSizes;
		IndexSizes = nullptr;
		delete[] VertexData;
		VertexData = nullptr;
		delete[] IndexData;
		IndexData = nullptr;
	}

#endif

	return EXIT_SUCCESS;
}