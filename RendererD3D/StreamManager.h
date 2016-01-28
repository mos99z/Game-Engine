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
		static StreamManager*  instancePtr;

		static std::unordered_map<std::string, Model> AllModels;

		static ID3D11Buffer* GstreamBufferPtr;
		static ID3D11Buffer* TstreamBufferPtr;
		static ID3D11Buffer* AstreamBufferPtr;

	public:
		StreamManager();
		~StreamManager();

		static StreamManager& GetRef();
		static void DeleteInstance();

		void AddStream(std::string& _filename, RenderMesh& nodeMesh);
		unsigned int AddGstream(const Gstream* _RawGBufferPtr, unsigned int& _numofVertices);
		void AddTstream(const Tstream* _RawTBufferPtr, unsigned int& _numofVertices);
		void AddAstream(const Astream* _RawABufferPtr, unsigned int& _numofVertices);


	};

}