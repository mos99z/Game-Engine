#include "InputLayout.hlsli"

float4 main(VSOUT input) : SV_TARGET
{
	return input.color;
}