#include "RZLightManager.h"

using namespace RZ;

RZLightManager* RZLightManager::m_instance = NULL;

RZLightManager::RZLightManager()
{
}

RZLightManager::RZLightManager(const RZLightManager& other)
{
}

RZLightManager::~RZLightManager()
{
	delete m_instance;
}

bool RZLightManager::Initialize(ID3D11Device* pDevice,ID3D11DeviceContext* pd3dDeviceContext)
{
	m_device=pDevice;
	m_deviceContext=pd3dDeviceContext;
  /*
	HRESULT result;  


    D3D11_BUFFER_DESC staticBufferDesc; 
	D3D11_BUFFER_DESC dynamicBufferDesc;

  
	staticBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;  
    staticBufferDesc.ByteWidth = sizeof(RZLightParams);  
    staticBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  
    staticBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  
    staticBufferDesc.MiscFlags = 0;  
    staticBufferDesc.StructureByteStride = 0;  
  
    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class. 

    result = m_device->CreateBuffer(&staticBufferDesc, NULL, &m_staticBuffer);  
    if(FAILED(result))  
    {  
        return false;  
    }

	dynamicBufferDesc.Usage = D3D11_USAGE_DYNAMIC;  
    dynamicBufferDesc.ByteWidth = sizeof(RZLightParams);  
    dynamicBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  
    dynamicBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  
    dynamicBufferDesc.MiscFlags = 0;  
    dynamicBufferDesc.StructureByteStride = 0;  
  
    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class. 

    result = m_device->CreateBuffer(&dynamicBufferDesc, NULL, &m_dynamicBuffer);  
    if(FAILED(result))  
    {  
        return false;  
    }


	*/

	
	XMFLOAT3 dir;
	dir.x=2;
	dir.y=-1;
	dir.z=0;
	XMFLOAT3 color;
	color.x=1;
	color.y=1;
	color.z=1;
	AddDirectionLight(true,dir,color);

	

	return true;
}

bool RZLightManager::AddDirectionLight(bool isStatic, XMFLOAT3 dir,XMFLOAT3 color)
{
	RZLight *dl=new RZLight();
	dl->Initialize(3,isStatic);
	dl->SetLightColor(color.x, color.y,color.z);
	dl->SetLightDirection(dir.x,dir.y,dir.z);

	if(isStatic)
	{
		m_staticList.push_back(dl);
	}
	m_dynamicList.push_back(dl);

	return true;
}

bool RZLightManager::AddPointLight(bool isStatic, XMFLOAT3 pos,XMFLOAT3 color,float range)
{
	RZLight *pl=new RZLight();
	pl->Initialize(1,isStatic);
	pl->SetLightColor(color.x, color.y,color.z);
	pl->SetLightPosition(pos.x,pos.y,pos.z);
	pl->SetLightRange(range);
	if(isStatic)
	{
		m_staticList.push_back(pl);
	}
	m_dynamicList.push_back(pl);

	return true;
}

bool RZLightManager::AddSpotLight(bool isStatic, XMFLOAT3 pos, XMFLOAT3 dir,XMFLOAT3 color,float range,float angleX,float angleY)
{
	RZLight *sl=new RZLight();
	sl->Initialize(2,isStatic);
	sl->SetLightColor(color.x, color.y,color.z);
	sl->SetLightDirection(dir.x,dir.y,dir.z);
	sl->SetLightPosition(pos.x,pos.y,pos.z);
	sl->SetSpotLightAngles(angleX,angleY);
	sl->SetLightRange(range);
	if(isStatic)
	{
		m_staticList.push_back(sl);
	}
	m_dynamicList.push_back(sl);

	return true;
}

bool RZLightManager::Render()
{

	for(vector<RZLight*>::iterator it=m_staticList.begin();it!=m_staticList.end();++it)
	{
		(*it)->Render(m_deviceContext);
	}
	return true;
}

void RZLightManager::BeginScene()
{

	RZLightParams *cbpl=new RZLightParams();
	/*
	for(vector<RZLight*>::iterator it=m_staticList.begin();it!=m_staticList.end();++it)
	{
		(*it)->GetLightColor(cbpl->lightColor);
		int type=(*it)->GetLightType();
		switch(type)
		{
			case 1:
			{
				(*it)->GetLightPosition(cbpl->lightPos);
				(*it)->GetPointLightRange(cbpl->lightRange.x);
				break;
			}
			case 2:
			{
				(*it)->GetLightPosition(cbpl->lightPos);
				(*it)->GetLightDirection(cbpl->lightDir);
				(*it)->GetSpotLightAngles(cbpl->lightRange.y,cbpl->lightRange.z);
				break;
			}
			case 3:
			{
				(*it)->GetLightDirection(cbpl->lightDir);
				break;
			}
			default:
			{
			}
		}
		m_deviceContext->UpdateSubresource( m_staticBuffer, 0, NULL, &cbpl, 0, 0 );
	}
	*/
}

void RZLightManager::ShotDown()
{
	for(vector<RZLight*>::iterator it=m_staticList.begin();it!=m_staticList.end();++it)
	{
		delete (*it);
	}
	m_staticList.clear();

	for(vector<RZLight*>::iterator it=m_dynamicList.begin();it!=m_dynamicList.end();++it)
	{
		delete (*it);
	}
	m_dynamicList.clear();
}