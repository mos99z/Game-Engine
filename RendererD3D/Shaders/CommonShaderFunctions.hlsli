#include "../ConstantBuffer.h"
#include "InputLayout.hlsli"
#include "Samplers.hlsli"

float3 HemisphericAmbient(float3 ambientDown, float3 ambientRange, float3 normal, float4 lightColor)
{
	normal = normalize(normal);
	lightColor = float4(lightColor.rgb * lightColor.rgb, lightColor.a);
	float up = normal.y * 0.5 + 0.5;
	float3 Ambient = ambientDown + up * (ambientRange + ambientDown);
	return Ambient * lightColor.rgb;
}

float3 CalcDirColor(float3 pos, float3 lightDir,float4 DLightColor,float3 cameraPos, Material material)
{
	// Diffuse
	material.normal = normalize(material.normal);

	float3 DirToLight = normalize(lightDir);
	float NDotL = dot(DirToLight, material.normal);
	float3 finalColor = DLightColor.rgb * saturate(NDotL);

	//Specular
	float3 ToCamera = normalize(cameraPos - pos);
	float3 HalfWay = normalize(ToCamera + DirToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += DLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;

	return finalColor * material.diffuseColor.rgb;
}


float3 CalcPointColor(float3 pos, float3 PLightPos, float PointLightRangeRcp, float4 PLightColor, float3 cameraPos, Material material)
{
	float3 ToLight = PLightPos - pos;
	float DistToLight = length(ToLight);
	ToLight = normalize(ToLight);
	// Diffuse
	material.normal = normalize(material.normal);
	float NDotL = dot(ToLight, material.normal);
	float3 finalColor = PLightColor.rgb * saturate(NDotL);

	//Specular
	float3 ToCamera = normalize(cameraPos - pos);
	float3 HalfWay = normalize(ToCamera + ToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += PLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;

	// Attenuation
	float DistToLightNorm = 1.0 - saturate(DistToLight * PointLightRangeRcp);
	float Attenuation = DistToLightNorm * DistToLightNorm;
	
	return finalColor * material.diffuseColor.rgb * Attenuation;
}




