#include "RZEntity.h"


using namespace RZ;

RZEntity::RZEntity()
{
}

RZEntity::RZEntity(const RZEntity& other)
{
}


RZEntity::~RZEntity()
{
}

bool RZEntity::Initialize(RZEntityParams* params)
{
	m_mesh=params->pMesh;
	return true;
}

void RZEntity::ShutDown()
{
	m_mesh->ReleaseModel();
}

bool RZEntity::Render(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	bool result;
	result=m_mesh->Render(pd3dDeviceContext,worldMatrix,viewMatrix,projectionMatrix);
	return result;
}



