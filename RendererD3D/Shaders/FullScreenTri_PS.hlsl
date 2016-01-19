#include "Samplers.hlsli"


float4 main(float2 uv: TexCoord) : SV_TARGET
{
	return map.Sample(anisoWrapSampler, uv);
}