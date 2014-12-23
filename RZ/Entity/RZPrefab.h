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
		bool Initialize(RZEntity* entity, const XMFLOAT4X4 &transform);
		void ShutDown();
		bool Render(ID3D11DeviceContext* pd3dDeviceContext,const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		XMFLOAT4X4 GetTransform(){return m_trans; };
	private:
		int m_id;
		RZEntity* m_entity;
		XMFLOAT4X4 m_trans;
	};



};

#endif