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
using namespace std;

struct VPNS // Vertex-Position-Normal
{
    D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexCoord;
};

class D3DClass  
{  
public:  
    D3DClass();  
    D3DClass(const D3DClass&);  
    ~D3DClass();  
  
    bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,float screenDepth, float screenNear);  
    void Shutdown();  
      
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
    ID3D11RenderTargetView* m_renderTargetView;  
    ID3D11Texture2D* m_depthStencilBuffer;  
    ID3D11DepthStencilState* m_depthStencilState;  
    ID3D11DepthStencilView* m_depthStencilView;  
    ID3D11RasterizerState* m_rasterState;  
    D3DXMATRIX m_projectionMatrix;  
    D3DXMATRIX m_worldMatrix;  
    D3DXMATRIX m_orthoMatrix;  


	//Deferred Rendering

private:
	// The Quad Mesh variables
	ID3D11Buffer*			_quadVB; // vertex buffer 
	ID3D11Buffer*			_quadIB; // index  buffer
	UINT					_numVQuad, _numIQuad;

	//MRTS
	ID3D11Texture2D*                    _mrtTex;					// Environment map
	ID3D11RenderTargetView*             _mrtRTV;					// Render target view for the mrts
	ID3D11ShaderResourceView*           _mrtSRV;					// Shader resource view for the mrts
	ID3D11Texture2D*                    _mrtMapDepth;				// Depth stencil for the environment map
	ID3D11DepthStencilView*             _mrtDSV;					// Depth stencil view for environment map for all 6 faces
	#define	NUMRTS 4;												// number of render targets
	#define TEXSCALE 2;												// scale of the render targets

	bool InitializeQuad(int screenWidth, int screenHeight);
	bool SetupMRTS(int screenWidth, int screenHeight);
	bool RenderMRTS();
	bool RenderFrame();


};

#endif