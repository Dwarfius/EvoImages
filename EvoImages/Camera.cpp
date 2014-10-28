#include "Camera.h"

Camera::Camera()
{
	maxPitch = D3DXToRadian(89.0f);
	maxVelocity = 1;
	invertY = false;
	enableYMovement = true;
	position = D3DXVECTOR3(0, 0, 0);
	velocity = D3DXVECTOR3(0, 0, 0);
	forward = D3DXVECTOR3(0, 0, 1);
	CreateProjectionMatrix(D3DX_PI / 3.0f, 1.3f, 0.1f, 1000.0f);
	Update();
}

void Camera::CreateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane)
{
	this->fov = fov;
	this->aspect = aspect;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	D3DXMatrixPerspectiveFovLH(&projection, fov, aspect, nearPlane, farPlane);
}

void Camera::MoveForward(float amt)
{
	if(enableYMovement)
		velocity += amt * forward;
	else
	{
		D3DXVECTOR3 moveVector(forward.x, 0, forward.z);
		velocity += *D3DXVec3Normalize(&moveVector, &moveVector) * amt;
	}
}

void Camera::Strafe(float amt)
{
	velocity += amt * right;
}

void Camera::MoveUp(float amt)
{
	if(enableYMovement)
		velocity.y += amt;
}

//rotation around Y axis
void Camera::Yaw(float rad)
{
	if(rad == 0.0f)
		return;

	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &up, rad);
	D3DXVec3TransformNormal(&right, &right, &rotation);
	D3DXVec3TransformNormal(&forward, &forward, &rotation);
}

//rotation around X axis
void Camera::Pitch(float rad)
{
	if(rad == 0.0f)
		return;

	rad = invertY ? -rad : rad; //inversion
	pitch -= rad;
	if(pitch > maxPitch) //holding it in the [-maxPitch..maxPitch] range
		rad += pitch - maxPitch;
	else if(pitch < -maxPitch)
		rad += pitch + maxPitch;

	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &right, rad);
	D3DXVec3TransformNormal(&up, &up, &rotation);
	D3DXVec3TransformNormal(&forward, &forward, &rotation);
}

//rotation around Z axis
void Camera::Roll(float rad)
{
	if(rad == 0.0f)
		return;

	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &forward, rad);
	D3DXVec3TransformNormal(&up, &up, &rotation);
	D3DXVec3TransformNormal(&right, &right, &rotation);
}

void Camera::Update()
{
	if(D3DXVec3Length(&velocity) > maxVelocity)
		velocity = *D3DXVec3Normalize(&velocity, &velocity) * maxVelocity;

	//translating
	position += velocity;
	velocity = D3DXVECTOR3(0, 0, 0);
	lookAt = position + forward;

	//creating a new view matrix
	D3DXMatrixLookAtLH(&view, &position, &lookAt, &D3DXVECTOR3(0, 1, 0));

	//setting up axes from computed view matrix
	right.x = view._11;
	right.y = view._21;
	right.z = view._31;
	up.x = view._12;
	up.y = view._22;
	up.z = view._32;
	forward.x = view._13;
	forward.y = view._23;
	forward.z = view._33;
	
	//calculating yaw and pitch
	float lookLengthOnXZ = sqrtf(forward.x * forward.x + forward.z * forward.z);
	pitch = atan2(forward.y, lookLengthOnXZ);
	yaw = atan2(forward.x, forward.z);
}

void Camera::SetPos(D3DXVECTOR3 *pos)
{
	position.x = pos->x;
	position.y = pos->y;
	position.z = pos->z;
}

void Camera::LookAt(D3DXVECTOR3 *target)
{
	lookAt.x = target->x;
	lookAt.y = target->y;
	lookAt.z = target->z;
	D3DXVec3Normalize(&forward, &(lookAt - position));
}