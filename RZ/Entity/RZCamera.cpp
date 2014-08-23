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

D3DXVECTOR3 RZCameraBase::GetPosition()  
{  
    return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);  
}

D3DXVECTOR3 RZCameraBase::GetRotation()  
{  
    return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);  
}

RZSimpleCamera::RZSimpleCamera(InputClass *input)
{
	m_pInput=input;
	m_camMoveSpeed=0.01f;
	m_camRotSpeed=0.5f;
	m_focusDistance=10.0f;
}


void RZSimpleCamera::UpdateTransform()
{
	if(m_pInput->IsMapKeyDown(RESET))
	{
		D3DXVECTOR3 lookAt; 
		D3DXMATRIX rotationMatrix;
		float yaw, pitch, roll;

		lookAt.x = 0.0f;  
		lookAt.y = 0.0f;  
		lookAt.z = m_focusDistance;

		pitch = m_rotationX * 0.0174532925f;  
		yaw   = m_rotationY * 0.0174532925f;  
		roll  = m_rotationZ * 0.0174532925f;

		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll); 
		D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);

		m_positionX = -lookAt.x;  
		m_positionY = -lookAt.y;  
		m_positionZ = -lookAt.z; 	
	}
	if(m_pInput->IsMapKeyDown(SHIFTDOWN))
	{
		D3DXVECTOR3 Move; 
		D3DXMATRIX rotationMatrix;
		float yaw, pitch, roll;

		Move.x = 0.0f;  
		Move.y = 0.0f;  
		Move.z = 0.0f; 
		
		if(m_pInput->IsMapKeyDown(MOVE_FORWARD))
			Move.z+=0.1;
		if(m_pInput->IsMapKeyDown(MOVE_BACKWARD))
			Move.z+=-0.1;
		if(m_pInput->IsMapKeyDown(MOVE_UP))
			Move.y+=0.1;
		if(m_pInput->IsMapKeyDown(MOVE_DOWN))
			Move.y+=-0.1;
		if(m_pInput->IsMapKeyDown(MOVE_RIGHT))
			Move.x+=0.1;
		if(m_pInput->IsMapKeyDown(MOVE_LEFT))
			Move.x+=-0.1;

		// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.  
		pitch = m_rotationX * 0.0174532925f;  
		yaw   = m_rotationY * 0.0174532925f;  
		roll  = m_rotationZ * 0.0174532925f;  
  
		// Create the rotation matrix from the yaw, pitch, and roll values.  
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll); 
		D3DXVec3TransformCoord(&Move, &Move, &rotationMatrix); 

		m_positionX += Move.x;  
		m_positionY += Move.y;  
		m_positionZ += Move.z;  

	}
	if(m_pInput->IsMapKeyDown(ALTDOWN))
	{
		MoveDelta delta=m_pInput->GetMouseDelta();
		if(m_pInput->IsMouseMButtonDown())
		{

			D3DXVECTOR3 Move; 
			D3DXMATRIX rotationMatrix;
			float yaw, pitch, roll;

			Move.x = -delta.x*m_camMoveSpeed;  
			Move.y = delta.y*m_camMoveSpeed;  
			Move.z = 0.0f; 
		
			// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.  
			pitch = m_rotationX * 0.0174532925f;  
			yaw   = m_rotationY * 0.0174532925f;  
			roll  = m_rotationZ * 0.0174532925f;  
  
			// Create the rotation matrix from the yaw, pitch, and roll values.  
			D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll); 
			D3DXVec3TransformCoord(&Move, &Move, &rotationMatrix); 

			m_positionX += Move.x;  
			m_positionY += Move.y;  
			m_positionZ += Move.z;  
		}
		if(m_pInput->IsMouseLButtonDown())
		{
			D3DXVECTOR3 lookAt,position,localPos;
			D3DXMATRIX rotationMatrix;
			float yaw, pitch, roll;

			lookAt.x = 0.0f;  
			lookAt.y = 0.0f;  
			lookAt.z = m_focusDistance;

			position.x = m_positionX;  
			position.y = m_positionY;  
			position.z = m_positionZ; 

			pitch = m_rotationX * 0.0174532925f;  
			yaw   = m_rotationY * 0.0174532925f;  
			roll  = m_rotationZ * 0.0174532925f;  
  
			// Create the rotation matrix from the yaw, pitch, and roll values.  
			D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll); 
			D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);

			localPos = position + lookAt;

			lookAt.x = 0.0f;  
			lookAt.y = 0.0f;  
			lookAt.z = m_focusDistance;

			m_rotationY+=delta.x*m_camRotSpeed;
			m_rotationX+=delta.y*m_camRotSpeed;

			pitch = m_rotationX * 0.0174532925f;  
			yaw   = m_rotationY * 0.0174532925f;  
			roll  = m_rotationZ * 0.0174532925f;

			D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll); 
			D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);

			position = localPos-lookAt;

			m_positionX = position.x;  
			m_positionY = position.y;  
			m_positionZ = position.z; 
		}
		if(m_pInput->IsMouseRButtonDown())
		{
			D3DXVECTOR3 Move; 
			D3DXMATRIX rotationMatrix;
			float yaw, pitch, roll;

			Move.x = 0;  
			Move.y = 0; 
			Move.z = (delta.y)*m_camMoveSpeed; 
			m_focusDistance+=(-delta.y)*m_camMoveSpeed;
			// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.  
			pitch = m_rotationX * 0.0174532925f;  
			yaw   = m_rotationY * 0.0174532925f;  
			roll  = m_rotationZ * 0.0174532925f;  
  
			// Create the rotation matrix from the yaw, pitch, and roll values.  
			D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll); 
			D3DXVec3TransformCoord(&Move, &Move, &rotationMatrix); 

			m_positionX += Move.x;  
			m_positionY += Move.y;  
			m_positionZ += Move.z; 
		}
	}
}



void RZSimpleCamera::Render()  
{
    D3DXVECTOR3 up, position, lookAt;  
    float yaw, pitch, roll;  
    D3DXMATRIX rotationMatrix;  
  
  
    // Setup the vector that points upwards.  
    up.x = 0.0f;  
    up.y = 1.0f;  
    up.z = 0.0f;  
  
    // Setup the position of the camera in the world.  
    position.x = m_positionX;  
    position.y = m_positionY;  
    position.z = m_positionZ;  
  
    // Setup where the camera is looking by default.  
    lookAt.x = 0.0f;  
    lookAt.y = 0.0f;  
    lookAt.z = m_focusDistance;  
  
    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.  
    pitch = m_rotationX * 0.0174532925f;  
    yaw   = m_rotationY * 0.0174532925f;  
    roll  = m_rotationZ * 0.0174532925f;  
  
    // Create the rotation matrix from the yaw, pitch, and roll values.  
    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);  
  
    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.  
    D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);  
    D3DXVec3TransformCoord(&up, &up, &rotationMatrix);  
  
    // Translate the rotated camera position to the location of the viewer.  
    lookAt = position + lookAt;  
  
    // Finally create the view matrix from the three updated vectors.  
    D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);  
  
    return;  
}

void RZSimpleCamera::GetViewMatrix(D3DXMATRIX& viewMatrix)  
{  
    viewMatrix = m_viewMatrix;  
    return;  
}
