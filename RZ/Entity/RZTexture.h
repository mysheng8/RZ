#ifndef _RZTEXTURE_H_
#define _RZTEXTURE_H_

#include <d3d11.h>  
#include <d3dx11async.h>  

namespace RZ
{

	class RZTexture
	{
	public:
		RZTexture();
		RZTexture(const RZTexture &other);
		~RZTexture();
		bool Initialize(ID3D11Device* device, char* texFileName);
		void Render(ID3D11DeviceContext* pDeviceContext);
		ID3D11ShaderResourceView* GetShaderResourceView(){return m_pShaderResourceView;};
		void Release();

	private:
		ID3D11ShaderResourceView* m_pShaderResourceView;
		ID3D11SamplerState*	m_pSampler;            

	};
};

#endif