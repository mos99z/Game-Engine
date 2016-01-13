#include "InputLayout.hlsli"
#include "CommonShaderFunctions.hlsli"
#include "Samplers.hlsli"
float4 main(VSOUT input) : SV_TARGET
{
	return float4(HemisphericAmbient(float3(0.0f,0.0f,0.0f),float3(1.0f,0.2f,0.2f),normalize(input.normal),float4(1.0f,1.0f,1.0f,1.0f)),1);
	//return input.color;
	//return map.Sample(anisoWrapSampler, input.color.xy);
}