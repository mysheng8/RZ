#include "RZLight.h"

using namespace RZ;


RZLight::RZLight()  
{  
}  


void RZLight::Initialize(int lightType, bool isStatic)  
{  
	m_isStatic = isStatic;
	m_lightType = lightType;

	lightDir=XMFLOAT3(0,1.0f,-1.0f);
	lightPos=XMFLOAT3(0,1.0f,0.0f);
	lightColor=XMFLOAT3(1.0f,1.0f,1.0f);
	lightRange=XMFLOAT3(0,1.0f,0.0f);

	m_rMRTs=RZRenderMRTs::GetInstance();

}

void RZLight::Render(ID3D11DeviceContext* pDeviceContext)
{
	m_rMRTs->RenderLightingPass(pDeviceContext,m_lightType,lightDir,lightPos,lightColor,lightRange);
}

RZLight::RZLight(const RZLight& other)  
{  
}  
  
  
RZLight::~RZLight()  
{  

}

void RZLight::SetLightDirection(float x,float y, float z)  
{  
	lightDir.x=x;
	lightDir.y=y;
	lightDir.z=z;
    return;  
}

void RZLight::GetLightDirection(XMFLOAT3& outDir)  
{  
	outDir=lightDir;
    return;  
}

void RZLight::SetLightPosition(float x,float y, float z)  
{  
	lightPos.x=x;
	lightPos.y=y;
	lightPos.z=z;
    return;  
}

void RZLight::GetLightPosition(XMFLOAT3& outPos)  
{  
	outPos=lightPos;
    return;  
}

void RZLight::SetLightColor(float x,float y, float z)  
{  
	lightDir.x=x;
	lightDir.y=y;
	lightDir.z=z;
    return;  
}

void RZLight::GetLightColor(XMFLOAT3& outColor)  
{  
	outColor=lightColor;
    return;  
}

void RZLight::SetLightRange(float x)  
{  
	lightRange.x=x;
    return;  
}

void RZLight::GetLightRange(float& x)  
{  
	x=lightRange.x;
    return;  
}

void RZLight::SetSpotLightAngles(float y,float z)  
{  
	lightRange.y=y;
	lightRange.z=z;
    return;  
}

void RZLight::GetSpotLightAngles(float& y, float& z)  
{  
	y=lightRange.y;
	z=lightRange.z;
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