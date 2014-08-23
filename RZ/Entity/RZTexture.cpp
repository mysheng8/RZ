#include"RZTexture.h"

using namespace RZ;

RZTexture::RZTexture()
{
	m_pShaderResourceView=0;
	m_pSampler=0;
}

RZTexture::RZTexture(const RZTexture& other)
{
}


RZTexture::~RZTexture()
{
}

bool RZTexture::Initialize(ID3D11Device* device,char* texFileName)
{
	HRESULT result;  
	D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory( &samplerDesc, sizeof( samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result=device->CreateSamplerState( &samplerDesc, &m_pSampler);

	if(FAILED(result))
    {
        return false;
    }

	result = D3DX11CreateShaderResourceViewFromFile( device, texFileName, 0, 0, &m_pShaderResourceView, 0);
    if( FAILED( result))
    {
        return false;
    } 

}

void RZTexture::Release()
{
	if(m_pSampler)
	{
		m_pSampler->Release();  
		m_pSampler=0;
	}

	if(m_pShaderResourceView)
	{
		m_pShaderResourceView->Release();
		m_pShaderResourceView=0;
	}
}

void RZTexture::Render(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->PSSetSamplers(0,1,&m_pSampler);
	pDeviceContext->PSSetShaderResources(0,1,&m_pShaderResourceView);
}