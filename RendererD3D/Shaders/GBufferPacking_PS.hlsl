#include "InputLayout.hlsli"
#include "CommonShaderFunctions.hlsli"
#include "Samplers.hlsli"
#include "..\ConstantBuffer.h"

//Deferred Shading Functions
static const float2 g_SpecPowerRange = { 0.1, 250.0 };

PS_GBUFFER_OUT PackGBuffer(float3 BaseColor, float3 Normal, float SpecIntensity, float SpecPower)
{
	PS_GBUFFER_OUT Out;
	float SpecPowerNorm = (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;
	Out.ColorSpecInt = float4(BaseColor.rgb, SpecIntensity);
	Out.Normal = float4(Normal.xyz * 0.5 + 0.5, 0.0);
	Out.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);
	return Out;
}


PS_GBUFFER_OUT main(VEROUT_PosNorDiffUVTan input)
{
	float SpecIntensity =0;
	float SpecPower = 1;
	input.color.xyz = map.Sample(anisoWrapSampler, float2(input.texcoord.x, 1.0f -input.texcoord.y)).xyz;
	return PackGBuffer(input.color, normalize(input.normal), SpecIntensity, SpecPower);
}