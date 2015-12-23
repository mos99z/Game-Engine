#include "stdafx.h"
#include "RenderSet.h"
#include "RenderNode.h"
namespace RendererD3D
{
	/*virtual*/ void RenderSet::AddNode(RenderNode* _node)
	{
		_node->SetNext(headPtr);
		headPtr = _node;
	}


}