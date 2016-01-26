#pragma once

namespace RendererD3D
{
	class ABone;
	class AKeyframe
	{
		float timeStamp = 0.0f;
		unsigned int numOfbones = 0;
		ABone* bones = nullptr;
	public:
		AKeyframe();
		~AKeyframe();

		inline float TimeStamp() const { return timeStamp; }
		inline ABone* Bones() const { return bones; }
		inline unsigned int NumberOfBones() const { return numOfbones; }
	};
}
