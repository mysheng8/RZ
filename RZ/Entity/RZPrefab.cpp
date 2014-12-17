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

bool RZPrefab::Initialize(RZEntity* entity, D3DXMATRIX transform)
{
	m_entity=entity;
	m_trans=transform;
	m_id=0;
}

void RZPrefab::ShutDown()
{
	m_entity->ShutDown();
}

bool RZPrefab::Render(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX worldMatrix,D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;
	result=m_entity->Render(pd3dDeviceContext,worldMatrix,viewMatrix,projectionMatrix);
	return result;
}



