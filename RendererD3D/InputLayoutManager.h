#pragma once
#include "Renderer.h"
namespace RendererD3D
{
	enum VertexFormat {
		eVertex_POSNORDIFF= 0, 
		eVertex_PosNorDiffUVTan, 
		eVertex_PosNorDiffUVTanBoneWeight,
		eVetex_MAX,
	};
	class InputLayoutManager
	{
		friend class Renderer;
		
	public:
		ID3D11InputLayout*	 inputLayouts[eVetex_MAX];
		InputLayoutManager();
		~InputLayoutManager();
	};
}
