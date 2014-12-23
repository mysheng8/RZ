#pragma once
#ifndef _RZMATERIAL_H_
#define _RZMATERIAL_H_



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
		XMFLOAT4				ambience;
		XMFLOAT4				emissive;				
		XMFLOAT2				specular;    //x--glossness, y--roughness
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
		bool Render(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, int vertexStart,  const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		//bool GetParams(RZMatParams& outParams){outParams= m_params;return true;};
		

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