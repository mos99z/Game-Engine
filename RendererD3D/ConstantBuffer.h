#ifndef CONSTANT_BUFFERS_H
#define CONSTANT_BUFFERS_H

#include "SharedDefines.h"


CONSTANT_BUFFER_BEGIN(cbPerObject, b0)
/// The concatenated world view projection matrices
float4x4	gMVP;
/// The world matrix of the object to be rendered.
float4x4	gWorld;
#ifdef __cplusplus
const static int REGISTER_SLOT = 0;
#endif
CONSTANT_BUFFER_END

CONSTANT_BUFFER_BEGIN(cbPerCamera, b1)
/// The current cameras current view projection concatenated matrix
float4x4	gViewProj;
/// The current cameras current inverse view-projection matrix
float4x4	gInvViewProj;


float4 PerspectiveValues;
float4x4 ViewInv;


/// The position of the camera, or eye.
float3 gCameraPos;
/// The distance of the far clip plane
float gFarDist;
// The size of the main render target in pixels
float2 gScreenSize;
/// Precomputed value used to convert depth to actual distance.
/// gPlaneX = -far / (far - near)
/// distance = gPlaneY / (gPlaneX + depth)
float gPlaneX;
/// Precomputed value used to convert depth to actual distance.
/// gPlaneY = -far * near/ (far - near)
/// distance = gPlaneY / (gPlaneX + depth)
float gPlaneY;
float3 gCameraDir;

#ifdef __cplusplus
const static int REGISTER_SLOT = 1;
#endif
CONSTANT_BUFFER_END


//Light buffers
CONSTANT_BUFFER_BEGIN(cbDirLight, b2)
float4 lightPos;
float4 DLightColor;

#ifdef __cplusplus
const static int REGISTER_SLOT = 2;
#endif
CONSTANT_BUFFER_END
#endif //CONSTANT_BUFFERS_H




