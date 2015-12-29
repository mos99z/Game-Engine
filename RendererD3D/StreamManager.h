#pragma once
#include "Renderer.h"
namespace RendererD3D
{
	struct _regAlign Gstream
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 specular;
	};
	struct _regAlign Tstream
	{
		DirectX::XMFLOAT4 texture1;
		DirectX::XMFLOAT4 tangent;
	};

	struct  _regAlign Astream
	{
		DirectX::XMINT4   bone;
		DirectX::XMFLOAT4 weights;
	};

	struct _regAlign Istream
	{
		DirectX::XMFLOAT4X4 worldMatrix;
	};

	class StreamManager
	{
		std::vector<Gstream> GstreamRawBuffer;
		std::vector<Tstream> TstreamRawBuffer;
		std::vector<Astream> AstreamRawBuffer;
		std::vector<Istream> IstreamRawBuffer;
	public:
		StreamManager();
		~StreamManager();
		int AppendToGstreamBuffer(Gstream& _gblock);
		int AppendToTstreamBuffer(Tstream& _tblock);
		int AppendToAstreamBuffer(Astream& _ablock);
		int AppendToIstreamBuffer(Istream& _iblock);

	};

}