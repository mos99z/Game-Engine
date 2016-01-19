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
	float4 tangent SEMANTIC(TANGENT0);
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
struct _regAlign VEROUT_POSNORDIFF
{
	float4 position SEMANTIC(SV_POSITION);
	float3 normal  SEMANTIC(NORMAL);
	float4 color SEMANTIC(COLOR);
};

struct _regAlign VEROUT_PosNorDiffUVTan
{
	float4 position SEMANTIC(SV_POSITION);
	float3 normal  SEMANTIC(NORMAL);
	float4 color SEMANTIC(COLOR);
	float2 texcoord SEMANTIC(TEXCOORD);
	float4 tangent SEMANTIC(TANGENT);
};

struct _regAlign Material
{
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};


/*
SV_TARGET0 : depth,
SV_TARGET1 : diffuse
SV_TARGET2 : normal
SV_TARGET3 : spec
SV_TARGET4 : backBuffer
*/
struct _regAlign PS_GBUFFER_OUT
{
	float4 ColorSpecInt SEMANTIC(SV_TARGET4);
	float4 Normal		SEMANTIC(SV_TARGET2);
	float4 SpecPow		SEMANTIC(SV_TARGET3);
};

struct _regAlign SURFACE_DATA
{
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float SpecInt;
	float SpecPow;
};

#endif //INPUTLAYOUT_HLSLI