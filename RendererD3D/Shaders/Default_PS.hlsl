#include "InputLayout.hlsli"
#include "CommonShaderFunctions.hlsli"
#include "Samplers.hlsli"
#include "..\ConstantBuffer.h"




float4 main(VEROUT_PosNorDiffUVTan input)  : SV_TARGET0
{
	float3 DiffuseColor = saturate(float4(HemisphericAmbient(float3(0.0f, 0.0f, 0.0f), float3(0.3f, 0.3f, 0.3f), normalize(input.normal), float4(1.0f, 1.0f, 1.0f, 1.0f)), 1) +
		map.Sample(anisoWrapSampler, float2(input.texcoord.x, 1.0f - input.texcoord.y))).xyz;
	Material m = (Material)0;
	m.normal = normalize(input.normal);
	m.diffuseColor = input.color;
	m.specExp = 1;
	m.specIntensity = 0;
	DiffuseColor += CalcDirColor(input.position.xyz, lightPos.xyz, DLightColor, gCameraPos, m);
	float3 Normal = input.normal;
	float SpecIntensity = 0.8f;
	float SpecPower = 4;



	return float4(DiffuseColor,1);
	//return input.color;
	//return map.Sample(anisoWrapSampler, input.color.xy);
	//return input.pos.z / input.pos.w;
	//return map.Sample(anisoWrapSampler, input.pos.z / input.pos.w);
}

