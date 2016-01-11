#pragma once

#ifdef RENDERNODEDLL_EXPORTS
#define  RENDERNODEDLL __declspec(dllexport) 
#else
#define  RENDERNODEDLL __declspec(dllimport) 
#endif

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

		RENDERNODEDLL RenderNode() {}
		RENDERNODEDLL ~RenderNode() {}

		RENDERNODEDLL inline RenderNode *GetNext(void) { return nextPtr; }
		RENDERNODEDLL inline void SetNext(RenderNode *nodePtr) { nextPtr = nodePtr; }
		
		inline void RenderProcess()
		{
			RenderFunc(*this);
		}

	};
}
