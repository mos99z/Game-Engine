#pragma once

namespace RendererD3D
{
	class RenderNode;

	class RenderSet
	{
		friend class Renderer;
	protected:
		RenderNode* headPtr = nullptr;
	public:
		RenderSet() {}
		virtual ~RenderSet() {}
		inline RenderNode* GetHead() { return headPtr; }
		inline void ClearSet() { headPtr  = nullptr; }

		virtual void AddNode(RenderNode* _node);
		


	};
}
