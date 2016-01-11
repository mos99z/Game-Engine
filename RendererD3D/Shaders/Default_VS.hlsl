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
	output.color = float4(input.normal,1);
	return output;
}