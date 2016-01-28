#pragma once
#include "SharedDefines.h"
namespace RendererD3D
{

	class ABone
	{
		float3 translation;
		float4 rotationQuat;
		float3 scale;


	public:
		ABone();
		ABone(float3 _translation, float4 _rotationQuat, float3 _scale);
		~ABone();

		inline float3 Translation() const { return translation; }
		inline float4 RotationQuat() const { return rotationQuat; }
		inline float3 Scale() const { return scale; }

	};
}
