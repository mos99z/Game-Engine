#pragma once
#include "Renderer.h"
namespace RendererD3D
{
	enum VertexFormat {
		eVertex_POSNORDIFFSPEC = 0, 
		eVertex_PosNorDiffSpecUVTan, 
		eVertex_PosNorDiffSpecUVTanBoneWeight,
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
