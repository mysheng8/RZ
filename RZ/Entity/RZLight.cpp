#include "RZLight.h"

using namespace RZ;

RZLight::RZLight(InputClass* input)  
{  
	m_lightDir=D3DXVECTOR4(0,1.0f,-1.0f,0);
	D3DXVec4Normalize(&m_lightDir,&m_lightDir);
	m_pInput=input;
	m_rotSpeed=0.005f;
}

RZLight::RZLight(const RZLight& other)  
{  
}  
  
  
RZLight::~RZLight()  
{  
}

void RZLight::SetLightDirection(float x,float y, float z)  
{  
	m_lightDir=D3DXVECTOR4(x,y,z,0);
    return;  
}

void RZLight::GetLightDirection(D3DXVECTOR4& outDir)  
{  
	outDir=m_lightDir;
    return;  
}

void RZLight::UpdateTransform()
{
	if(m_pInput->IsMapKeyDown(SPACEDOWN))
	{
		MoveDelta delta=m_pInput->GetMouseDelta();

		D3DXVECTOR3 lookAt;


		D3DXMATRIX rotationMatrix;
		float yaw, pitch, roll;

		lookAt.x = m_lightDir.x;  
		lookAt.y = m_lightDir.y;  
		lookAt.z = m_lightDir.z;

		pitch = 0.0f;  
		yaw   = -delta.x*m_rotSpeed;  
		roll  = 0.0f; 

		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll); 
		D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);

		m_lightDir.x=lookAt.x;
		m_lightDir.y=lookAt.y;
		m_lightDir.z=lookAt.z;
	}


}