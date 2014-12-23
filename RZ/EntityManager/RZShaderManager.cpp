#include "RZShaderManager.h"

using namespace RZ;

RZShaderManager* RZShaderManager::m_instance = NULL;

RZShaderManager::RZShaderManager()
{
}
RZShaderManager::RZShaderManager(RZShaderManager &other)
{
}

RZShaderManager::~RZShaderManager()
{
	delete m_instance;
}

bool RZShaderManager::Initialize(ID3D11Device* pDevice,HWND hwnd)
{
    HRESULT result;
	

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.  
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;  
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);  
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  
    matrixBufferDesc.MiscFlags = 0;  
    matrixBufferDesc.StructureByteStride = 0;  
  
    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class. 

    result = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer); 

	if(FAILED(result))  
    {  
        return false;  
    }  

	RZShader* pShader=new RZShader();
	if(!pShader->Initialize(pDevice,hwnd,m_matrixBuffer,basicVS, basicPS))
	{
		return false;
	}

	m_shaderMap.insert(make_pair("basic",pShader));

	return true; 
}

bool RZShaderManager::ShotDown()
{
	for(map<string,RZShader*>::iterator it=m_shaderMap.begin();it!=m_shaderMap.end();++it)
	{
		it->second->Shutdown();
	}
	m_shaderMap.clear();

	// Release the matrix constant buffer.  
    if(m_matrixBuffer)  
    {  
        m_matrixBuffer->Release();  
        m_matrixBuffer = 0;  
    } 
	return true;
}



