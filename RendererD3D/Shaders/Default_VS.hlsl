#include "InputLayout.hlsli"
#include "..\ConstantBuffer.h"

//VSOUT main(VERIN_POSNORDIFF input) 
//{
//	VSOUT output = (VSOUT)0;
//	output.pos = float4(input.position, 1.0f);
//	output.color = input.diffuse;
//	return output;
//}

VEROUT_PosNorDiffUVTan main(VERIN_PosNorDiffUVTan input)
{
	VEROUT_PosNorDiffUVTan output = (VEROUT_PosNorDiffUVTan)0;
	output.position = mul(float4(input.position,1), gMVP);
	output.position.x = output.position.x;
	output.normal = input.normal;
	output.texcoord = input.texcoord;
	output.tangent = input.tangent;
	output.color = input.diffuse;
	return output;
}