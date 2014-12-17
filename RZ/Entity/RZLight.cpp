#include "RZLight.h"

using namespace RZ;
/*
RZLight::RZLight(InputClass* input)  
{  
	m_lightDir=D3DXVECTOR4(0,1.0f,-1.0f,0);
	D3DXVec4Normalize(&m_lightDir,&m_lightDir);
	m_pInput=input;
	m_rotSpeed=0.005f;
}
*/

RZLight::RZLight(int lightType, bool isStatic)  
{  
	m_isStatic = isStatic;
	m_lightType = lightType;
	m_params = new LightParams();

	m_params->lightDir=D3DXVECTOR3(0,1.0f,-1.0f);
	m_params->lightPos=D3DXVECTOR3(0,1.0f,-1.0f);
	m_params->lightColor=D3DXVECTOR3(0,1.0f,-1.0f);
	m_params->lightRange=D3DXVECTOR3(0,1.0f,-1.0f);

}

RZLight::RZLight(const RZLight& other)  
{  
}  
  
  
RZLight::~RZLight()  
{  
}

void RZLight::SetLightDirection(float x,float y, float z)  
{  
	m_params->lightDir=D3DXVECTOR3(x,y,z);
    return;  
}

void RZLight::GetLightDirection(D3DXVECTOR3& outDir)  
{  
	outDir=m_params->lightDir;
    return;  
}

void RZLight::SetLightPosition(float x,float y, float z)  
{  
	m_params->lightPos=D3DXVECTOR3(x,y,z);
    return;  
}

void RZLight::GetLightPosition(D3DXVECTOR3& outPos)  
{  
	outPos=m_params->lightPos;
    return;  
}

void RZLight::SetLightColor(float x,float y, float z)  
{  
	m_params->lightDir=D3DXVECTOR3(x,y,z);
    return;  
}

void RZLight::GetLightColor(D3DXVECTOR3& outColor)  
{  
	outColor=m_params->lightColor;
    return;  
}

void RZLight::SetPointLightRange(float x)  
{  
	m_params->lightRange.x=x;
    return;  
}

void RZLight::GetPointLightRange(float& x)  
{  
	x=m_params->lightRange.x;
    return;  
}

void RZLight::SetSpotLightAngles(float y,float z)  
{  
	m_params->lightRange.y=y;
	m_params->lightRange.z=z;
    return;  
}

void RZLight::GetSpotLightAngles(float& y, float& z)  
{  
	y=m_params->lightRange.y;
	z=m_params->lightRange.z;
    return;  
}

/*
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
*/