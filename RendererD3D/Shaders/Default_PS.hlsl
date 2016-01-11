#include "InputLayout.hlsli"
#include "CommonShaderFunctions.hlsli"
#include "Samplers.hlsli"
float4 main(VSOUT input) : SV_TARGET
{
	return map.Sample(anisoWrapSampler, input.color.xy);
}