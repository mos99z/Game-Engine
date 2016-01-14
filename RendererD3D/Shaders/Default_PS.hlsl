#include "InputLayout.hlsli"
#include "CommonShaderFunctions.hlsli"
#include "Samplers.hlsli"

static const float2 g_SpecPowerRange = { 0.1, 250.0 };

PS_GBUFFER_OUT PackGBuffer(float3 BaseColor, float3 Normal, float SpecIntensity, float SpecPower)
{
	PS_GBUFFER_OUT Out;

	// Normalize the specular power
	float SpecPowerNorm = (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;

	// Pack all the data into the GBuffer structure
	Out.ColorSpecInt = float4(BaseColor.rgb, SpecIntensity);
	Out.Normal = float4(Normal.xyz * 0.5 + 0.5, 0.0);
	Out.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);

	return Out;
}


PS_GBUFFER_OUT main(VSOUT input)
{
	float3 DiffuseColor = input.color.xyz;
	float3 Normal = input.normal;
	float SpecIntensity = 0.8f;
	float SpecPower = 4;



	return PackGBuffer(DiffuseColor, normalize(Normal), SpecIntensity,SpecPower);
	//return float4(HemisphericAmbient(float3(0.0f,0.0f,0.0f),float3(1.0f,0.2f,0.2f),normalize(input.normal),float4(1.0f,1.0f,1.0f,1.0f)),1);
	//return input.color;
	//return map.Sample(anisoWrapSampler, input.color.xy);
	//return input.pos.z / input.pos.w;
	//return map.Sample(anisoWrapSampler, input.pos.z / input.pos.w);
}