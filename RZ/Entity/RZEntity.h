#pragma once
#ifndef _RZENTITY_H_
#define _RZENTITY_H_

#include <d3d11.h>  
#include <d3dx10math.h>  
#include <stdio.h>
#include <vector>
#include "RZMesh.h"
#include "RZTexture.h"

using namespace std;

namespace RZ
{
	struct RZEntityParams
	{
		RZMesh*			pMesh;
	};

	class RZEntity
	{
	//friend class RZEntityFactory;
	public:
		RZEntity();
		RZEntity(const RZEntity& other);
		virtual ~RZEntity();
		bool Initialize(RZEntityParams* params);
		bool Render(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
		void ShutDown();

	private:
		RZMesh*			m_mesh;
	};
};

#endif