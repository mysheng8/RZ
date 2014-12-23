#include "RZD3dRender.h"

using namespace RZ;

RZD3dRender* RZD3dRender::m_instance = NULL;

RZD3dRender::RZD3dRender()  
{  /*
    m_swapChain = 0;  
    m_device = 0;  
    m_deviceContext = 0;  
    m_renderTargetView = 0;  
    m_depthStencilBuffer = 0;  
    m_depthStencilState = 0;  
    m_depthStencilView = 0;  
    m_rasterState = 0;  
	m_rMRTs = 0
	*/
}

RZD3dRender::RZD3dRender(const RZD3dRender& other)  
{  
} 
  
RZD3dRender::~RZD3dRender()  
{  
	delete m_instance;
}

bool RZD3dRender::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;  
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;  
    D3D11_RASTERIZER_DESC rasterDesc;  
    D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	m_vsync_enabled = vsync;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))  
    {  
        return false;  
    }

	result = factory->EnumAdapters(0, &adapter);  
    if(FAILED(result))  
    {  
        return false;  
    }

	result = adapter->EnumOutputs(0, &adapterOutput);  
    if(FAILED(result))  
    {  
        return false;  
    }

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);  
    if(FAILED(result))  
    {  
        return false;  
    }

	displayModeList = new DXGI_MODE_DESC[numModes];  
    if(!displayModeList)  
    {  
        return false;  
    }

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);  
    if(FAILED(result))  
    {  
        return false;  
    }

	for(i=0; i<numModes; i++)  
    {  
        if(displayModeList[i].Width == (unsigned int)screenWidth)  
        {  
            if(displayModeList[i].Height == (unsigned int)screenHeight)  
            {  
                numerator = displayModeList[i].RefreshRate.Numerator;  
                denominator = displayModeList[i].RefreshRate.Denominator;  
            }  
        }  
    }

	result = adapter->GetDesc(&adapterDesc);  
    if(FAILED(result))  
    {  
        return false;  
    }

	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);  
    if(error != 0)  
    {  
        return false;  
    }

	// Release the display mode list.  
    delete [] displayModeList;  
    displayModeList = 0; 

	// Release the adapter output.  
    adapterOutput->Release();  
    adapterOutput = 0;

   // Release the adapter.  
    adapter->Release();  
    adapter = 0;  
  
    // Release the factory.  
    factory->Release();  
    factory = 0;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;

    swapChainDesc.BufferDesc.Width = screenWidth;  
    swapChainDesc.BufferDesc.Height = screenHeight;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if(m_vsync_enabled)  
    {  
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;  
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;  
    }  
    else  
    {  
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;  
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;  
    }

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;

	swapChainDesc.SampleDesc.Count = 1;  
    swapChainDesc.SampleDesc.Quality = 0;

	if(fullscreen)  
    {  
        swapChainDesc.Windowed = false;  
    }  
    else  
    {  
        swapChainDesc.Windowed = true;  
    }

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;  
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);  
    if(FAILED(result))  
    {  
        return false;  
    }

	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);  
    if(FAILED(result))  
    {  
        return false;  
    }

	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);  
    if(FAILED(result))  
    {  
        return false;  
    }

	backBufferPtr->Release();  
    backBufferPtr = 0;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;  
    depthBufferDesc.Height = screenHeight;  
    depthBufferDesc.MipLevels = 1;  
    depthBufferDesc.ArraySize = 1;  
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  
    depthBufferDesc.SampleDesc.Count = 1;  
    depthBufferDesc.SampleDesc.Quality = 0;  
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;  
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;  
    depthBufferDesc.CPUAccessFlags = 0;  
    depthBufferDesc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);  
    if(FAILED(result))  
    {  
        return false;  
    }

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;  
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;  
    depthStencilDesc.StencilReadMask = 0xFF;  
    depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;  
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;  
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;  
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;  
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;  
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;  
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);  
    if(FAILED(result))  
    {  
        return false;  
    }

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;  
    depthStencilViewDesc.Texture2D.MipSlice = 0;
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);  
    if(FAILED(result))  
    {  
        return false;  
    }

	//m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	rasterDesc.AntialiasedLineEnable = false;  
    rasterDesc.CullMode = D3D11_CULL_BACK;  
    rasterDesc.DepthBias = 0;  
    rasterDesc.DepthBiasClamp = 0.0f;  
    rasterDesc.DepthClipEnable = true;  
    rasterDesc.FillMode = D3D11_FILL_SOLID;  
    rasterDesc.FrontCounterClockwise = false;  
    rasterDesc.MultisampleEnable = false;  
    rasterDesc.ScissorEnable = false;  
    rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);  
    if(FAILED(result))  
    {  
        return false;  
    }

	m_deviceContext->RSSetState(m_rasterState);
	viewport.Width = (float)screenWidth;  
    viewport.Height = (float)screenHeight;  
    viewport.MinDepth = 0.0f;  
    viewport.MaxDepth = 1.0f;  
    viewport.TopLeftX = 0.0f;  
    viewport.TopLeftY = 0.0f;
	m_deviceContext->RSSetViewports(1, &viewport);
	fieldOfView = (float)XM_PI / 4.0f;  
    screenAspect = (float)screenWidth / (float)screenHeight;

	XMStoreFloat4x4(&m_projectionMatrix,XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth));
	XMStoreFloat4x4(&m_worldMatrix,XMMatrixIdentity());
	XMStoreFloat4x4(&m_orthoMatrix,XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth));  
  

	m_rMRTs=RZRenderMRTs::GetInstance();
	if(!m_rMRTs->Initialize(m_device,hwnd,screenWidth,screenHeight))
	{
		return false;
	}
    return true; 
}



void RZD3dRender::BeginScene(float red, float green, float blue, float alpha)  
{  
	//set renderTarget to MRT
	ID3D11RenderTargetView* renderTargetViewArray[4] = 
	{
		m_rMRTs->GetRenderTargetView(1),
		m_rMRTs->GetRenderTargetView(2),
		m_rMRTs->GetRenderTargetView(3),
		m_rMRTs->GetRenderTargetView(4)
	};


	m_deviceContext->OMSetRenderTargets(4, renderTargetViewArray, m_depthStencilView);

	m_rMRTs->ClearMRTs(m_deviceContext);


    float color[4];  
  
  
    // Setup the background color  
    color[0] = red;  
    color[1] = green;  
    color[2] = blue;  
    color[3] = alpha;  
  
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);  
      
    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  
  
    return;  
}

void RZD3dRender::EndScene()  
{ 
	
	ID3D11RenderTargetView*	lightRTV;
	ID3D11BlendState* m_pCurrentBlendState;
	ID3D11BlendState* m_pOldBlendState;

	lightRTV=(m_rMRTs->GetRenderTargetView(0));
	m_deviceContext->OMSetRenderTargets(1, &lightRTV, m_depthStencilView);

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof(blendDesc) );
    blendDesc.AlphaToCoverageEnable = false;  
    blendDesc.IndependentBlendEnable = false;  
    blendDesc.RenderTarget[0].BlendEnable = TRUE;  
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;  
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;  
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;  
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;  
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;  
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;  
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; 

	m_device->CreateBlendState(&blendDesc, &m_pCurrentBlendState);

	m_deviceContext->OMGetBlendState(&m_pOldBlendState, 0, 0);
	m_deviceContext->OMSetBlendState(m_pCurrentBlendState, 0, 0xffffffff);  

	
	m_lManager->Render();

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	m_deviceContext->OMSetBlendState(m_pOldBlendState, 0, 0xffffffff); 
	m_rMRTs->RenderFinalPass(m_deviceContext);
    // Present the back buffer to the screen since rendering is complete.  
    if(m_vsync_enabled)  
    {  
        m_swapChain->Present(1, 0);  
    }  
    else  
    {  
        m_swapChain->Present(0, 0);  
    }  
  
    return;  
}



ID3D11Device* RZD3dRender::GetDevice()  
{  
    return m_device;  
}

ID3D11DeviceContext* RZD3dRender::GetDeviceContext()  
{  
    return m_deviceContext;  
}

void RZD3dRender::GetProjectionMatrix(XMMATRIX& projectionMatrix)  
{  
    projectionMatrix =XMLoadFloat4x4(&m_projectionMatrix);  
    return;  
}

void RZD3dRender::GetWorldMatrix(XMMATRIX& worldMatrix)  
{  
    worldMatrix = XMLoadFloat4x4(&m_worldMatrix);  
    return;  
}

void RZD3dRender::GetOrthoMatrix(XMMATRIX& orthoMatrix)  
{  
    orthoMatrix = XMLoadFloat4x4(&m_orthoMatrix);  
    return;  
}

void RZD3dRender::GetVideoCardInfo(char* cardName, int& memory)  
{  
    strcpy_s(cardName, 128, m_videoCardDescription);  
    memory = m_videoCardMemory;  
    return;  
}

void RZD3dRender::Shutdown()  
{  
	RZRELEASE(m_swapChain);
	RZRELEASE(m_rasterState);
	RZRELEASE(m_depthStencilView);
	RZRELEASE(m_depthStencilState);
	RZRELEASE(m_depthStencilBuffer);
	RZRELEASE(m_renderTargetView);
	RZRELEASE(m_deviceContext);
	RZRELEASE(m_device);
	RZRELEASE(m_swapChain);
  
    return;  
}
