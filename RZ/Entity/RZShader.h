#pragma once
#ifndef _RZShader_H_
#define _RZShader_H_

#include <d3d11.h>  
#include <d3dx10math.h>  
#include <d3dx11async.h>  
#include <fstream>

namespace RZ
{

	struct MatrixBufferType  
	{  
		D3DXMATRIX world;  
		D3DXMATRIX view;  
		D3DXMATRIX projection; 
	};

	class RZShader
	{
	public:
		RZShader();  
		RZShader(const RZShader&);  
		~RZShader();
		bool Initialize(ID3D11Device* device, HWND hwnd, ID3D11Buffer* matrixBuffer, CHAR* vsFilename, CHAR* psFilename);  
		bool Render(ID3D11DeviceContext*, int indexCount, int indexStart, int vertexStart, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
		void Shutdown(); 

	private:
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, CHAR*);  
		bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);  
		void RenderShader(ID3D11DeviceContext*, int indexCount, int indexStart, int vertexStart);

		ID3D11VertexShader* m_vertexShader;  
		ID3D11PixelShader* m_pixelShader;  
		ID3D11InputLayout* m_layout;  
		ID3D11Buffer* m_matrixBuffer;

	};
};
#endif