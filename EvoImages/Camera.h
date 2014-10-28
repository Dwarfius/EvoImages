#pragma once

#include "GlobalHeader.h"

class Camera
{
public:
	Camera();
	void CreateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane);
	void MoveForward(float amt);
	void Strafe(float amt);
	void MoveUp(float amt);

	void Yaw(float rad);
	void Pitch(float rad);
	void Roll(float rad);
	void Update();

	void SetPos(D3DXVECTOR3 *pos);
	void LookAt(D3DXVECTOR3 *target);
	void SetFOV(float fov) { CreateProjectionMatrix(fov, aspect, nearPlane, farPlane); };
	void SetAspectRatio(float aspect) { CreateProjectionMatrix(fov, aspect, nearPlane, farPlane); };
	void SetNearPlane(float nearPlane) { CreateProjectionMatrix(fov, aspect, nearPlane, farPlane); };
	void SetFarPlane(float farPlane) { CreateProjectionMatrix(fov, aspect, nearPlane, farPlane); };
	void SetMaxVelocity(float maxVelocity) { this->maxVelocity = maxVelocity; };
	void SetInvertY(bool invert) { invertY = invert; };
	void EnableYMovement(bool enable) { enableYMovement = enable; };

	D3DXMATRIX* GetViewMatrix() { return &view; };
	D3DXMATRIX* GetProjMatrix() { return &projection; };
	D3DXVECTOR3* GetPosition() { return &position; };
	D3DXVECTOR3* GetLookAt() { return &lookAt; };
	float GetFOV() { return fov; };
	float GetAspectRatio() { return aspect; };
	float GetNearPlane() { return nearPlane; };
	float GetFarPlane() { return farPlane; };
	float GetMaxVelocity() { return maxVelocity; };
	bool GetInvertY() { return invertY; };
	float GetPitch() { return pitch; };
	float GetYaw() { return yaw; };
	float GetMaxPitch() { return maxPitch; };

private:
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 forward;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 velocity;
	float yaw;
	float pitch;
	float maxPitch;
	float maxVelocity;
	float fov;
	float aspect;
	float nearPlane;
	float farPlane;
	bool invertY;
	bool enableYMovement;
};