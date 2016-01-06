#include "InputLayout.hlsli"
float4 main(VERIN_PosNorDiffUVTan input) : SV_POSITION
{
	return float4(input.position, 1);
}