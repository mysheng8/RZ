#ifndef _RZMATERIAL_H_
#define _RZMATERIAL_H_

#include <d3d11.h>  
#include <d3dx10math.h>  
#include <d3dx11async.h>
#include "RZshader.h"
#include "RZTexture.h"
#include <stdio.h>
#include <vector>
using namespace std;

namespace RZ
{
	class RZMaterial
	{
	public:
		RZMaterial();
		RZMaterial(const RZMaterial&); 
		~RZMaterial();
		void Initialize(RZShader* shader){m_shader=shader;};
		void AddTexture(RZTexture* tex){m_texList.push_back(tex);};
		bool Render(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, int vertexStart, D3DXMATRIX worldViewProjection, D3DXVECTOR4 lightDir, D3DXMATRIX worldMatrix,D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	private:
		RZShader* m_shader;
		vector<RZTexture*> m_texList;
	};
};


#endif