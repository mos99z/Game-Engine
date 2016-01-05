#include "stdafx.h"
#include "StreamManager.h"

namespace RendererD3D
{
	StreamManager::StreamManager()
	{
	}


	StreamManager::~StreamManager()
	{
		
	}

	
	
	auto StreamManager::AppendToGstreamBuffer(Gstream& _gblock)
	{
		auto startIndex = GstreamRawBuffer.size();
		GstreamRawBuffer.push_back(_gblock);
		return startIndex;
	}
	auto StreamManager::AppendToTstreamBuffer(Tstream& _tblock)
	{
		auto startIndex = TstreamRawBuffer.size();
		TstreamRawBuffer.push_back(_tblock);
		return startIndex;
	}
	auto StreamManager::AppendToAstreamBuffer(Astream& _ablock)
	{
		auto startIndex = AstreamRawBuffer.size();
		AstreamRawBuffer.push_back(_ablock);
		return startIndex;
	}
	auto StreamManager::AppendToIstreamBuffer(Istream& _iblock)
	{
		auto startIndex = IstreamRawBuffer.size();
		IstreamRawBuffer.push_back(_iblock);
		return startIndex;
	}
}