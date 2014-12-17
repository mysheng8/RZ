#pragma once
#ifndef _RZPREFAB_H_
#define _RZPREFAB_H_


#include <stdio.h>
#include <vector>
#include "RZEntity.h"

using namespace std;

namespace RZ
{
	class RZPrefab
	{
	public:
		RZPrefab();
		RZPrefab(const RZPrefab& other);
		virtual ~RZPrefab();
		bool Initialize(RZEntity* entity, D3DXMATRIX transform);
		void ShutDown();
		bool Render(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX worldMatrix,D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
		D3DXMATRIX GetTransform(){return m_trans; };
	private:
		int m_id;
		RZEntity* m_entity;
		D3DXMATRIX m_trans;
	};



};

#endif