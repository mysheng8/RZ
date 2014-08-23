#ifndef _RZShader_H_
#define _RZShader_H_

#include <d3d11.h>  
#include <d3dx10math.h>  
#include <d3dx11async.h>  
#include <fstream>

namespace RZ
{

	class RZShader
	{
	public:
		RZShader();  
		RZShader(const RZShader&);  
		~RZShader();

		bool Initialize(ID3D11Device*, HWND);  
		void Shutdown();  
		bool Render(ID3D11DeviceContext*, int indexCount, int indexStart, int vertexStart, D3DXMATRIX, D3DXVECTOR4, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
		bool InitializeShader(ID3D11Device*, HWND, CHAR*, CHAR*);

	private:
		struct MatrixBufferType  
		{  
			D3DXMATRIX worldViewProjection;
			D3DXVECTOR4 lightDir;
			D3DXMATRIX world;  
			D3DXMATRIX view;  
			D3DXMATRIX projection; 
		};

		  
		void ShutdownShader();  
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, CHAR*);  
  
		bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXVECTOR4, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);  
		void RenderShader(ID3D11DeviceContext*, int indexCount, int indexStart, int vertexStart);

		ID3D11VertexShader* m_vertexShader;  
		ID3D11PixelShader* m_pixelShader;  
		ID3D11InputLayout* m_layout;  
		ID3D11Buffer* m_matrixBuffer;

	};
};
#endif