#include "InputLayout.hlsli"
#include "..\ConstantBuffer.h"




VEROUT_PosUV main(uint VertexID : SV_VertexID)
{
	VEROUT_PosUV Output;
	Output.UV = float2((VertexID << 1) & 2, VertexID & 2);
	Output.Position = float4(Output.UV * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
	return Output;
}