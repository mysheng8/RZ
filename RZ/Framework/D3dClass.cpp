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
	fieldOfView = (float)D3DX_PI / 4.0f;  
    screenAspect = (float)screenWidth / (float)screenHeight;
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
	D3DXMatrixIdentity(&m_worldMatrix);
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);  
  
	if(!InitScene(screenWidth,screenHeight))
	{
		return false;
	}
    return true; 
}



void D3DClass::BeginScene(float red, float green, float blue, float alpha)  
{  
	//set renderTarget to MRT
	ID3D11RenderTargetView* renderTargetViewArray[4] = 
	{
		m_colorRTV,
		m_normalRTV,
		m_specularRTV,
		m_posRTV
	};


	m_deviceContext->OMSetRenderTargets(4, renderTargetViewArray, m_mrtDSV);

	

	// Clear render targets
	const float clearColor[4] = {0.f, 0.f, 0.f, 0.f};
	m_deviceContext->ClearRenderTargetView(m_colorRTV, clearColor); 
	m_deviceContext->ClearRenderTargetView(m_specularRTV, clearColor);

	const float clearNormal[4] = {0.5f, 0.5f, 0.5f, 0.f};
	m_deviceContext->ClearRenderTargetView(m_normalRTV, clearNormal);

	const float clearPos[4] = {0.f,0.f,0.f,1.f};
	m_deviceContext->ClearRenderTargetView(m_posRTV, clearPos);

	m_deviceContext->ClearDepthStencilView(m_mrtDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);


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

void D3DClass::EndScene()  
{  
	//set renderTarget back
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	//render quad
	m_deviceContext->VSSetShader(m_finalVS, 0, 0);
	m_deviceContext->PSSetShader(m_finalPS, 0, 0);

	UINT stride, offset;
	stride = sizeof(VPS);
    offset = 0;
	m_deviceContext->IASetInputLayout(m_layout);
	m_deviceContext->IASetIndexBuffer( m_quadIB, DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetVertexBuffers( 0, 1, &m_quadVB, &stride, &offset );
	m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	CBufferType cbt;
	
	D3DXMATRIX WVP;
	D3DXMatrixTranslation( &WVP,0.5f, -0.5f, 0.0f );
	D3DXMatrixScaling( &WVP,1.0f, 1.0f, 0.0f );
	D3DXMatrixTranspose(&WVP,&WVP);

	cbt.wvp=WVP;
	m_deviceContext->UpdateSubresource( cBuffer, 0, NULL, &cbt, 0, 0 );
	m_deviceContext->VSSetConstantBuffers( 0, 1, &cBuffer );

	m_deviceContext->PSSetShaderResources( 0, 1, &m_colorSRV );	// Draw the color map to the square
	m_deviceContext->PSSetSamplers( 0, 1, &m_colorSS );

	m_deviceContext->PSSetShaderResources( 1, 1, &m_normalSRV );	// Draw the normal map to the square
	m_deviceContext->PSSetSamplers( 1, 1, &m_normalSS );

	m_deviceContext->PSSetShaderResources( 2, 1, &m_specularSRV );	// Draw the specular map to the square
	m_deviceContext->PSSetSamplers( 2, 1, &m_specularSS );

	m_deviceContext->PSSetShaderResources( 3, 1, &m_posSRV );	// Draw the pos map to the square
	m_deviceContext->PSSetSamplers( 3, 1, &m_posSS );


	//Draw the second cube
	m_deviceContext->DrawIndexed( 6, 0, 0 );


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


bool D3DClass::InitializeQuad()
{
	HRESULT hr;

	// initialize the quad

	// the vertex info
	vector<VPS> _quadVertices;

	VPS v[4];

	/*
	  2----3
	  |    |
	  0----1 */

	v[0].Pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	v[0].TexCoord = D3DXVECTOR2(0, 1.0f);

	v[1].Pos = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	v[1].TexCoord = D3DXVECTOR2(0, 0);

	v[2].Pos = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	v[2].TexCoord = D3DXVECTOR2(1.0f, 0);

	v[3].Pos = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	v[3].TexCoord = D3DXVECTOR2(1, 1);

	_quadVertices.push_back(v[0]);
	_quadVertices.push_back(v[1]);
	_quadVertices.push_back(v[2]);
	_quadVertices.push_back(v[3]);

	// setup the vertex buffer
	m_numVQuad = _quadVertices.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VPS) * m_numVQuad;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
    
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = _quadVertices.data();
	
	// Do the creation of the actual vertex buffer
	hr = m_device->CreateBuffer(&bd, &InitData, &m_quadVB);
	if(FAILED(hr))
	{
        return false;
	}

	/** Setup Index Buffer **/
	DWORD _quadIndices[] = {0, 1, 2, 0, 2, 3};

	m_numIQuad = 6;

	bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(DWORD) * m_numIQuad;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    InitData.pSysMem = _quadIndices;
   
	// Create the index buffer
	hr = m_device->CreateBuffer(&bd, &InitData, &m_quadIB);

	if(FAILED(hr))
	{
        return false;
	}

	//Create constant buffer
	D3D11_BUFFER_DESC cbbd;	
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(CBufferType);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = m_device->CreateBuffer(&cbbd, NULL, &cBuffer);
	
    if(FAILED(hr))
	{
        return false;
	}

	// Create shader
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;

	hr = D3DX11CompileFromFile("E:\\mine\\RZ\\RZ\\Resource\\shader\\renderToQuad.vs", 0, 0, "RenderToQuadVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0,&VS_Buffer, 0, 0); 

	if(FAILED(hr))  
    {    
        return false;  
    } 

	hr = D3DX11CompileFromFile("E:\\mine\\RZ\\RZ\\Resource\\shader\\renderToQuad.ps", 0, 0, "RenderToQuadPixelShader", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0, &PS_Buffer, 0, 0); 

	if(FAILED(hr))  
    {    
        return false;  
    } 

	hr = m_device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &m_finalVS);  
    if(FAILED(hr))  
    {  
        return false;  
    }  
  
    // Create the pixel shader from the buffer.  
    hr = m_device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &m_finalPS);  
    if(FAILED(hr))  
    {  
        return false;  
    }  

	// Create the vertex input layout. 

	D3D11_INPUT_ELEMENT_DESC quadLayout[2];

	quadLayout[0].SemanticName = "POSITION";  
    quadLayout[0].SemanticIndex = 0;  
    quadLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  
    quadLayout[0].InputSlot = 0;  
    quadLayout[0].AlignedByteOffset = 0;  
    quadLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;  
    quadLayout[0].InstanceDataStepRate = 0;  


	quadLayout[1].SemanticName = "UV";  
    quadLayout[1].SemanticIndex = 0;  
    quadLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;  
    quadLayout[1].InputSlot = 0;  
    quadLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;  
    quadLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;  
    quadLayout[1].InstanceDataStepRate = 0;

	     
    hr = m_device->CreateInputLayout(quadLayout, 2, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &m_layout);  
    if(FAILED(hr))  
    {  
        return false;  
    }

	VS_Buffer->Release();  
    VS_Buffer = 0;  
  
    PS_Buffer->Release();  
    PS_Buffer = 0;

	return true;

}


bool D3DClass::SetupMRTS(int width, int height)
{
	HRESULT hr;

	// Create depth stencil texture.
    D3D11_TEXTURE2D_DESC dstex;
	ZeroMemory( &dstex, sizeof(dstex) );
    dstex.Width = width;
    dstex.Height = height;
    dstex.MipLevels = 1;
	dstex.ArraySize = 1;
    dstex.SampleDesc.Count = 1;
    dstex.SampleDesc.Quality = 0;
    dstex.Format = DXGI_FORMAT_D32_FLOAT;
    dstex.Usage = D3D11_USAGE_DEFAULT;
    dstex.BindFlags =  D3D11_BIND_DEPTH_STENCIL;
    dstex.CPUAccessFlags = 0;
    //dstex.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

	m_mrtMapDepth = NULL;

    hr =  m_device->CreateTexture2D( &dstex, NULL, &m_mrtMapDepth );

	if(FAILED(hr))
	{
        return false;
	}
    // Create the depth stencil view for the mrts
	m_mrtDSV = NULL;
    hr =  m_device->CreateDepthStencilView( m_mrtMapDepth, 0, &m_mrtDSV );

	if(FAILED(hr))
	{
        return false;
	}

    // Create all the multiple render target textures
    dstex.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
    dstex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    //dstex.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;
    //dstex.MipLevels = 2;
	m_colorTex = NULL;
    hr =  m_device->CreateTexture2D( &dstex, NULL, &m_colorTex );

	if(FAILED(hr))
	{
        return false;
	}

	m_normalTex = NULL;
    hr =  m_device->CreateTexture2D( &dstex, NULL, &m_normalTex );

	if(FAILED(hr))
	{
        return false;
	}

	m_specularTex = NULL;
    hr =  m_device->CreateTexture2D( &dstex, NULL, &m_specularTex );

	if(FAILED(hr))
	{
        return false;
	}

	m_posTex = NULL;
    hr =  m_device->CreateTexture2D( &dstex, NULL, &m_posTex );

	if(FAILED(hr))
	{
        return false;
	}

	// Create the 4 render target view
    D3D11_RENDER_TARGET_VIEW_DESC DescRT;
    DescRT.Format = dstex.Format;
    DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice=0;
    hr =  m_device->CreateRenderTargetView( m_colorTex, &DescRT, &m_colorRTV ); 

	if(FAILED(hr))
	{
        return false;
	}

	hr =  m_device->CreateRenderTargetView( m_normalTex, &DescRT, &m_normalRTV ); 

	if(FAILED(hr))
	{
        return false;
	}

	hr =  m_device->CreateRenderTargetView( m_specularTex, &DescRT, &m_specularRTV ); 

	if(FAILED(hr))
	{
        return false;
	}

	hr =  m_device->CreateRenderTargetView( m_posTex, &DescRT, &m_posRTV ); 

	if(FAILED(hr))
	{
        return false;
	}

	// Create the shader resource view for mrt map
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
	SRVDesc.Format = dstex.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	m_colorSRV = NULL;
    hr =  m_device->CreateShaderResourceView( m_colorTex, &SRVDesc, &m_colorSRV );
	if(FAILED(hr))
	{
        return false;
	}

	m_normalSRV = NULL;
    hr =  m_device->CreateShaderResourceView( m_normalTex, &SRVDesc, &m_normalSRV );
	if(FAILED(hr))
	{
        return false;
	}

	m_specularSRV = NULL;
    hr =  m_device->CreateShaderResourceView( m_specularTex, &SRVDesc, &m_specularSRV );
	if(FAILED(hr))
	{
        return false;
	}

	m_posSRV = NULL;
    hr =  m_device->CreateShaderResourceView( m_posTex, &SRVDesc, &m_posSRV );
	if(FAILED(hr))
	{
        return false;
	}

	// Describe the Shader Resource Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;    
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = m_device->CreateSamplerState( &sampDesc, &m_colorSS );
	if(FAILED(hr))
	{
        return false;
	}

	hr = m_device->CreateSamplerState( &sampDesc, &m_normalSS );
	if(FAILED(hr))
	{
        return false;
	}

	hr = m_device->CreateSamplerState( &sampDesc, &m_specularSS );

	if(FAILED(hr))
	{
        return false;
	}

	hr = m_device->CreateSamplerState( &sampDesc, &m_posSS );

	if(FAILED(hr))
	{
        return false;
	}

	return true;
}

bool D3DClass::InitScene(int screenWidth, int screenHeight)
{


	//Deferred Rendering
	if(!SetupMRTS(screenWidth,screenHeight))
	{
		return false;
	}


	if(!InitializeQuad())
	{
		return false;
	}

	return true;

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

	if(m_colorSS)  
    {  
        m_colorSS->Release();  
        m_colorSS = 0;  
    }

	if(m_normalSS)  
    {  
        m_normalSS->Release();  
        m_normalSS = 0;  
    }

	if(m_specularSS)  
    {  
        m_specularSS->Release();  
        m_specularSS = 0;  
    }

	if(m_posSS)  
    {  
        m_posSS->Release();  
        m_posSS = 0;  
    }

	if(m_mrtDSV)  
    {  
        m_mrtDSV->Release();  
        m_mrtDSV = 0;  
    }

	if(m_mrtMapDepth)  
    {  
        m_mrtMapDepth->Release();  
        m_mrtMapDepth = 0;  
    }

	if(m_colorRTV)  
    {  
        m_colorRTV->Release();  
        m_colorRTV = 0;  
    } 

	if(m_normalRTV)  
    {  
        m_normalRTV->Release();  
        m_normalRTV = 0;  
    }

	if(m_specularRTV)  
    {  
        m_specularRTV->Release();  
        m_specularRTV = 0;  
    }

	if(m_posRTV)  
    {  
        m_posRTV->Release();  
        m_posRTV = 0;  
    }

	if(m_colorSRV)  
    {  
        m_colorSRV->Release();  
        m_colorSRV = 0;  
    }

	if(m_normalSRV)  
    {  
        m_normalSRV->Release();  
        m_normalSRV = 0;  
    }

	if(m_specularSRV)  
    {  
        m_specularSRV->Release();  
        m_specularSRV = 0;  
    }

	if(m_posSRV)  
    {  
        m_posSRV->Release();  
        m_posSRV = 0;  
    }

	if(m_colorTex)  
    {  
        m_colorTex->Release();  
        m_colorTex = 0;  
    } 

	if(m_normalTex)  
    {  
        m_normalTex->Release();  
        m_normalTex = 0;  
    } 

	if(m_specularTex)  
    {  
        m_specularTex->Release();  
        m_specularTex = 0;  
    } 

	if(m_posTex)  
    {  
        m_posTex->Release();  
        m_posTex = 0;  
    } 

	if(m_quadVB)  
    {  
        m_quadVB->Release();  
        m_quadVB = 0;  
    }

	if(m_quadIB)  
    {  
        m_quadIB->Release();  
        m_quadIB = 0;  
    }

	if(m_finalVS)  
    {  
        m_finalVS->Release();  
        m_finalVS = 0;  
    }

	if(m_finalPS)  
    {  
        m_finalPS->Release();  
        m_finalPS = 0;  
    }

	if(m_layout)  
    {  
        m_layout->Release();  
        m_layout = 0;  
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
