#pragma once
#include "Renderer.h"
namespace RendererD3D
{
	
	class InputLayoutManager
	{

		friend class Renderer;
		
	private:
		static InputLayoutManager* instancePtr;
		InputLayoutManager();
	public:
		enum VertexFormat {
			eVertex_POSNORDIFF = 0,
			eVertex_PosNorDiffUVTan,
			eVertex_PosNorDiffUVTanBoneWeight,
			eVetex_MAX,
		};
		CComPtr<ID3D11InputLayout>	 inputLayouts[eVetex_MAX];
		
		static InputLayoutManager& GetRef();
		static void DeleteInstance();
	};
}
