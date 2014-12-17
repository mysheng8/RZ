#include "RZEntityManager.h"

using namespace RZ;

RZEntityManager::RZEntityManager()
{
}

RZEntityManager::RZEntityManager(const RZEntityManager &other)
{
}

RZEntityManager::~RZEntityManager()
{
}

bool RZEntityManager::Initialize(ID3D11Device* pDevice)
{

	RZMesh *mesh=new RZMesh();
	mesh->CreateFromFile(pDevice,"E:\\mine\\RZ\\RZ\\Resource\\model\\test.rz");
	mesh->SetMaterial(0,RZMaterialManager.GetInstance()->GetMaterial("basic0"));
	mesh->SetMaterial(1,RZMaterialManager.GetInstance()->GetMaterial("basic1"));

	RZEntityParams *params=new RZEntityParams();
	params->pMesh=mesh;


	RZEntity *entity=new RZEntity();

	entity->Initialize(params);

	m_entityMap.insert(make_pair("test",entity));


	return true; 
}

bool RZEntityManager::ShotDown()
{
	for(map<string,RZEntity*>::iterator it=m_entityMap.begin();it!=m_entityMap.end();++it)
	{
		it->second->ShutDown();
	}
	m_entityMap.clear();
}



