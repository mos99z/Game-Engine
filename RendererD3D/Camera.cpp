#include "stdafx.h"
#include "Camera.h"

namespace RendererD3D
{



	Camera::Camera() :up(0.0f, 1.0f, 0.0f), side(1.0f, 0.0f, 0.0f), forward(0.0f, 0.0f, -1.0f), position(0.0f, 25.0f, 100.0f)
	{

		XMFLOAT3 focus{ 0.0f, 0.0f,-1.0f };
		view = XMMatrixLookToLH(XMLoadFloat3(&position), XMLoadFloat3(&forward), XMLoadFloat3(&up));
		proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 16.0f/9.0f, 0.1f, 1000.0f);
	}


	Camera::~Camera()
	{
	}

	void Camera::UpdateView()
	{
		view = XMMatrixLookToLH(XMLoadFloat3(&position), XMLoadFloat3(&forward), XMLoadFloat3(&up));
	}

	void Camera::UpdateProjection(float _fovDegree, float _width, float _height, float _nPlane, float _fPlane)
	{
		proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(_fovDegree), _width / _height, _nPlane, _fPlane);
	}

	void Camera::Walk(float _speed)
	{
		XMVECTOR nForward = XMVector3Normalize(XMLoadFloat3(&forward));
		XMStoreFloat3(&position, XMVectorMultiplyAdd(XMVectorReplicate(_speed), nForward, XMLoadFloat3(&position)));
		UpdateView();
	}

	void Camera::Stafe(float _speed)
	{
		XMVECTOR nSide = XMVector3Normalize(XMLoadFloat3(&side));
		XMStoreFloat3(&position, XMVectorMultiplyAdd(XMVectorReplicate(_speed), nSide, XMLoadFloat3(&position)));
		UpdateView();
	}

	void Camera::Climb(float _speed)
	{
		XMFLOAT3 y{ 0.0f,1.0f,0.0f };
		XMStoreFloat3(&position, XMVectorMultiplyAdd(XMVectorReplicate(_speed), XMLoadFloat3(&y), XMLoadFloat3(&position)));
		UpdateView();
	}

	void Camera::Pitch(float _angleDegree)
	{
		XMVECTOR nUp = XMVector3Normalize(XMLoadFloat3(&up));
		XMVECTOR nSide = XMVector3Normalize(XMLoadFloat3(&side));
		XMVECTOR nForward = XMVector3Normalize(XMLoadFloat3(&forward));

		XMMATRIX rot = XMMatrixRotationAxis(nSide, XMConvertToRadians(_angleDegree));
		XMStoreFloat3(&up, XMVector3TransformNormal(nUp, rot));
		XMStoreFloat3(&forward, XMVector3TransformNormal(nForward, rot));
		UpdateView();
	}

	void Camera::RotateY(float _angleDegree)
	{
		XMVECTOR nUp = XMVector3Normalize(XMLoadFloat3(&up));
		XMVECTOR nSide = XMVector3Normalize(XMLoadFloat3(&side));
		XMVECTOR nForward = XMVector3Normalize(XMLoadFloat3(&forward));

		XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(_angleDegree));
		XMStoreFloat3(&up, XMVector3TransformNormal(nUp, rot));
		XMStoreFloat3(&side, XMVector3TransformNormal(nSide, rot));
		XMStoreFloat3(&forward, XMVector3TransformNormal(nForward, rot));
		UpdateView();
	}

	void Camera::WalkForward()
	{
		XMVECTOR nForward = XMVector3Normalize(XMLoadFloat3(&forward));
		XMStoreFloat3(&position, XMVectorMultiplyAdd(XMVectorReplicate(mSpeed * deltaTime), nForward, XMLoadFloat3(&position)));
		UpdateView();
	}
	void Camera::WalkBackward()
	{
		XMVECTOR nForward = XMVector3Normalize(XMLoadFloat3(&forward));
		XMStoreFloat3(&position, XMVectorMultiplyAdd(XMVectorReplicate(-mSpeed * deltaTime), nForward, XMLoadFloat3(&position)));
		UpdateView();
	}
	void Camera::StafeLeft()
	{
		XMVECTOR nSide = XMVector3Normalize(XMLoadFloat3(&side));
		XMStoreFloat3(&position, XMVectorMultiplyAdd(XMVectorReplicate(mSpeed * deltaTime), nSide, XMLoadFloat3(&position)));
		UpdateView();
	}
	void Camera::StafeRight()
	{
		XMVECTOR nSide = XMVector3Normalize(XMLoadFloat3(&side));
		XMStoreFloat3(&position, XMVectorMultiplyAdd(XMVectorReplicate(-mSpeed * deltaTime), nSide, XMLoadFloat3(&position)));
		UpdateView();
	}



}