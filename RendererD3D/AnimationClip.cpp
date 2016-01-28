#include "stdafx.h"
#include "AnimationClip.h"
#include "AKeyframe.h"
#include "ABone.h"
namespace RendererD3D
{
	AnimationClip::AnimationClip()
	{
	}


	AnimationClip::~AnimationClip()
	{
	}

	unsigned int AnimationClip::FindKeyframeIndex(float _time)
	{
		if (_time <= keyframes[0].TimeStamp())
		{
			return 0;
		}


		if (_time >= keyframes[numOfkeyframes - 1].TimeStamp())
		{
			return numOfkeyframes - 1;
		}

		UINT keyframeIndex = 1;
		for (; keyframeIndex < numOfkeyframes - 1 && _time >= keyframes[keyframeIndex].TimeStamp(); keyframeIndex++);
		return keyframeIndex - 1;
	}

	float4x4 AnimationClip::GetTransform(unsigned int _boneIndex, float _time)
	{
		using namespace DirectX;

		UINT keyframeIndex = FindKeyframeIndex(_time);
		if (keyframeIndex == 0 || keyframeIndex == numOfkeyframes - 1)
		{
			auto T = keyframes[keyframeIndex].Bones()[_boneIndex].Translation();
			auto R = keyframes[keyframeIndex].Bones()[_boneIndex].RotationQuat();
			auto S = keyframes[keyframeIndex].Bones()[_boneIndex].Scale();
			auto transformMat = XMMatrixAffineTransformation(XMLoadFloat3(&S), XMVectorZero(), XMLoadFloat4(&R), XMLoadFloat3(&T));
			float4x4 transform;
			XMStoreFloat4x4(&transform, transformMat);
			return transform;
		}
		else
		{
			auto T1 = keyframes[keyframeIndex].Bones()[_boneIndex].Translation();
			auto R1 = keyframes[keyframeIndex].Bones()[_boneIndex].RotationQuat();
			auto S1 = keyframes[keyframeIndex].Bones()[_boneIndex].Scale();
			auto T2 = keyframes[keyframeIndex + 1].Bones()[_boneIndex].Translation();
			auto R2 = keyframes[keyframeIndex + 1].Bones()[_boneIndex].RotationQuat();
			auto S2 = keyframes[keyframeIndex + 1].Bones()[_boneIndex].Scale();

			float lerpValue = (_time - keyframes[keyframeIndex].TimeStamp()) / (keyframes[keyframeIndex + 1].TimeStamp() - keyframes[keyframeIndex].TimeStamp());
			auto T = XMVectorLerp(XMLoadFloat3(&T1), XMLoadFloat3(&T2), lerpValue);
			auto R = XMQuaternionSlerp(XMLoadFloat4(&R1), XMLoadFloat4(&R2), lerpValue);
			auto S = XMVectorLerp(XMLoadFloat3(&S1), XMLoadFloat3(&S2), lerpValue);
			auto transformMat = XMMatrixAffineTransformation(S, XMVectorZero(), R, T);
			float4x4 transform;
			XMStoreFloat4x4(&transform, transformMat);
			return transform;
		}
	}

}