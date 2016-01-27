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
	input.normal.z = input.normal.z;
	output.normal = mul(float4(input.normal, 0), gWorld).xyz;
	//float y = -output.normal.y;
	//output.normal.y = output.normal.x;
	//output.normal.x = y;
	output.texcoord = input.texcoord;
	output.tangent = input.tangent;
	output.color = input.diffuse;
	return output;
}