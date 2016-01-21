#include "InputLayout.hlsli"
#include "..\ConstantBuffer.h"



static const float2 arrBasePos[4] = {
	/*float2(0.0, 0.0),
	float2(1.0, 0.0),
	float2(1.0, 1.0),
	float2(0.0, 1.0),
*/

	float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, -1.0),
};
VEROUT_PosUV main(uint VertexID : SV_VertexID)
{
	VEROUT_PosUV Output;
	//Output.Position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
	//Output.UV = Output.Position.xy;
	//Output.UV.y = 1.0f - Output.UV.y;
	Output.UV = float2((VertexID << 1) & 2, VertexID & 2);
	Output.Position = float4(Output.UV * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
	return Output;
}