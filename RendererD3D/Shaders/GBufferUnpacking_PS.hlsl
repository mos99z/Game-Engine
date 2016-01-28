#include "InputLayout.hlsli"
#include "CommonShaderFunctions.hlsli"
#include "Samplers.hlsli"
#include "..\ConstantBuffer.h"


float3 CalcWorldPos(float2 csPos, float linearDepth)
{
	float4 position;
	position.xy = csPos.xy * PerspectiveValues.xy * linearDepth;
	position.z = linearDepth;
	position.w = 1.0;
	return mul(position, ViewInv).xyz;
}

float ConvertDepthToLinear(float depth)
{
	float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w);
	return linearDepth;
}

static const float2 g_SpecPowerRange = { 0.1, 250.0 };

SURFACE_DATA UnpackGBuffer(float2 location)
{
	SURFACE_DATA Out;
	// Get the depth value and convert it to linear depth
	float depth = DepthMap.Sample(pointSampler, location).x;
	Out.LinearDepth = ConvertDepthToLinear(depth);

	// Get the base color and specular intensity
	float4 baseColorSpecInt = ColorSpecIntMap.Sample(pointSampler, location);
	Out.Color = baseColorSpecInt.xyz;
	Out.SpecInt = baseColorSpecInt.w;

	// Sample the normal, convert it to the full range and noramalize it
	Out.Normal = NormalMap.Sample(pointSampler, location).xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	// Scale the specular power back to the original range
	float SpecPowerNorm = SpecPowMap.Sample(pointSampler, location).x;
	Out.SpecPow = SpecPowerNorm.x + SpecPowerNorm * g_SpecPowerRange.y;

	return Out;
}






float4 main(VEROUT_PosUV input) : SV_TARGET0
{

	SURFACE_DATA data = UnpackGBuffer(input.UV);

	Material mat;
	mat.normal = data.Normal;
	mat.diffuseColor.xyz = data.Color;
	mat.diffuseColor.w = 1.0; 
	mat.specExp = 1;
	mat.specIntensity =0;
	
	float3 position = CalcWorldPos(input.UV, data.LinearDepth);
	
	// Calculate the ambient and directional light contributions
    float4 finalColor = mat.diffuseColor;
	finalColor.xyz *= HemisphericAmbient(float3(0.1f, 0.1f, 0.1f), float3(0.5f, 0.5f, 0.5f), normalize(mat.normal), float4(1.0f, 1.0f, 1.0f, 1.0f));
	finalColor.xyz += CalcDirColor(position.xyz, lightPos.xyz, DLightColor, gCameraPos, mat);

	
	return saturate(finalColor);
}