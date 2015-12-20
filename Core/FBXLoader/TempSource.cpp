#include <iostream>
#include "FBXLoaderManager.h"

int main(int argc, char** argv)
{
	FBXLoaderManager* fbxParser = new FBXLoaderManager();

	for (int file = 0; file < argc; file++)
		fbxParser->Initilize(argv[file]);

	delete fbxParser;
	return EXIT_SUCCESS;
}