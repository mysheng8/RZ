#pragma once
#ifndef _RZRENDERMRTS_H_  
#define _RZRENDERMRTS_H_

#include <fstream>
#include "RZCommon.h"
using namespace std;




namespace RZ
{
	struct RZLightParams
	{
		XMFLOAT3 lightPos;
		XMFLOAT3 lightDir;
		XMFLOAT3 lightColor;
		XMFLOAT3 lightRange;	//x - point light range; yz - spot light Angles 
	
	};

	struct RZQuadCB  
	{  
		XMMATRIX wvp; 
	};

	struct RZQuadVPS // Vertex-Position-UV
	{
		XMFLOAT3 Pos;
		XMFLOAT2 TexCoord;
	};

	class RZRenderMRTs  
	{  
	public:
		static RZRenderMRTs* GetInstance()
		{
			if(!m_instance)
			{
				m_instance= new RZRenderMRTs();
			}
			return m_instance;
		}

		~RZRenderMRTs();  
		bool Initialize(ID3D11Device* device,HWND hwnd, int screenWidth, int screenHeight);
		void ShutDown();
		void ClearMRTs(ID3D11DeviceContext* deviceContext);
		void RenderFinalPass(ID3D11DeviceContext* deviceContext);
		void RenderLightingPass(ID3D11DeviceContext* deviceContext,int type,RZLightParams* params);	//1--point,2--spot,3--directional

		ID3D11RenderTargetView* GetRenderTargetView(int type);					//0--Lighting,1--color,2--normal,3--specular,4--pos

  
	private:
		RZRenderMRTs();  
		RZRenderMRTs(const RZRenderMRTs&);
		static RZRenderMRTs*				m_instance;

		ID3D11Device*						m_device;

		//G buffer
		ID3D11Texture2D*                    m_colorTex;							// color map
		ID3D11Texture2D*                    m_normalTex;						// normal map
		ID3D11Texture2D*                    m_specularTex;						// specular map
		ID3D11Texture2D*                    m_posTex;							// position map

		ID3D11RenderTargetView*             m_colorRTV;							// Render target view for the color
		ID3D11RenderTargetView*             m_normalRTV;						// Render target view for the normal
		ID3D11RenderTargetView*             m_specularRTV;						// Render target view for the specular
		ID3D11RenderTargetView*             m_posRTV;							// Render target view for the position

		ID3D11ShaderResourceView*           m_colorSRV;							// Shader resource view for the color
		ID3D11ShaderResourceView*           m_normalSRV;						// Shader resource view for the normal
		ID3D11ShaderResourceView*           m_specularSRV;						// Shader resource view for the specular
		ID3D11ShaderResourceView*           m_posSRV;							// Shader resource view for the position

		//Lighting buffer
		ID3D11Texture2D*					m_lightingTex;						// lighting map
		ID3D11RenderTargetView*             m_lightingRTV;						// Render target view for the lighting
		ID3D11ShaderResourceView*           m_lightingSRV;						// Shader resource view for the lighting

		ID3D11SamplerState*					m_samplerState;						// position map sampler state

		// The Quad Mesh variables
		ID3D11Buffer*						m_quadVB;							// vertex buffer 
		ID3D11Buffer*						m_quadIB;							// index  buffer

		UINT								m_numVQuad, m_numIQuad;

		ID3D11Buffer*						m_matrixCB;							// world transform matrix constant buffer
		ID3D11Buffer*						m_lightingCB;						// lighting constant buffer

		//shaders
		ID3D11VertexShader*					m_finalVS;
		ID3D11PixelShader*					m_finalPS;
			
		ID3D11VertexShader*					m_pointLightVS;
		ID3D11PixelShader*					m_pointLightPS;

		ID3D11VertexShader*					m_spotLightVS;
		ID3D11PixelShader*					m_spotLightPS;

		ID3D11VertexShader*					m_directionLightVS;
		ID3D11PixelShader*					m_directionLightPS;


		ID3D11InputLayout*					m_layout;
		HWND								m_hwnd;
 
		bool InitializeQuad();
		bool SetupMRTS(int width, int height);
		bool InitializeShader();	

		void OutputShaderErrorMessage(ID3D10Blob*, HWND,const CHAR*); 
	
	};
};

#endif