#pragma once
#ifndef _RZENTITY_H_
#define _RZENTITY_H_


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
		bool Render(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		void ShutDown();

	private:
		RZMesh*			m_mesh;
	};
};

#endif