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
	
	struct Model
	{
		unsigned int numOfVertices = 0;
		unsigned int numOfIndices = 0;
		unsigned int stratVertex = 0;
		unsigned int stratIndex = 0;
	};

	class RenderMesh;

	class StreamManager
	{
		
	public:
		static ID3D11Buffer* GstreamBufferPtr;
		static ID3D11Buffer* TstreamBufferPtr;
		Tstream* TstreamRawBufferPtr = nullptr;
		/*std::vector<Astream> AstreamRawBuffer;
		std::vector<Istream> IstreamRawBuffer;*/
		static StreamManager*  instancePtr;
		static std::unordered_map<std::string, Model> AllModels;
		UINT numofGstream = 0;
		UINT numofTstream = 0;
	public:
		StreamManager();
		~StreamManager();
		static StreamManager& GetRef();
		static void DeleteInstance();


		void AddGStream(std::string& _filename, RenderMesh& nodeMesh);

	};

}