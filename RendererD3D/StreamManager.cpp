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

	void StreamManager::AddGStream(RenderShape& renderShape)
	{
		AWBX::AWBXLoader loaderTest;
		unsigned int* indices = nullptr;
		loaderTest.LoadAWBXMesh("E:\\GitHub\\Clone\\Game-Engine\\Assets\\FBXs\\Teddy_Idle.AWBX", renderShape.numofVertices, (void**)&GstreamRawBufferPtr, renderShape.numofIndices, &indices);
		numofGstream += renderShape.numofVertices;
		IndexBufferManager::GetRef().AddIndices(indices, renderShape.numofIndices);


	}


}