#ifndef INPUTLAYOUT_HLSLI
#define INPUTLAYOUT_HLSLI
#include "../SharedDefines.h"

struct _regAlign VERIN_POSNORDIFF
{
    float3 position SEMANTIC(POSITION0);
	float3 normal SEMANTIC(NORMAL0);
	float4 diffuse SEMANTIC(COLOR0);
};

struct _regAlign VERIN_PosNorDiffUVTan
{
	float3 position SEMANTIC(POSITION0);
	float3 normal SEMANTIC(NORMAL0);
	float4 diffuse SEMANTIC(COLOR0);
	float2 texcoord SEMANTIC(TEXCOORD0);
	float3 tangent SEMANTIC(TANGENT0);
};

struct _regAlign VERIN_PosNorDiffUVTanBoneWeight
{
	float3 position SEMANTIC(POSITION0);
	float3 normal SEMANTIC(NORMAL0);
	float4 diffuse SEMANTIC(COLOR0);
	float2 texcoord SEMANTIC(TEXCOORD0);
	float3 tangent SEMANTIC(TANGENT0);
	int4   bone     SEMANTIC(BONEIDS0);
	float4 weights  SEMANTIC(BONEWEIGHTS0);
};
struct _regAlign VSOUT
{
	float4 pos SEMANTIC(SV_POSITION);
	float4 color SEMANTIC(COLOR);
};
#endif //INPUTLAYOUT_HLSLI