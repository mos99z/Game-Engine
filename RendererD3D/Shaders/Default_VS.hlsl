#include "InputLayout.hlsli"
#include "..\ConstantBuffer.h"

VSOUT main(VERIN_POSNORDIFF input) 
{
	VSOUT output = (VSOUT)0;
	output.pos = float4(input.position, 1.0f);
	output.color = input.diffuse;
	return output;
}