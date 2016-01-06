#include "InputLayout.hlsli"
float4 main(VERIN_PosNorDiffUVTanBoneWeight input) : SV_POSITION
{
	return float4(input.position, 1);
}