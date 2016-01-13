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
		CAMERADLL_API static XMFLOAT3 up;
		CAMERADLL_API static XMFLOAT3 side;
		CAMERADLL_API static XMFLOAT3 forward;
		CAMERADLL_API static XMFLOAT3 position;
		CAMERADLL_API static XMMATRIX view;
		CAMERADLL_API static XMMATRIX proj;
	public:
		 Camera();
		 ~Camera();

		//Getter
		 static inline XMFLOAT3 GetUp(void)  { return up; }
		 static inline XMFLOAT3 GetSide(void)  { return side; }
		 static inline XMFLOAT3 GetForward(void)  { return forward; }
		 static inline XMFLOAT3 GetPosition(void)  { return position; }
		 static inline XMMATRIX GetView(void)  { return view; }
		 static inline XMMATRIX GetProj(void)  { return proj; }

		//Setter
		 static inline void SetUp(XMFLOAT3 _newUp) { up = _newUp; }
		 static inline void SetSide(XMFLOAT3 _newSide) { side = _newSide; }
		 static inline void SetForward(XMFLOAT3 _newForward) { forward = _newForward; }
		 static inline void SetPosition(XMFLOAT3 _newPos) { position = _newPos; }

		 CAMERADLL_API static void UpdateView();
		 static void UpdateProjection(float _fovDegree, float _width, float _height, float _nPlane, float _fPlane);


		//Transformation 
		CAMERADLL_API static void Walk(float _speed);
		CAMERADLL_API static void Stafe(float _speed);
		CAMERADLL_API static void Climb(float _speed);
		CAMERADLL_API static void Pitch(float _angleDegree);
		CAMERADLL_API static void RotateY(float _angleDegree);



	};

}