#pragma once
#include "RenderNode.h"

namespace RendererD3D
{
	
	class RenderMesh :
		public RenderNode
	{
	public:
		enum MeshFiles
		{
			MESH_NAME_1, MESH_NAME_2, MESH_NAME_3, MESH_NAME_4, MESH_NAME_5, MESH_COUNT
		};
	private:
		 std::wstring meshFiles[MESH_COUNT];
	public:
		RenderMesh();
		~RenderMesh();
	};
}
