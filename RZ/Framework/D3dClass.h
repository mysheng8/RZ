#ifndef _D3DCLASS_H_  
#define _D3DCLASS_H_

#pragma comment(lib, "dxgi.lib")  
#pragma comment(lib, "d3d11.lib")  
#pragma comment(lib, "d3dx11.lib")  
#pragma comment(lib, "d3dx10.lib") 

#include <dxgi.h>  
#include <d3dcommon.h>  
#include <d3d11.h>  
#include <d3dx10math.h> 
#include <vector>
#include <d3dx11async.h>  
using namespace std;

struct VPS // Vertex-Position-UV
{
    D3DXVECTOR3 Pos;
	D3DXVECTOR2 TexCoord;
};

struct CBufferType  
{  
	D3DXMATRIX wvp; 
};

class D3DClass  
{  
public:  
    D3DClass();  
    D3DClass(const D3DClass&);  
    ~D3DClass();  
  
    bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,float screenDepth, float screenNear);  
    void Shutdown();  
    
	bool InitScene(int screenWidth, int screenHeight);
    void BeginScene(float, float, float, float); 
    void EndScene();  
  
    ID3D11Device* GetDevice();  
    ID3D11DeviceContext* GetDeviceContext();  
  
    void GetProjectionMatrix(D3DXMATRIX&);  
    void GetWorldMatrix(D3DXMATRIX&);  
    void GetOrthoMatrix(D3DXMATRIX&);  
  
    void GetVideoCardInfo(char*, int&);  



private:  
    bool m_vsync_enabled;  
    int m_videoCardMemory;  
    char m_videoCardDescription[128];

    IDXGISwapChain* m_swapChain;  
    ID3D11Device* m_device;  
    ID3D11DeviceContext* m_deviceContext;  
	ID3D11DepthStencilState* m_depthStencilState; 
	ID3D11RasterizerState* m_rasterState;

	//final view
    ID3D11RenderTargetView* m_renderTargetView;  
    ID3D11Texture2D* m_depthStencilBuffer;  
	ID3D11DepthStencilView* m_depthStencilView;
	
     
    D3DXMATRIX m_projectionMatrix;  
    D3DXMATRIX m_worldMatrix;  
    D3DXMATRIX m_orthoMatrix;  



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
	void RenderFrame();


};

#endif