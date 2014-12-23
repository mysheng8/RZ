#include "RZMaterialManager.h"

using namespace RZ;

RZMaterialManager* RZMaterialManager::m_instance = NULL;

RZMaterialManager::RZMaterialManager()
{
}
RZMaterialManager::RZMaterialManager(RZMaterialManager &other)
{
}

RZMaterialManager::~RZMaterialManager()
{
	delete m_instance;
}

bool RZMaterialManager::Initialize(ID3D11Device* pDevice)
{

    HRESULT result;
	RZShaderManager* sManager=RZShaderManager::GetInstance();
	RZShader *shader=sManager->GetShader("basic");

	RZTextureManager* tManager=RZTextureManager::GetInstance();
	RZTexture *tex0=tManager->GetTexture("color0");
	RZTexture *tex1=tManager->GetTexture("normal0");



	RZMaterial* mat0=new RZMaterial();
	RZMatParams *params=new RZMatParams(false,true,true,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.5f);
	mat0->Initialize(shader,params,2);
	mat0->AddTexture(0,tex0);
	mat0->AddTexture(1,tex1);
	m_matMap.insert(make_pair("basic0",mat0));

	RZMaterial* mat1=new RZMaterial();
	RZMatParams *mParams=new RZMatParams(false,true,true,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.5f);
	mat1->Initialize(shader,mParams,2);
	mat1->AddTexture(0,tex0);
	mat1->AddTexture(1,tex1);
	m_matMap.insert(make_pair("basic1",mat1));
	

	return true; 
}

bool RZMaterialManager::ShotDown()
{
	for(map<string,RZMaterial*>::iterator it=m_matMap.begin();it!=m_matMap.end();++it)
	{
		it->second->ShutDown();
	}
	m_matMap.clear();
	return true;
}



