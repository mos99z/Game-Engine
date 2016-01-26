#include "stdafx.h"
#include "ABone.h"

namespace RendererD3D
{
	ABone::ABone()
	{
	}

	ABone::ABone(float3 _translation, float4 _rotationQuat, float3 _scale) : translation(_translation), 
		rotationQuat(_rotationQuat), scale(_scale)
	{
		using namespace DirectX;

		auto transformMat = XMMatrixAffineTransformation(XMLoadFloat3(&scale), XMVectorZero(), XMLoadFloat4(&rotationQuat), XMLoadFloat3(&translation));
		XMStoreFloat4x4(&transform, transformMat);
	}

	ABone::~ABone()
	{
	}


	float4x4 ABone::GetTransform() const
	{
		
		return transform;
	}
}