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

bool RZMaterial::Render(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, int vertexStart, D3DXMATRIX worldViewProjection, D3DXVECTOR4 lightDir, D3DXMATRIX worldMatrix,D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	for(int i=0;i!=m_texList.size();++i)
	{
		m_texList[i]->Render(deviceContext);
	}
	bool result;
	result=m_shader->Render( deviceContext, indexCount, indexStart, vertexStart, worldViewProjection, lightDir, worldMatrix, viewMatrix, projectionMatrix );

	return result;
}