#include "RZLightManager.h"

using namespace RZ;
RZLightManager::RZLightManager()
{
}

RZLightManager::RZLightManager(const RZLightManager& other)
{
}

RZLightManager::~RZLightManager()
{
}

bool RZLightManager::Initialize(ID3D11Device* pDevice,ID3D11DeviceContext* pd3dDeviceContext)
{
	m_device=pDevice;
	m_deviceContext=pd3dDeviceContext;

	HRESULT result;  
    ID3D10Blob* errorMessage;  
    ID3D10Blob* vertexShaderBuffer;  
    ID3D10Blob* pixelShaderBuffer;  

    D3D11_BUFFER_DESC staticBufferDesc; 
	D3D11_BUFFER_DESC dynamicBufferDesc;
  
  
    // Initialize the pointers this function will use to null.  
    errorMessage = 0;  
    vertexShaderBuffer = 0;  
    pixelShaderBuffer = 0;  


	//generate Point Light shaders
	D3D10_SHADER_MACRO Shader_Macros[1] = { "LIGHTTYPE", "1"  };
 
    result = D3DX11CompileFromFile("E:\\mine\\RZ\\RZ\\Resource\\shader\\lighting.vs", Shader_Macros, NULL, "LightingVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &vertexShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        return false;  
    }  
 
    result = D3DX11CompileFromFile("E:\\mine\\RZ\\RZ\\Resource\\shader\\lighting.ps", Shader_Macros, NULL, "LightingPixelShader", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &pixelShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {    
        return false;  
    }  
  
    result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pointLightVS);  
    if(FAILED(result))  
    {  
        return false;  
    }  
  
    result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pointLightPS);  
    if(FAILED(result))  
    {  
        return false;  
    } 

	//generate spot Light shaders
	D3D10_SHADER_MACRO Shader_Macros[1] = { "LIGHTTYPE", "2"  };
 
    result = D3DX11CompileFromFile("E:\\mine\\RZ\\RZ\\Resource\\shader\\lighting.vs", Shader_Macros, NULL, "LightingVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &vertexShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        return false;  
    }  
 
    result = D3DX11CompileFromFile("E:\\mine\\RZ\\RZ\\Resource\\shader\\lighting.ps", Shader_Macros, NULL, "LightingPixelShader", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &pixelShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {    
        return false;  
    }  
  
    result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_spotLightVS);  
    if(FAILED(result))  
    {  
        return false;  
    }  
  
    result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_spotLightPS);  
    if(FAILED(result))  
    {  
        return false;  
    }  

	//generate direction Light shaders
	D3D10_SHADER_MACRO Shader_Macros[1] = { "LIGHTTYPE", "3"  };
 
    result = D3DX11CompileFromFile("E:\\mine\\RZ\\RZ\\Resource\\shader\\lighting.vs", Shader_Macros, NULL, "LightingVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &vertexShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        return false;  
    }  
 
    result = D3DX11CompileFromFile("E:\\mine\\RZ\\RZ\\Resource\\shader\\lighting.ps", Shader_Macros, NULL, "LightingPixelShader", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &pixelShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {    
        return false;  
    }  
  
    result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_directionLightVS);  
    if(FAILED(result))  
    {  
        return false;  
    }  
  
    result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_directionLightPS);  
    if(FAILED(result))  
    {  
        return false;  
    }  

	vertexShaderBuffer->Release();  
    vertexShaderBuffer = 0;  
  
    pixelShaderBuffer->Release();  
    pixelShaderBuffer = 0; 

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





	D3DXVECTOR3 dir;
	dir.x=0;
	dir.y=-1;
	dir.z=-1;
	D3DXVECTOR3 color;
	color.x=1;
	color.y=1;
	color.z=1;
	AddDirectionLight(true,dir,color);










	return true;
}

bool RZLightManager::AddDirectionLight(bool isStatic, D3DXVECTOR3 dir,D3DXVECTOR3 color)
{
	RZLight *dl=new RZLight(3,isStatic);
	dl->SetLightColor(color.x, color.y,color.z);
	dl->SetLightDirection(dir.x,dir.y,dir.z);

	if(isStatic)
	{
		m_staticList.push_back(dl);
	}
	m_dynamicList.push_back(dl);

	return true;
}

bool RZLightManager::AddPointLight(bool isStatic, D3DXVECTOR3 pos,D3DXVECTOR3 color,float range)
{
	RZLight *pl=new RZLight(1,isStatic);
	pl->SetLightColor(color.x, color.y,color.z);
	pl->SetLightPosition(pos.x,pos.y,pos.z);
	pl->SetPointLightRange(range);
	if(isStatic)
	{
		m_staticList.push_back(pl);
	}
	m_dynamicList.push_back(pl);

	return true;
}

bool RZLightManager::AddSpotLight(bool isStatic, D3DXVECTOR3 pos, D3DXVECTOR3 dir,D3DXVECTOR3 color,float angleX,float angleY)
{
	RZLight *sl=new RZLight(2,isStatic);
	sl->SetLightColor(color.x, color.y,color.z);
	sl->SetLightDirection(dir.x,dir.y,dir.z);
	sl->SetLightPosition(pos.x,pos.y,pos.z);
	sl->SetSpotLightAngles(angleX,angleY);

	if(isStatic)
	{
		m_staticList.push_back(sl);
	}
	m_dynamicList.push_back(sl);

	return true;
}

bool RZLightManager::Render()
{


}

void RZLightManager::BeginScene()
{

	RZLightParams *cbpl=new RZLightParams();

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

}

void RZLightManager::ShotDown()
{

}