#include "../SharedDefines.h"

struct VERIN_POSNORDIFF
{
    float3 position : POSITION0;
	float3 normal : NORMAL0;
	float4 diffuse : COLOR0;
};

struct VERIN_PosNorDiffUVTan
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float4 diffuse : COLOR0;
	float2 texcoord : TEXCOORD0;
	float3 tangent : TANGENT0;
};

struct VERIN_PosNorDiffUVTanBoneWeight
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float4 diffuse : COLOR0;
	float2 texcoord : TEXCOORD0;
	float3 tangent : TANGENT0;
	int4   bone     :   BONEIDS0;
	float4 weights  :   BONEWEIGHTS0;
};
