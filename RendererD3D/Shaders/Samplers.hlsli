#ifndef SAMPLERS_HLSLI
#define SAMPLERS_HLSLI
Texture2D map			: register(t0);
SamplerState anisoWrapSampler						: register(s0);
SamplerState anisoClampSampler						: register(s1);
#endif //SAMPLERS_HLSLI