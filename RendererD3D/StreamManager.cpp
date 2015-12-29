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

	
	
	int StreamManager::AppendToGstreamBuffer(Gstream& _gblock)
	{
		int startIndex = GstreamRawBuffer.size();
		GstreamRawBuffer.push_back(_gblock);
		return startIndex;
	}
	int StreamManager::AppendToTstreamBuffer(Tstream& _tblock)
	{
		int startIndex = TstreamRawBuffer.size();
		TstreamRawBuffer.push_back(_tblock);
		return startIndex;
	}
	int StreamManager::AppendToAstreamBuffer(Astream& _ablock)
	{
		int startIndex = AstreamRawBuffer.size();
		AstreamRawBuffer.push_back(_ablock);
		return startIndex;
	}
	int StreamManager::AppendToIstreamBuffer(Istream& _iblock)
	{
		int startIndex = IstreamRawBuffer.size();
		IstreamRawBuffer.push_back(_iblock);
		return startIndex;
	}
}