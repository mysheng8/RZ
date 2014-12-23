#include "RZLight.h"

using namespace RZ;


RZLight::RZLight()  
{  
}  


void RZLight::Initialize(int lightType, bool isStatic)  
{  
	m_isStatic = isStatic;
	m_lightType = lightType;
	m_params = new RZLightParams();

	m_params->lightDir=XMFLOAT3(0,1.0f,-1.0f);
	m_params->lightPos=XMFLOAT3(0,1.0f,-1.0f);
	m_params->lightColor=XMFLOAT3(0,1.0f,-1.0f);
	m_params->lightRange=XMFLOAT3(0,1.0f,-1.0f);

	m_rMRTs=RZRenderMRTs::GetInstance();

}

void RZLight::Render(ID3D11DeviceContext* pDeviceContext)
{
	m_rMRTs->RenderLightingPass(pDeviceContext,m_lightType,m_params);
}

RZLight::RZLight(const RZLight& other)  
{  
}  
  
  
RZLight::~RZLight()  
{  
}

void RZLight::SetLightDirection(float x,float y, float z)  
{  
	m_params->lightDir=XMFLOAT3(x,y,z);
    return;  
}

void RZLight::GetLightDirection(XMFLOAT3& outDir)  
{  
	outDir=m_params->lightDir;
    return;  
}

void RZLight::SetLightPosition(float x,float y, float z)  
{  
	m_params->lightPos=XMFLOAT3(x,y,z);
    return;  
}

void RZLight::GetLightPosition(XMFLOAT3& outPos)  
{  
	outPos=m_params->lightPos;
    return;  
}

void RZLight::SetLightColor(float x,float y, float z)  
{  
	m_params->lightDir=XMFLOAT3(x,y,z);
    return;  
}

void RZLight::GetLightColor(XMFLOAT3& outColor)  
{  
	outColor=m_params->lightColor;
    return;  
}

void RZLight::SetLightRange(float x)  
{  
	m_params->lightRange.x=x;
    return;  
}

void RZLight::GetLightRange(float& x)  
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

		XMFLOAT3 lookAt;


		XMMATRIX rotationMatrix;
		float yaw, pitch, roll;

		lookAt.x = m_lightDir.x;  
		lookAt.y = m_lightDir.y;  
		lookAt.z = m_lightDir.z;

		pitch = 0.0f;  
		yaw   = -delta.x*m_rotSpeed;  
		roll  = 0.0f; 

		XMMATRIXRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll); 
		D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);

		m_lightDir.x=lookAt.x;
		m_lightDir.y=lookAt.y;
		m_lightDir.z=lookAt.z;
	}


}
*/