#include "stdafx.h"
#include "StreamManager.h"
#include "RenderShape.h"
#include "IndexBufferManager.h"
#include <AWBXLoader.h>
namespace RendererD3D
{
	StreamManager*  StreamManager::instancePtr = nullptr;

	StreamManager& StreamManager::GetRef()
	{
		if (!instancePtr)
		{
			instancePtr = new StreamManager;
		}
		return *instancePtr;
	}

	void StreamManager::DeleteInstance()
	{
		delete instancePtr;
		instancePtr = nullptr;
	}

	StreamManager::StreamManager()
	{
	}


	StreamManager::~StreamManager()
	{
		delete[] GstreamRawBufferPtr;
		delete[] TstreamRawBufferPtr;
	}

	void StreamManager::AddGStream(std::string& _filename, RenderShape& renderShape)
	{
		static	AWBX::AWBXLoader loaderTest;
		unsigned int* indices = nullptr;

		loaderTest.LoadAWBXCombinedMesh(("..\\Assets\\FBXs\\" + _filename).c_str(), renderShape.numofVertices, (void**)&GstreamRawBufferPtr, (void**)&TstreamRawBufferPtr, renderShape.numofIndices, &indices);
		numofGstream += renderShape.numofVertices;
		IndexBufferManager::GetRef().AddIndices(indices, renderShape.numofIndices);


	}


}