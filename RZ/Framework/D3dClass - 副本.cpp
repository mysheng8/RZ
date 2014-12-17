#include "D3dClass.h"

D3DClass::D3DClass()  
{  
    m_swapChain = 0;  
    m_device = 0;  
    m_deviceContext = 0;  
    m_renderTargetView = 0;  
    m_depthStencilBuffer = 0;  
    m_depthStencilState = 0;  
    m_depthStencilView = 0;  
    m_rasterState = 0;  
}

D3DClass::D3DClass(const D3DClass& other)  
{  
} 
  
D3DClass::~D3DClass()  
{  
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,float screenDepth, float screenNear)
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

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

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
	fieldOfView = (float)D3DX_PI / 4.0f;  
    screenAspect = (float)screenWidth / (float)screenHeight;
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
	D3DXMatrixIdentity(&m_worldMatrix);
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);  
  
    return true; 
}

void D3DClass::Shutdown()  
{  
    // Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.  
    if(m_swapChain)  
    {  
        m_swapChain->SetFullscreenState(false, NULL);  
    }  
  
    if(m_rasterState)  
    {  
        m_rasterState->Release();  
        m_rasterState = 0;  
    }  
  
    if(m_depthStencilView)  
    {  
        m_depthStencilView->Release();  
        m_depthStencilView = 0;  
    }  
  
    if(m_depthStencilState)  
    {  
        m_depthStencilState->Release();  
        m_depthStencilState = 0;  
    }  
  
    if(m_depthStencilBuffer)  
    {  
        m_depthStencilBuffer->Release();  
        m_depthStencilBuffer = 0;  
    }  
  
    if(m_renderTargetView)  
    {  
        m_renderTargetView->Release();  
        m_renderTargetView = 0;  
    }  
  
    if(m_deviceContext)  
    {  
        m_deviceContext->Release();  
        m_deviceContext = 0;  
    }  
  
    if(m_device)  
    {  
        m_device->Release();  
        m_device = 0;  
    }  
  
    if(m_swapChain)  
    {  
        m_swapChain->Release();  
        m_swapChain = 0;  
    }  
  
    return;  
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)  
{  
    float color[4];  
  
  
    // Setup the color to clear the buffer to.  
    color[0] = red;  
    color[1] = green;  
    color[2] = blue;  
    color[3] = alpha;  
  
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);  
      
    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  
  
    return;  
}

void D3DClass::EndScene()  
{  
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

ID3D11Device* D3DClass::GetDevice()  
{  
    return m_device;  
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()  
{  
    return m_deviceContext;  
}

void D3DClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)  
{  
    projectionMatrix = m_projectionMatrix;  
    return;  
}

void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)  
{  
    worldMatrix = m_worldMatrix;  
    return;  
}

void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)  
{  
    orthoMatrix = m_orthoMatrix;  
    return;  
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)  
{  
    strcpy_s(cardName, 128, m_videoCardDescription);  
    memory = m_videoCardMemory;  
    return;  
}

bool D3DClass::InitializeQuad(int screenWidth, int screenHeight)
{
	HRESULT hr;

	// initialize the quad

	// the vertex info
	vector<VPNS> _quadVertices;

	VPNS v[4];

	/*
	  2----3
	  |    |
	  0----1 */

	v[0].Pos = D3DXVECTOR3(-screenWidth/2.0, -screenHeight/2.0, 0.0);//-800.0, -400.0, 0.0);
	v[0].Normal = D3DXVECTOR3(0, 0, 1.0);
	v[0].TexCoord = D3DXVECTOR2(0, 0);

	v[1].Pos = D3DXVECTOR3(screenWidth/2.0, -screenHeight/2.0, 0.0);//800.0, -400.0, 0.0);
	v[1].Normal = D3DXVECTOR3(0, 0, 1.0);
	v[1].TexCoord = D3DXVECTOR2(1, 0);

	v[2].Pos = D3DXVECTOR3(-screenWidth/2.0, screenHeight/2.0, 0.0);//-800.0, 400.0, 0.0);
	v[2].Normal = D3DXVECTOR3(0, 0, 1.0);
	v[2].TexCoord = D3DXVECTOR2(0, 1);

	v[3].Pos = D3DXVECTOR3(screenWidth/2.0, screenHeight/2.0, 0.0);//800.0, 400.0, 0.0);
	v[3].Normal = D3DXVECTOR3(0, 0, 1.0);
	v[3].TexCoord = D3DXVECTOR2(1, 1);

	_quadVertices.push_back(v[0]);
	_quadVertices.push_back(v[1]);
	_quadVertices.push_back(v[2]);
	_quadVertices.push_back(v[3]);

	// setup the vertex buffer
	_numVQuad = _quadVertices.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VPNS) * _numVQuad;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
    
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = _quadVertices.data();
	
	// Do the creation of the actual vertex buffer
	hr = m_device->CreateBuffer(&bd, &InitData, &_quadVB);
	if(FAILED(hr))
	{
        return false;
	}

	/** Setup Index Buffer -Triangle strip **/
	DWORD _quadIndices[4] = {0, 1, 2, 3};

	_numIQuad = 4;

	bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(DWORD) * _numIQuad;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    InitData.pSysMem = _quadIndices;
   
	// Create the index buffer
	hr = m_device->CreateBuffer(&bd, &InitData, &_quadIB);

    if(FAILED(hr))
	{
        return false;
	}

}


bool D3DClass::SetupMRTS(int screenWidth, int screenHeight)
{
	HRESULT hr;

	// Create depth stencil texture.
    D3D11_TEXTURE2D_DESC dstex;
	ZeroMemory( &dstex, sizeof(dstex) );
    dstex.Width = screenWidth * TEXSCALE;
    dstex.Height = screenHeight * TEXSCALE;
    dstex.MipLevels = 1;
    dstex.ArraySize = NUMRTS;
    dstex.SampleDesc.Count = 1;
    dstex.SampleDesc.Quality = 0;
    dstex.Format = DXGI_FORMAT_D32_FLOAT;
    dstex.Usage = D3D11_USAGE_DEFAULT;
    dstex.BindFlags =  D3D11_BIND_DEPTH_STENCIL;
    dstex.CPUAccessFlags = 0;
    //dstex.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

	_mrtMapDepth = NULL;

    hr =  m_device->CreateTexture2D( &dstex, NULL, &_mrtMapDepth );

	if(FAILED(hr))
	{
        return false;
	}
    // Create the depth stencil view for the mrts
    D3D11_DEPTH_STENCIL_VIEW_DESC DescDS;
	DescDS.Format = dstex.Format;
    DescDS.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    DescDS.Texture2DArray.FirstArraySlice = 0;
    DescDS.Texture2DArray.ArraySize = NUMRTS;
    DescDS.Texture2DArray.MipSlice = 0;

	_mrtDSV = NULL;

    hr =  m_device->CreateDepthStencilView( _mrtMapDepth, &DescDS, &_mrtDSV );

	if(FAILED(hr))
	{
        return false;
	}

    // Create all the multiple render target textures
    dstex.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
    dstex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    //dstex.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;
    //dstex.MipLevels = 2;
	_mrtTex = NULL;
    hr =  m_device->CreateTexture2D( &dstex, NULL, &_mrtTex );

	if(FAILED(hr))
	{
        return false;
	}

	// Create the 4 render target view
    D3D11_RENDER_TARGET_VIEW_DESC DescRT;
    DescRT.Format = dstex.Format;
    DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    DescRT.Texture2DArray.FirstArraySlice = 0;
    DescRT.Texture2DArray.ArraySize = NUMRTS;
    DescRT.Texture2DArray.MipSlice = 0;
    hr =  m_device->CreateRenderTargetView( _mrtTex, &DescRT, &_mrtRTV ); 

	if(FAILED(hr))
	{
        return false;
	}
	// Create the shader resource view for the cubic env map
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
	SRVDesc.Format = dstex.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	//SRVDesc.Texture2DArray;
	SRVDesc.Texture2DArray.ArraySize = NUMRTS;
	SRVDesc.Texture2DArray.FirstArraySlice = 0;
	SRVDesc.Texture2DArray.MipLevels = 1;
    //SRVDesc.TextureCube.MipLevels = 1;
    SRVDesc.Texture2DArray.MostDetailedMip = 0;
	_mrtSRV = NULL;
    hr =  m_device->CreateShaderResourceView( _mrtTex, &SRVDesc, &_mrtSRV );
	if(FAILED(hr))
	{
        return false;
	}
	return true;
}