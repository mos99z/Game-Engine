#pragma once


namespace RendererD3D
{
	class RenderNode;
	typedef void(*RenderFunc)(RenderNode &node);

	class RenderNode
	{
	protected:
		RenderNode *nextPtr;
	public:
		RenderNode() { nextPtr = 0; }
		RenderFunc RenderFunc;
		void RenderProcess()
		{
			RenderFunc(*this);
		}
		inline RenderNode *GetNext(void) { return nextPtr; }
		inline void SetNext(RenderNode *nodePtr) { nextPtr = nodePtr; }
	};
}
