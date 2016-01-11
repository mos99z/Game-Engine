#pragma once

#ifdef RENDERSETDLL_EXPORTS
#define  RENDERSETDLL __declspec(dllexport) 
#else
#define  RENDERSETDLL __declspec(dllimport) 
#endif

namespace RendererD3D
{
	class RenderNode;
	class RenderSet
	{
		friend class Renderer;
	protected:
		RenderNode* headPtr = nullptr;
	public:
		RENDERSETDLL RenderSet() {}
		RENDERSETDLL virtual ~RenderSet() {}
		RENDERSETDLL inline RenderNode* GetHead() { return headPtr; }
		RENDERSETDLL inline void ClearSet() { headPtr  = nullptr; }

		RENDERSETDLL virtual void AddNode(RenderNode* _node);
		


	};
}
