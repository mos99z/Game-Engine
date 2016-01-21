#pragma once
#ifdef CAMERADLL_EXPORTS
#define CAMERADLL_API __declspec(dllexport) 
#else
#define CAMERADLL_API __declspec(dllimport) 
#endif
#include <DirectXMath.h>
using namespace DirectX;
namespace RendererD3D
{
	class  Camera
	{
	private:
		XMFLOAT3 up;
		XMFLOAT3 side;
		XMFLOAT3 forward;
		XMFLOAT3 position;
		XMMATRIX view;
		XMMATRIX proj;
		float mSpeed = 1000.0f; //100000.0f;
		float deltaTime = 1.0f / 60.0f;
	public:
		Camera();
		~Camera();

		//Getter
		inline XMFLOAT3 GetUp(void) { return up; }
		inline XMFLOAT3 GetSide(void) { return side; }
		inline XMFLOAT3 GetForward(void) { return forward; }
		inline XMFLOAT3 GetPosition(void) { return position; }
		inline XMMATRIX GetView(void) { return view; }
		inline XMMATRIX GetProj(void) { return proj; }

		//Setter
		inline void SetUp(XMFLOAT3 _newUp) { up = _newUp; }
		inline void SetSide(XMFLOAT3 _newSide) { side = _newSide; }
		inline void SetForward(XMFLOAT3 _newForward) { forward = _newForward; }
		inline void SetPosition(XMFLOAT3 _newPos) { position = _newPos; }

		CAMERADLL_API void UpdateView();
		CAMERADLL_API void UpdateProjection(float _fovDegree, float _width, float _height, float _nPlane, float _fPlane);
		CAMERADLL_API inline void SetDeltaTime(float dt) { deltaTime = dt; }

		//Transformation 
		CAMERADLL_API void Walk(float _speed);
		CAMERADLL_API void Stafe(float _speed);
		CAMERADLL_API void WalkForward();
		CAMERADLL_API void WalkBackward();
		CAMERADLL_API void StafeLeft();
		CAMERADLL_API void StafeRight();
		CAMERADLL_API void Climb(float _speed);
		CAMERADLL_API void Pitch(float _angleDegree);
		CAMERADLL_API void RotateY(float _angleDegree);



	};

}