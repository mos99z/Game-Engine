#pragma once
#include "RenderNode.h"

namespace RendererD3D
{

	class RenderMesh :
		public RenderNode
	{

	private:
		UINT numofVertices = 0;
		UINT startVertex = 0;
		UINT startIndex = 0;
		UINT numofIndices = 0;
	public:

		///Accessors
		inline unsigned int NumberOfVertices() const		{ return numofVertices; }
		inline unsigned int StartVertex() const				{ return startVertex; }
		inline unsigned int StartIndex() const				{ return startIndex; }
		inline unsigned int NumberOfIndices() const			{ return numofIndices; }

		///Mutators
		inline void SetNumberOfVertices(unsigned int _in)	{ numofVertices = _in; }
		inline void SetStartVertex(unsigned int _in)		{ startVertex = _in; }
		inline void SetStartIndex(unsigned int _in)			{ startIndex = _in; }
		inline void SetNumberOfIndices(unsigned int _in)	{ numofIndices = _in; }

		RenderMesh();
		~RenderMesh();
	};
}
