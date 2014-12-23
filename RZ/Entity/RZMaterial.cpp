#include"RZMaterial.h"


using namespace RZ;


RZMaterial::RZMaterial()  
{  
    m_shader = 0; 
}

RZMaterial::RZMaterial(const RZMaterial& other)  
{  
}  
  
  
RZMaterial::~RZMaterial()  
{  
}

void RZMaterial::Initialize(RZShader* shader,RZMatParams* params, int numTex)
{
	m_shader=shader;
	m_params=params;
	m_numTex=numTex;
	m_texs=(RZTexture **)malloc(numTex*sizeof(RZTexture*));



}

bool RZMaterial::Render(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, int vertexStart, const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	for(int i=0;i!=m_numTex;++i)
	{
		m_texs[i]->Render(deviceContext,i);
	}
	bool result;
	result=m_shader->Render( deviceContext, indexCount, indexStart, vertexStart, worldMatrix, viewMatrix, projectionMatrix );

	return result;
}

void RZMaterial::ShutDown()
{
	//release in manager in the end
	m_shader=0;
	m_texs=0;

	delete(m_params);

	if(m_matBuffer)
	{
		m_matBuffer->Release();
		m_matBuffer=0;
	}
}



