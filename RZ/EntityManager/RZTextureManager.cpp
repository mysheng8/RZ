#include "RZTextureManager.h"

using namespace RZ;

RZTextureManager* RZTextureManager::m_instance = NULL;

RZTextureManager::RZTextureManager()
{
}
RZTextureManager::RZTextureManager(RZTextureManager &other)
{
}

RZTextureManager::~RZTextureManager()
{
	delete m_instance;
}

bool RZTextureManager::Initialize(ID3D11Device* pDevice)
{
	RZTexture* pTex1=new RZTexture();
	pTex1->Initialize(pDevice,colorImage);
	m_texMap.insert(make_pair("color0",pTex1));

	RZTexture* pTex2=new RZTexture();
	pTex2->Initialize(pDevice,normalImage);
	m_texMap.insert(make_pair("normal0",pTex2));
	return true;
}

bool RZTextureManager::ShotDown()
{
	for(map<string,RZTexture*>::iterator it=m_texMap.begin();it!=m_texMap.end();++it)
	{
		it->second->ShutDown();
	}
	m_texMap.clear();
	return true;
}