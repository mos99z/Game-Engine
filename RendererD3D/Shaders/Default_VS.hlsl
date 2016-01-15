#include "InputLayout.hlsli"
#include "..\ConstantBuffer.h"

//VSOUT main(VERIN_POSNORDIFF input) 
//{
//	VSOUT output = (VSOUT)0;
//	output.pos = float4(input.position, 1.0f);
//	output.color = input.diffuse;
//	return output;
//}

VSOUT main(VERIN_POSNORDIFF input)
{
	VSOUT output = (VSOUT)0;
	output.pos = mul(float4(input.position,1), gMVP);
	output.pos.x = output.pos.x;
	output.normal = input.normal;
	output.normal.x = output.normal.x;

	output.color = input.diffuse;
	return output;
}