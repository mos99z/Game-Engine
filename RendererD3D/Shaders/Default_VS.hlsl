#include "InputLayout.hlsli"
#include "..\ConstantBuffer.h"

float4 main(VERIN_POSNORDIFF input) : SV_POSITION
{
	return mul(float4(input.position,1),gMVP);
}