#pragma once
#include "SharedDefines.h"

namespace RendererD3D
{
	class RenderNode;
	typedef void(*RenderFunc)(RenderNode &node);

	class RenderNode
	{
	protected:
		RenderNode *nextPtr = nullptr;
	public:
		RenderFunc RenderFunc;

		RenderNode() {}
		~RenderNode() {}

		inline RenderNode *GetNext(void) { return nextPtr; }
		inline void SetNext(RenderNode *nodePtr) { nextPtr = nodePtr; }
		
		inline void RenderProcess()
		{
			RenderFunc(*this);
		}

	};
}
