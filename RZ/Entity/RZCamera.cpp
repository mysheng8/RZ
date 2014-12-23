#include "RZCamera.h"

using namespace RZ;

RZCameraBase::RZCameraBase()  
{  
    m_positionX = 0.0f;  
    m_positionY = 0.0f;  
    m_positionZ = -10.0f;  
  
    m_rotationX = 0.0f;  
    m_rotationY = 0.0f;  
    m_rotationZ = 0.0f;  
}

RZCameraBase::RZCameraBase(const RZCameraBase& other)  
{  
}  
  
  
RZCameraBase::~RZCameraBase()  
{  
}

void RZCameraBase::SetPosition(float x, float y, float z)  
{  
    m_positionX = x;  
    m_positionY = y;  
    m_positionZ = z;  
    return;  
}

void RZCameraBase::SetRotation(float x, float y, float z)  
{  
    m_rotationX = x;  
    m_rotationY = y;  
    m_rotationZ = z;  
    return;  
}

XMFLOAT3 RZCameraBase::GetPosition()  
{  
    return XMFLOAT3(m_positionX, m_positionY, m_positionZ);  
}

XMFLOAT3 RZCameraBase::GetRotation()  
{  
    return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);  
}

RZSimpleCamera::RZSimpleCamera(InputClass *input)
{
	m_pInput=input;
	m_camMoveSpeed=0.008f;
	m_camRotSpeed=0.5f;
	m_focusDistance=10.0f;
}


void RZSimpleCamera::UpdateTransform()
{
	if(m_pInput->IsMapKeyDown(RESET))
	{
		XMVECTOR lookAt; 
		XMMATRIX rotationMatrix;
		float yaw, pitch, roll;

		lookAt=XMVectorSet(0.0f,0.0f,m_focusDistance,0.0f);

		pitch = m_rotationX * 0.0174532925f;  
		yaw   = m_rotationY * 0.0174532925f;  
		roll  = m_rotationZ * 0.0174532925f;

		rotationMatrix=XMMatrixRotationRollPitchYaw(yaw, pitch, roll); 
		lookAt=XMVector3TransformCoord(lookAt, rotationMatrix);

		m_positionX = -XMVectorGetX(lookAt);  
		m_positionY = -XMVectorGetY(lookAt);  
		m_positionZ = -XMVectorGetZ(lookAt); 	
	}
	if(m_pInput->IsMapKeyDown(SHIFTDOWN))
	{
		XMVECTOR Move; 
		XMMATRIX rotationMatrix;
		float yaw, pitch, roll;
		Move=XMVectorSet(0.0f,0.0f,0.0f,0.0f);
		
		if(m_pInput->IsMapKeyDown(MOVE_FORWARD))
			Move+=XMVectorSet(0.0f,0.0f,1.0f,0.0f);
		if(m_pInput->IsMapKeyDown(MOVE_BACKWARD))
			Move+=XMVectorSet(0.0f,0.0f,-1.0f,0.0f);
		if(m_pInput->IsMapKeyDown(MOVE_UP))
			Move+=XMVectorSet(0.0f,1.0f,0.0f,0.0f);
		if(m_pInput->IsMapKeyDown(MOVE_DOWN))
			Move+=XMVectorSet(0.0f,-1.0f,0.0f,0.0f);
		if(m_pInput->IsMapKeyDown(MOVE_RIGHT))
			Move+=XMVectorSet(1.0f,0.0f,0.0f,0.0f);
		if(m_pInput->IsMapKeyDown(MOVE_LEFT))
			Move+=XMVectorSet(-1.0f,0.0f,0.0f,0.0f);

		// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.  
		pitch = m_rotationX * 0.0174532925f;  
		yaw   = m_rotationY * 0.0174532925f;  
		roll  = m_rotationZ * 0.0174532925f;  
  
		// Create the rotation matrix from the yaw, pitch, and roll values.  
		rotationMatrix=XMMatrixRotationRollPitchYaw(yaw, pitch, roll); 
		Move=XMVector3TransformCoord(Move, rotationMatrix);
 

		m_positionX += XMVectorGetX(Move);  
		m_positionY += XMVectorGetY(Move);  
		m_positionZ += XMVectorGetZ(Move);  

	}
	if(m_pInput->IsMapKeyDown(ALTDOWN))
	{
		MoveDelta delta=m_pInput->GetMouseDelta();
		if(m_pInput->IsMouseMButtonDown())
		{

			XMVECTOR Move; 
			XMMATRIX rotationMatrix;
			float yaw, pitch, roll;

			Move=XMVectorSet(-delta.x*m_camMoveSpeed,delta.y*m_camMoveSpeed,0.0f,0.0f);
		
			// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.  
			pitch = m_rotationX * 0.0174532925f;  
			yaw   = m_rotationY * 0.0174532925f;  
			roll  = m_rotationZ * 0.0174532925f;  
  
			// Create the rotation matrix from the yaw, pitch, and roll values.
			rotationMatrix=XMMatrixRotationRollPitchYaw(yaw, pitch, roll); 
			Move=XMVector3TransformCoord(Move, rotationMatrix);

			m_positionX += XMVectorGetX(Move);  
			m_positionY += XMVectorGetY(Move);  
			m_positionZ += XMVectorGetZ(Move);  
		}
		if(m_pInput->IsMouseLButtonDown())
		{
			XMVECTOR lookAt,position,localPos;
			XMMATRIX rotationMatrix;
			float yaw, pitch, roll;

			lookAt=XMVectorSet(0.0f,0.0f,m_focusDistance,0.0f);
			position=XMVectorSet(m_positionX,m_positionY,m_positionZ,0);

			pitch = m_rotationX * 0.0174532925f;  
			yaw   = m_rotationY * 0.0174532925f;  
			roll  = m_rotationZ * 0.0174532925f;  
  
			// Create the rotation matrix from the yaw, pitch, and roll values. 
			rotationMatrix=XMMatrixRotationRollPitchYaw(yaw, pitch, roll); 
			lookAt=XMVector3TransformCoord(lookAt, rotationMatrix);


			localPos = position + lookAt;

			lookAt=XMVectorSet(0.0f,0.0f,m_focusDistance,0.0f);

			m_rotationY+=delta.x*m_camRotSpeed;
			m_rotationX+=delta.y*m_camRotSpeed;

			pitch = m_rotationX * 0.0174532925f;  
			yaw   = m_rotationY * 0.0174532925f;  
			roll  = m_rotationZ * 0.0174532925f;

			rotationMatrix=XMMatrixRotationRollPitchYaw(yaw, pitch, roll); 
			lookAt=XMVector3TransformCoord(lookAt, rotationMatrix);

			position = localPos-lookAt;

			m_positionX = XMVectorGetX(position);  
			m_positionY = XMVectorGetY(position);  
			m_positionZ = XMVectorGetZ(position); 
		}
		if(m_pInput->IsMouseRButtonDown())
		{
			XMVECTOR Move; 
			XMMATRIX rotationMatrix;
			float yaw, pitch, roll;
			Move=XMVectorSet(0.0f,0.0f,(delta.y)*m_camMoveSpeed,0.0f);
 
			m_focusDistance+=(-delta.y)*m_camMoveSpeed;
			// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.  
			pitch = m_rotationX * 0.0174532925f;  
			yaw   = m_rotationY * 0.0174532925f;  
			roll  = m_rotationZ * 0.0174532925f;  
  
			// Create the rotation matrix from the yaw, pitch, and roll values.  
			rotationMatrix=XMMatrixRotationRollPitchYaw(yaw, pitch, roll); 
			Move=XMVector3TransformCoord(Move, rotationMatrix); 

			m_positionX += XMVectorGetX(Move);  
			m_positionY += XMVectorGetY(Move);  
			m_positionZ += XMVectorGetZ(Move); 
		}
	}
}



void RZSimpleCamera::Render()  
{
    XMVECTOR up, position, lookAt;  
    float yaw, pitch, roll;  
    XMMATRIX rotationMatrix;  
  
  
    // Setup the vector that points upwards. 
	up=XMVectorSet(0.0f,1.0f,0.0f,0.0f);
  
    // Setup the position of the camera in the world.  
	position=XMVectorSet(m_positionX,m_positionY,m_positionZ,0.0f);  
  
    // Setup where the camera is looking by default.
	lookAt=XMVectorSet(0.0f,0.0f,m_focusDistance,0.0f);
  
  
    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.  
    pitch = m_rotationX * 0.0174532925f;  
    yaw   = m_rotationY * 0.0174532925f;  
    roll  = m_rotationZ * 0.0174532925f;  
  
    // Create the rotation matrix from the yaw, pitch, and roll values.  
    rotationMatrix=XMMatrixRotationRollPitchYaw(yaw, pitch, roll);  
  
    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.  
    lookAt=XMVector3TransformCoord(lookAt, rotationMatrix);  
    up=XMVector3TransformCoord(up, rotationMatrix);  
  
    // Translate the rotated camera position to the location of the viewer.  
    lookAt = position + lookAt;  
  
    // Finally create the view matrix from the three updated vectors.
	XMStoreFloat4x4(&m_viewMatrix,XMMatrixLookAtLH(position, lookAt, up));
  
    return;  
}

void RZSimpleCamera::GetViewMatrix(XMMATRIX& viewMatrix)  
{  
    viewMatrix = XMLoadFloat4x4(&m_viewMatrix);  
    return;  
}
