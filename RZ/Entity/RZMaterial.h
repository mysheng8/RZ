#ifndef _RZMATERIAL_H_
#define _RZMATERIAL_H_

#include <d3d11.h>  
#include <d3dx10math.h>  
#include <d3dx11async.h>
#include <stdio.h>
#include <vector>

#include "RZshader.h"
#include "RZTexture.h"
using namespace std;

namespace RZ
{
	struct RZMatParams
	{
		bool					isAlphaTest;
		bool					castShadow;
		bool					receiveShadow;

		float					ambienceR;
		float					ambienceG;
		float					ambienceB;

		float					emissiveR;
		float					emissiveG;
		float					emissiveB;

		float					specularGlossness;
		float					specularRoughness;
		RZMatParams();
		RZMatParams(bool alpha,bool cs,bool rs,float ar,float ag,float ab,float er,float eg,float eb,float sg,float sr)
		{isAlphaTest=alpha;castShadow=cs;receiveShadow=rs;ambienceR=ar;ambienceG=ag;ambienceB=ab;emissiveR=er;emissiveG=eg;emissiveB=eb;specularGlossness=sg;specularRoughness=sr;};
	};

	struct RZMatBuffer
	{
		D3DXCOLOR				ambience;
		D3DXCOLOR				emissive;				
		D3DXVECTOR2				specular;    //x--glossness, y--roughness
	};

	class RZMaterial
	{

	public:
		RZMaterial();
		RZMaterial(const RZMaterial&); 
		~RZMaterial();
		void Initialize(RZShader* shader,RZMatParams* params, int numTex);
		void ShutDown();
		void AddTexture(int slot, RZTexture* tex){m_texs[slot]=tex;};
		bool Render(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, int vertexStart, D3DXMATRIX worldMatrix,D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
		bool GetParams(RZMatParams& outParams){return m_params;};
		

	private:
		RZShader*				m_shader;
		int						m_numTex;
		RZTexture**				m_texs;
		RZMatParams*			m_params;

		bool SetShaderParameters();
		ID3D11Buffer*			m_matBuffer;
	};

};


#endif