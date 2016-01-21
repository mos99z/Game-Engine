#include "InputLayout.hlsli"
#include "CommonShaderFunctions.hlsli"
#include "Samplers.hlsli"
#include "..\ConstantBuffer.h"

PS_GBUFFER_OUT main(VEROUT_PosNorDiffUVTan input)
{
	float SpecIntensity =0;
	float SpecPower = 1;
	input.color.xyz = map.Sample(anisoWrapSampler, float2(input.texcoord.x, 1.0f -input.texcoord.y)).xyz;
	return PackGBuffer(input.color, normalize(input.normal), SpecIntensity, SpecPower);
}