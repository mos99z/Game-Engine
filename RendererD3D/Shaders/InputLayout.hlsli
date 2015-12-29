#include "../SharedDefines.h"

struct VERIN_POSNORDIFFSPEC
{
    float3 position : POSITION0;
	float3 normal : NORMAL0;
	float4 diffuse : COLOR0;
	float4 specular: COLOR1;
};

struct VERIN_PosNorDiffSpecUVTan
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float4 diffuse : COLOR0;
	float4 specular: COLOR1;
	float2 texcoord : TEXCOORD0;
	float3 tangent : TANGENT0;
};

struct VERIN_PosNorDiffSpecUVTanBoneWeight
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float4 diffuse : COLOR0;
	float4 specular: COLOR1;
	float2 texcoord : TEXCOORD0;
	float3 tangent : TANGENT0;
	int4   bone     :   BONEIDS0;
	float4 weights  :   BONEWEIGHTS0;
};
