#include "Camera.h"

Camera::Camera(DirectX::XMVECTOR inCamPos, DirectX::XMVECTOR inLookAt, DirectX::XMVECTOR inUp, float inFov, float inAspectRatio,
	float inNearPlane, float inFarPlane)
{
	this->camPos = inCamPos;
	this->lookAt = inLookAt;
	this->up = inUp;
	this->fov = inFov;
	this->aspectRatio = inAspectRatio;
	this->nearPlane = inNearPlane;
	this->farPlane = inFarPlane;
}

Camera::Camera()
{
	float PI = 3.1415;

	this->camPos = DirectX::XMVectorSet(0, 2, -2.8, 1);
	this->lookAt = DirectX::XMVectorSet(0, 0, 0, 1);
	this->up = DirectX::XMVectorSet(0, 1, 0, 1);
	this->fov = PI * 0.45;
	this->aspectRatio = 1280 / 720;
	this->nearPlane = 0.1;
	this->farPlane = 20.0;
}

Camera::~Camera()
{
}

void Camera::setCamProperties(DirectX::XMVECTOR inCamPos, DirectX::XMVECTOR inLookAt, DirectX::XMVECTOR inUp, float inFov)
{
	this->camPos = inCamPos;
	this->lookAt = inLookAt;
	this->up = inUp;
	this->fov = inFov;
}

DirectX::XMVECTOR Camera::getCamPos() const
{
	return this->camPos;
}

DirectX::XMVECTOR Camera::getLookAt() const
{
	return this->lookAt;
}

DirectX::XMVECTOR Camera::getUp() const
{
	return this->up;
}

float Camera::getFov() const
{
	return this->fov;
}

float Camera::getAspectRatio() const
{
	return this->aspectRatio;
}

float Camera::getNearPlane() const
{
	return this->nearPlane;
}

float Camera::getFarPlane() const
{
	return this->farPlane;
}
