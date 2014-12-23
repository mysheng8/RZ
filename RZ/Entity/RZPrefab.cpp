#include "RZPrefab.h"


using namespace RZ;

RZPrefab::RZPrefab()
{
}

RZPrefab::RZPrefab(const RZPrefab& other)
{
}


RZPrefab::~RZPrefab()
{
}

bool RZPrefab::Initialize(RZEntity* entity, const XMFLOAT4X4 &transform)
{
	m_entity=entity;
	m_trans=transform;
	m_id=0;
	return true;
}

void RZPrefab::ShutDown()
{
	m_entity->ShutDown();
}

bool RZPrefab::Render(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	bool result;
	result=m_entity->Render(pd3dDeviceContext,worldMatrix,viewMatrix,projectionMatrix);
	return result;
}



