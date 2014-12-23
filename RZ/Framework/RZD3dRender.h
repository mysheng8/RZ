#pragma once
#ifndef _RZD3dRender_H_  
#define _RZD3dRender_H_

#pragma comment(lib, "dxgi.lib")  
#pragma comment(lib, "d3d11.lib")  
#pragma comment(lib, "d3dx11.lib")  
//#pragma comment(lib, "d3dx10_1.lib") 

#include <dxgi.h>  
#include <d3dcommon.h>  
#include <d3d11.h>  
#include <vector>
#include <d3dx11async.h>  
#include <xnamath.h> 

#include "RZCommon.h"
#include "RZRenderMRTs.h"
#include "../EntityManager/RZLightManager.h"
using namespace std;


namespace RZ
{
	class RZD3dRender  
	{  
	public:  
		static RZD3dRender* GetInstance()
		{
			if(!m_instance)
			{
				m_instance= new RZD3dRender();
			}
			return m_instance;
		}

		~RZD3dRender();  
  
		bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,float screenDepth, float screenNear);  
		void Shutdown();  
    
		bool InitScene(int screenWidth, int screenHeight);
		void BeginScene(float, float, float, float); 
		void EndScene();  
  
		ID3D11Device* GetDevice();  
		ID3D11DeviceContext* GetDeviceContext();  
  
		void GetProjectionMatrix(XMMATRIX&);  
		void GetWorldMatrix(XMMATRIX&);  
		void GetOrthoMatrix(XMMATRIX&);  
  
		void GetVideoCardInfo(char*, int&);  



	private:  
		RZD3dRender();  
		RZD3dRender(const RZD3dRender&); 

		static RZD3dRender*				m_instance;


		bool							m_vsync_enabled;  
		int								m_videoCardMemory;  
		char							m_videoCardDescription[128];

		IDXGISwapChain*					m_swapChain;  
		ID3D11Device*					m_device;  
		ID3D11DeviceContext*			m_deviceContext;  
		ID3D11DepthStencilState*		m_depthStencilState; 
		ID3D11RasterizerState*			m_rasterState;

		//final view
		ID3D11RenderTargetView*			m_renderTargetView;  
		ID3D11Texture2D*				m_depthStencilBuffer;  
		ID3D11DepthStencilView*			m_depthStencilView;
	
     
		XMFLOAT4X4						m_projectionMatrix;  
		XMFLOAT4X4						m_worldMatrix;  
		XMFLOAT4X4						m_orthoMatrix;  

		RZRenderMRTs*					m_rMRTs;
		RZLightManager*					m_lManager;

	/*
		//Deferred Rendering
	private:
		//G buffer
		ID3D11Texture2D*                    m_colorTex;					// color map
		ID3D11Texture2D*                    m_normalTex;				// normal map
		ID3D11Texture2D*                    m_specularTex;				// specular map
		ID3D11Texture2D*                    m_posTex;					// position map

		ID3D11RenderTargetView*             m_colorRTV;					// Render target view for the color
		ID3D11RenderTargetView*             m_normalRTV;				// Render target view for the normal
		ID3D11RenderTargetView*             m_specularRTV;				// Render target view for the specular
		ID3D11RenderTargetView*             m_posRTV;					// Render target view for the position

		ID3D11ShaderResourceView*           m_colorSRV;					// Shader resource view for the color
		ID3D11ShaderResourceView*           m_normalSRV;				// Shader resource view for the normal
		ID3D11ShaderResourceView*           m_specularSRV;				// Shader resource view for the specular
		ID3D11ShaderResourceView*           m_posSRV;					// Shader resource view for the position

		ID3D11SamplerState*					m_colorSS;					// color map sampler state
		ID3D11SamplerState*					m_normalSS;					// normal map sampler state
		ID3D11SamplerState*					m_specularSS;				// specular map sampler state
		ID3D11SamplerState*					m_posSS;					// position map sampler state

		ID3D11Texture2D*                    m_mrtMapDepth;				// Depth stencil for the mrt map
		ID3D11DepthStencilView*             m_mrtDSV;					// Depth stencil view for mrt map

		//Lighting buffer
		ID3D11Texture2D*					m_lightingTex;				// lighting map
		ID3D11RenderTargetView*             m_lightingRTV;				// Render target view for the lighting
		ID3D11ShaderResourceView*           m_lightingSRV;				// Shader resource view for the lighting



		// The Quad Mesh variables
		ID3D11Buffer*			m_quadVB; // vertex buffer 
		ID3D11Buffer*			m_quadIB; // index  buffer

		UINT					m_numVQuad, m_numIQuad;

		ID3D11Buffer*			cBuffer; // constant buffer


		ID3D11VertexShader*		m_finalVS;
		ID3D11PixelShader*		m_finalPS;
		ID3D11InputLayout*		m_layout;


		bool InitializeQuad();
		bool SetupMRTS(int width, int height);
		void RenderLightingPass();
		void RenderFinalPass();

	*/
	};
};

#endif