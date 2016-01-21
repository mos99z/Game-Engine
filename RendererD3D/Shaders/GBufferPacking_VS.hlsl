#include "InputLayout.hlsli"
#include "..\ConstantBuffer.h"

VEROUT_PosNorDiffUVTan main(VERIN_PosNorDiffUVTan input)
{
	VEROUT_PosNorDiffUVTan output = (VEROUT_PosNorDiffUVTan)0;
	output.position = mul(float4(input.position, 1), gMVP);
	output.normal = mul(float4(input.normal, 0), gWorld);
	output.texcoord = input.texcoord;
	output.tangent = input.tangent;
	output.color = input.diffuse;
	return output;
}