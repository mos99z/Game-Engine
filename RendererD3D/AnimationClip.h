#pragma once
#include "SharedDefines.h"
namespace RendererD3D
{
	class AKeyframe;
	class AnimationClip
	{
	public:
		unsigned int numOfkeyframes;
		AKeyframe* keyframes = nullptr;
		AnimationClip();
		~AnimationClip();
		unsigned int FindKeyframeIndex(float _time);
		float4x4 GetTransform(unsigned int _boneIndex, float _time);
	};
}

