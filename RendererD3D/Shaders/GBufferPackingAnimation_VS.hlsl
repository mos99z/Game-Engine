#include "InputLayout.hlsli"
#include "..\ConstantBuffer.h"

VEROUT_PosNorDiffUVTan main(VERIN_PosNorDiffUVTanBoneWeight input)
{
    VEROUT_PosNorDiffUVTan output = (VEROUT_PosNorDiffUVTan) 0;
    float4x4 skinTransform = (float4x4) 0;
    skinTransform += gBones[input.bone.x] * input.weights.x;
    skinTransform += gBones[input.bone.y] * input.weights.y;
    skinTransform += gBones[input.bone.z] * input.weights.z;
    skinTransform += gBones[input.bone.w] * input.weights.w;
    output.position = mul(float4(input.position, 1), skinTransform);
    output.position = mul(output.position, gMVP);
    output.normal = mul(float4(input.normal, 0), gWorld).xyz;
    output.texcoord = input.texcoord;
    output.tangent = input.tangent;
    output.color = input.diffuse;
    return output;
}