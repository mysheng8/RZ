#ifndef _RZENTITY_H_
#define _RZENTITY_H_


#include <stdio.h>
#include <vector>
#include "RZMesh.h"
#include "RZTexture.h"

using namespace std;

namespace RZ
{
	class RZEntity
	{
	public:
		RZEntity();
		RZEntity(const RZEntity& other);
		virtual ~RZEntity();
		bool Initialize(ID3D11Device* device,HWND hwnd, char* meshFileName,char* texFileName);
		bool RegisterShader(char* vsFileName,char* psFileName);
		void Shutdown();
		bool Render(ID3D11DeviceContext* pd3dDeviceContext,D3DXMATRIX, D3DXVECTOR4, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	private:
		UINT id;
		RZMesh* pMesh;
		vector<RZShader*> pShaderList;
		vector<RZTexture*> pTexList;

	};

};

#endif