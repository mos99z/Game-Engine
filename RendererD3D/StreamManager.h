#pragma once
#include "Renderer.h"
namespace RendererD3D
{
	struct _regAlign Gstream
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 diffuse;
	};
	struct _regAlign Tstream
	{
		DirectX::XMFLOAT2 texture1;
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
		auto AppendToGstreamBuffer(Gstream& _gblock);
		auto AppendToTstreamBuffer(Tstream& _tblock);
		auto AppendToAstreamBuffer(Astream& _ablock);
		auto AppendToIstreamBuffer(Istream& _iblock);

	};

}