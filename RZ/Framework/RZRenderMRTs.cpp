#include "RZRenderMRTs.h"

using namespace RZ;

RZRenderMRTs* RZRenderMRTs::m_instance = NULL;

RZRenderMRTs::RZRenderMRTs()
{
}

RZRenderMRTs::RZRenderMRTs(const RZRenderMRTs&)
{
}

bool RZRenderMRTs::Initialize(ID3D11Device* device,HWND hwnd,int screenWidth, int screenHeight)
{
	m_device=device;

	//Deferred Rendering
	m_hwnd=hwnd;

	if(!InitializeQuad())
	{
		return false;
	}

	if(!SetupMRTS(screenWidth,screenHeight))
	{
		return false;
	}

	if(!InitializeShader())
	{
		return false;
	}

	return true;
}

bool RZRenderMRTs::InitializeShader()
{
	HRESULT result;  
    ID3D10Blob* errorMessage;  
    ID3D10Blob* vertexShaderBuffer;  
    ID3D10Blob* pixelShaderBuffer; 

    // Initialize the pointers this function will use to null.  
    errorMessage = 0;  
    vertexShaderBuffer = 0;  
    pixelShaderBuffer = 0;  

	
	//generate Point Light shaders
	/*D3D10_SHADER_MACRO Shader_Macros[1];
	Shader_Macros[0].Name = "POINTLIGHT";
	Shader_Macros[0].Definition = "1";*/
	D3D_SHADER_MACRO Shader_Macros[]={"POINTLIGHT","1",NULL, NULL};
	/*
    result = D3DX11CompileFromFile(lightVS, Shader_Macros, NULL, "LightingVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &vertexShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        return false;  
    }  
	*/
    result = D3DX11CompileFromFile(lightPS, Shader_Macros, NULL, "LightingPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        if(errorMessage)  
        {  
            OutputShaderErrorMessage(errorMessage, m_hwnd, lightPS);  
        }  
        else  
        {  
            MessageBox(m_hwnd, lightPS, "Missing Shader File", MB_OK);  
        }  
  
        return false;  
    }   
	/*
    result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pointLightVS);  
    if(FAILED(result))  
    {  
        return false;  
    }  
	*/
    result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pointLightPS);  
    if(FAILED(result))  
    {  
        return false;  
    } 

	
	//generate spot Light shaders
	Shader_Macros[0].Name = "SPOTLIGHT";
	/*
    result = D3DX11CompileFromFile(lightVS, Shader_Macros, NULL, "LightingVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &vertexShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        return false;  
    }  
	*/
    result = D3DX11CompileFromFile(lightPS, &Shader_Macros[0], NULL, "LightingPS", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,&pixelShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        if(errorMessage)  
        {  
            OutputShaderErrorMessage(errorMessage, m_hwnd, lightPS);  
        }  
        else  
        {  
            MessageBox(m_hwnd, lightPS, "Missing Shader File", MB_OK);  
        }  
  
        return false;  
    }  
	/*
    result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_spotLightVS);  
    if(FAILED(result))  
    {  
        return false;  
    }  
	*/
    result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_spotLightPS);  
    if(FAILED(result))  
    {  
        return false;  
    }  

	//generate direction Light shaders
	Shader_Macros[0].Name = "DIRECTIONALLIGHT";
	/*
    result = D3DX11CompileFromFile(lightVS, Shader_Macros, NULL, "LightingVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &vertexShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        return false;  
    }  
	*/
    result = D3DX11CompileFromFile(lightPS, &Shader_Macros[0], NULL, "LightingPS", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        if(errorMessage)  
        {  
            OutputShaderErrorMessage(errorMessage, m_hwnd, lightPS);  
        }  
        else  
        {  
            MessageBox(m_hwnd, lightPS, "Missing Shader File", MB_OK);  
        }  
  
        return false;  
    } 
	/*
    result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_directionLightVS);  
    if(FAILED(result))  
    {  
        return false;  
    }  
	*/
    result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_directionLightPS);  
    if(FAILED(result))  
    {  
        return false;  
    } 

		//generate final shader
	result = D3DX11CompileFromFile(finalVS, 0, 0, "RenderToQuadVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0,&vertexShaderBuffer, &errorMessage, 0); 

    if(FAILED(result))  
    {   
        if(errorMessage)  
        {  
			OutputShaderErrorMessage(errorMessage, m_hwnd, finalVS);
        }  
        else  
        {  
			MessageBox(m_hwnd, finalVS, "Missing Shader File", MB_OK);
        }  
  
        return false;  
    } 

	//generate final shader
	result = D3DX11CompileFromFile(finalVS, 0, 0, "RenderToQuadVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0,&vertexShaderBuffer, &errorMessage, 0); 

    if(FAILED(result))  
    {   
        if(errorMessage)  
        {  
			OutputShaderErrorMessage(errorMessage, m_hwnd, finalVS);
        }  
        else  
        {  
			MessageBox(m_hwnd, finalVS, "Missing Shader File", MB_OK);
        }  
  
        return false;  
    } 

	result = D3DX11CompileFromFile(finalPS, 0, 0, "RenderToQuadPixelShader", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0, &pixelShaderBuffer, &errorMessage, 0); 

    if(FAILED(result))  
    {   
        if(errorMessage)  
        {  
			OutputShaderErrorMessage(errorMessage, m_hwnd, finalPS);
        }  
        else  
        {  
			MessageBox(m_hwnd, finalPS, "Missing Shader File", MB_OK);
        }  
  
        return false;  
    } 

	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_finalVS);  
    if(FAILED(result))  
    {  
        return false;  
    }  
  
    result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_finalPS);  
    if(FAILED(result))  
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

	     
    result = m_device->CreateInputLayout(quadLayout, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);  
    if(FAILED(result))  
    {  
        return false;  
    }


	vertexShaderBuffer->Release();  
    vertexShaderBuffer = 0;  
  
    pixelShaderBuffer->Release();  
    pixelShaderBuffer = 0; 

	return true;
	
}


bool RZRenderMRTs::InitializeQuad()
{
	HRESULT hr;

	// initialize the quad

	// the vertex info
	vector<RZQuadVPS> _quadVertices;

	RZQuadVPS v[4];

	/*
	  2----3
	  |    |
	  0----1 */

	v[0].Pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	v[0].TexCoord = XMFLOAT2(0, 1.0f);

	v[1].Pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	v[1].TexCoord = XMFLOAT2(0, 0);

	v[2].Pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
	v[2].TexCoord = XMFLOAT2(1.0f, 0);

	v[3].Pos = XMFLOAT3(1.0f, -1.0f, -1.0f);
	v[3].TexCoord = XMFLOAT2(1, 1);

	_quadVertices.push_back(v[0]);
	_quadVertices.push_back(v[1]);
	_quadVertices.push_back(v[2]);
	_quadVertices.push_back(v[3]);

	// setup the vertex buffer
	m_numVQuad = _quadVertices.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(RZQuadVPS) * m_numVQuad;
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
	cbbd.ByteWidth = sizeof(RZQuadCB);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = m_device->CreateBuffer(&cbbd, NULL, &m_matrixCB);
	
    if(FAILED(hr))
	{
        return false;
	}

	cbbd.ByteWidth = sizeof(RZLightParams);

	hr = m_device->CreateBuffer(&cbbd, NULL, &m_lightingCB);
	
    if(FAILED(hr))
	{
        return false;
	}

	return true;

}


bool RZRenderMRTs::SetupMRTS(int width, int height)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC dstex;
	ZeroMemory( &dstex, sizeof(dstex) );

	dstex.Width = width;
    dstex.Height = height;
    dstex.MipLevels = 1;
	dstex.ArraySize = 1;
    dstex.SampleDesc.Count = 1;
    dstex.SampleDesc.Quality = 0;
    dstex.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
    dstex.Usage = D3D11_USAGE_DEFAULT;
    dstex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    dstex.CPUAccessFlags = 0;


	/*
	// Create depth stencil texture.
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
	*/
    // Create all the multiple render target textures
    //dstex.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
    //dstex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
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

	m_lightingTex = NULL;
    hr =  m_device->CreateTexture2D( &dstex, NULL, &m_lightingTex );

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

	hr =  m_device->CreateRenderTargetView( m_lightingTex, &DescRT, &m_lightingRTV ); 

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

	m_lightingSRV = NULL;
    hr =  m_device->CreateShaderResourceView( m_lightingTex, &SRVDesc, &m_lightingSRV );
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
	hr = m_device->CreateSamplerState( &sampDesc, &m_samplerState );
	if(FAILED(hr))
	{
        return false;
	}
	/*
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
	*/
	return true;
}



void RZRenderMRTs::RenderLightingPass(ID3D11DeviceContext* deviceContext,int type,const XMFLOAT3& lightPos, const XMFLOAT3& lightDir,const XMFLOAT3& lightColor,const XMFLOAT3& lightRange)
{
	//render quad
	deviceContext->VSSetShader(m_finalVS, 0, 0);
	switch(type)
	{
	case(1):
		{
			//deviceContext->VSSetShader(m_pointLightVS, 0, 0);
			deviceContext->PSSetShader(m_pointLightPS, 0, 0);
			break;
		}
	case(2):
		{
			//deviceContext->VSSetShader(m_spotLightVS, 0, 0);
			deviceContext->PSSetShader(m_spotLightPS, 0, 0);
			break;
		}
	case(3):
		{
			//deviceContext->VSSetShader(m_directionLightVS, 0, 0);
			deviceContext->PSSetShader(m_directionLightPS, 0, 0);
			break;
		}
	default:
		{
			return;
		}
	}


	UINT stride, offset;
	stride = sizeof(RZQuadVPS);
    offset = 0;
	deviceContext->IASetInputLayout(m_layout);
	deviceContext->IASetIndexBuffer( m_quadIB, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers( 0, 1, &m_quadVB, &stride, &offset );
	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//update constant buffer
	RZQuadCB cbt;
	
	XMMATRIX WVP;
	WVP=XMMatrixTranslation( 0.5f, -0.5f, 0.0f );
	WVP=XMMatrixScaling( 1.0f, 1.0f, 0.0f );
	WVP=XMMatrixTranspose(WVP);

	cbt.wvp=WVP;
	deviceContext->UpdateSubresource( m_matrixCB, 0, NULL, &cbt, 0, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &m_matrixCB );
	
	RZLightParams cbl;
	cbl.lightColor=XMLoadFloat3(&lightColor);
	cbl.lightDir=XMLoadFloat3(&lightDir);
	cbl.lightPos=XMLoadFloat3(&lightPos);
	cbl.lightRange=XMLoadFloat3(&lightRange);
	deviceContext->UpdateSubresource( m_lightingCB, 0, NULL, &cbl, 0, 0 );
	deviceContext->PSSetConstantBuffers( 1, 1, &m_lightingCB );


	deviceContext->PSSetShaderResources( 0, 1, &m_colorSRV );	// Draw the color map to the square
	deviceContext->PSSetShaderResources( 1, 1, &m_normalSRV );	// Draw the normal map to the square
	deviceContext->PSSetShaderResources( 2, 1, &m_specularSRV );	// Draw the specular map to the square
	deviceContext->PSSetShaderResources( 3, 1, &m_posSRV );	// Draw the pos map to the square
	deviceContext->PSSetSamplers( 0, 1, &m_samplerState );

	//Draw the second cube
	deviceContext->DrawIndexed( 6, 0, 0 );

}

void RZRenderMRTs::RenderFinalPass(ID3D11DeviceContext* deviceContext)
{
//render quad
	deviceContext->VSSetShader(m_finalVS, 0, 0);
	deviceContext->PSSetShader(m_finalPS, 0, 0);

	UINT stride, offset;
	stride = sizeof(RZQuadVPS);
    offset = 0;

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->IASetIndexBuffer( m_quadIB, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers( 0, 1, &m_quadVB, &stride, &offset );
	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//update constant buffer
	RZQuadCB cbt;

	XMMATRIX WVP;
	WVP = XMMatrixTranslation(0.5f, -0.5f, 0.0f);
	WVP = XMMatrixScaling(1.0f, 1.0f, 0.0f);
	WVP = XMMatrixTranspose(WVP);

	cbt.wvp = WVP;
	deviceContext->UpdateSubresource(m_matrixCB, 0, NULL, &cbt, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_matrixCB);

	deviceContext->PSSetShaderResources( 0, 1, &m_colorSRV );
	deviceContext->PSSetShaderResources( 1, 1, &m_lightingSRV );
	deviceContext->PSSetSamplers( 0, 1, &m_samplerState );

	//Draw the second cube
	deviceContext->DrawIndexed( 6, 0, 0 );
	
}

void RZRenderMRTs::ClearMRTs(ID3D11DeviceContext* deviceContext)
{
	// Clear render targets
	const float clearColor[4] = {0.f, 0.f, 0.f, 0.f};
	deviceContext->ClearRenderTargetView(m_colorRTV, clearColor); 
	deviceContext->ClearRenderTargetView(m_specularRTV, clearColor);
	deviceContext->ClearRenderTargetView(m_lightingRTV, clearColor);

	const float clearNormal[4] = {0.5f, 0.5f, 0.5f, 0.f};
	deviceContext->ClearRenderTargetView(m_normalRTV, clearNormal);

	const float clearPos[4] = {0.f,0.f,0.f,1.f};
	deviceContext->ClearRenderTargetView(m_posRTV, clearPos);

	//deviceContext->ClearDepthStencilView(m_mrtDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

ID3D11RenderTargetView* RZRenderMRTs::GetRenderTargetView(int type)
{
	switch(type)
	{
	case 0:
		return m_lightingRTV;
	case 1:
		return m_colorRTV;
	case 2:
		return m_normalRTV;
	case 3:
		return m_specularRTV;
	case 4:
		return m_posRTV;
	default:
		return 0;
	}
}

void RZRenderMRTs::ShutDown()
{
	RZRELEASE(m_samplerState);
	RZRELEASE(m_colorRTV);
	RZRELEASE(m_normalRTV);
	RZRELEASE(m_specularRTV);
	RZRELEASE(m_posRTV);
	RZRELEASE(m_lightingRTV);
	RZRELEASE(m_colorSRV);
	RZRELEASE(m_normalSRV);
	RZRELEASE(m_specularSRV);
	RZRELEASE(m_posSRV);
	RZRELEASE(m_lightingSRV);
	RZRELEASE(m_colorTex);
	RZRELEASE(m_normalTex);
	RZRELEASE(m_specularTex);
	RZRELEASE(m_posTex);
	RZRELEASE(m_lightingTex);
	RZRELEASE(m_quadVB);
	RZRELEASE(m_quadIB);
	RZRELEASE(m_finalVS);
	RZRELEASE(m_finalPS);
	//RZRELEASE(m_pointLightVS);
	RZRELEASE(m_pointLightPS);
	//RZRELEASE(m_spotLightVS);
	RZRELEASE(m_spotLightPS);
	//RZRELEASE(m_directionLightVS);
	RZRELEASE(m_directionLightPS);
	RZRELEASE(m_layout);
	return;
}


void RZRenderMRTs::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd,const CHAR* shaderFilename)  
{  
    char* compileErrors;  
    unsigned long bufferSize, i;  
    ofstream fout;  
  
  
    // Get a pointer to the error message text buffer.  
    compileErrors = (char*)(errorMessage->GetBufferPointer());  
  
    // Get the length of the message.  
    bufferSize = errorMessage->GetBufferSize();  
  
    // Open a file to write the error message to.  
    fout.open("shader-error.txt");  
  
    // Write out the error message.  
    for(i=0; i<bufferSize; i++)  
    {  
        fout << compileErrors[i];  
    }  
  
    // Close the file.  
    fout.close();  
  
    // Release the error message.  
    errorMessage->Release();  
    errorMessage = 0;  
  
    // Pop a message up on the screen to notify the user to check the text file for compile errors.  
    MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);  
  
    return;  
}