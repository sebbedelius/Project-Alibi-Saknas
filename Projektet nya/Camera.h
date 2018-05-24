#ifndef CAMERA_H
#define CAMERA_H
#include <d3d11.h>
#include<DirectXMath.h>
class Camera
{
private:
	DirectX::XMVECTOR camPos;
	DirectX::XMVECTOR lookAt;
	DirectX::XMVECTOR up;
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

public:
	Camera(DirectX::XMVECTOR inCamPos, DirectX::XMVECTOR inLookAt, DirectX::XMVECTOR inUp, float inFov, float inAspectRatio,
		float inNearPlane, float inFarPlane);
	Camera();
	~Camera();
	void setCamProperties(DirectX::XMVECTOR inCamPos, DirectX::XMVECTOR inLookAt, DirectX::XMVECTOR inUp, float inFov);

	DirectX::XMVECTOR getCamPos() const;
	DirectX::XMVECTOR getLookAt() const;
	DirectX::XMVECTOR getUp() const;
	float getFov() const;
	float getAspectRatio() const;
	float getNearPlane() const;
	float getFarPlane() const;

};

#endif
