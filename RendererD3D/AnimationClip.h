#pragma once
#include "SharedDefines.h"
namespace RendererD3D
{
	class AKeyframe;
	class AnimationClip
	{
		unsigned int numOfkeyframes;
		AKeyframe* keyframes = nullptr;
	public:
		AnimationClip();
		~AnimationClip();
		unsigned int FindKeyframeIndex(float _time);
		float4x4 GetTransform(unsigned int _boneIndex, float _time);
	};
}

