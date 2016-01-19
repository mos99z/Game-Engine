#ifndef SAMPLERS_HLSLI
#define SAMPLERS_HLSLI
Texture2D map					: register(t0);
Texture2D DepthMap				: register(t1);
Texture2D ColorSpecIntMap		: register(t2);
Texture2D NormalMap				: register(t3);
Texture2D SpecPowMap			: register(t4);
SamplerState pointSampler				: register(s0);
SamplerState anisoWrapSampler			: register(s1);
SamplerState anisoClampSampler			: register(s2);
#endif //SAMPLERS_HLSLI