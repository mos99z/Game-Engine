#pragma once
#include <DirectXMath.h>
using namespace DirectX;
namespace RendererD3D
{
	class Camera
	{
	private:
		XMFLOAT3 up;
		XMFLOAT3 side;
		XMFLOAT3 forward;
		XMFLOAT3 position;
		XMMATRIX view;
		XMMATRIX proj;
	public:
		Camera();
		~Camera();

		//Getter
		inline XMFLOAT3 GetUp(void) const { return up; }
		inline XMFLOAT3 GetSide(void) const { return side; }
		inline XMFLOAT3 GetForward(void) const { return forward; }
		inline XMFLOAT3 GetPosition(void) const { return position; }
		inline XMMATRIX GetView(void) const { return view; }
		inline XMMATRIX GetProj(void) const { return proj; }

		//Setter
		inline void SetUp(XMFLOAT3 _newUp) { up = _newUp; }
		inline void SetSide(XMFLOAT3 _newSide) { side = _newSide; }
		inline void SetForward(XMFLOAT3 _newForward) { forward = _newForward; }
		inline void SetPosition(XMFLOAT3 _newPos) { position = _newPos; }

		void UpdateView();
		void UpdateProjection(float _fovDegree, float _width, float _height, float _nPlane, float _fPlane);


		//Transformation 
		void Walk(float _speed);
		void Stafe(float _speed);
		void Climb(float _speed);
		void Pitch(float _angleDegree);
		void RotateY(float _angleDegree);



	};

}