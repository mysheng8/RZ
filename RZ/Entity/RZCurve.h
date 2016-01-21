#pragma once
#ifndef _RZCURVE_H_
#define _RZCURVE_H_

#include "..\Framework\RZCommon.h";
#include "RZShader.h"
#include <fstream>
using namespace std;
namespace RZ
{
	struct CurveCBT
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};


	class RZCurveKnot
	{
	public:
		XMFLOAT4 position;
		XMFLOAT4 color;
		XMFLOAT2 uv;

		RZCurveKnot();
		RZCurveKnot(float px, float py, float pz, float cx, float cy, float cz, float u, float v);
		RZCurveKnot(const RZCurveKnot& other);
		RZCurveKnot& operator = (const RZCurveKnot&);
		~RZCurveKnot();

	};


	class RZCurve
	{
	public:
		RZCurve();
		RZCurve(const RZCurve& other);
		virtual ~RZCurve();
		bool Initialize(ID3D11Device* pd3dDevice, HWND hwnd);
		bool CreateLine(UINT length, RZCurveKnot *data);

		//bool CreateIndexBuffer(ID3D11Device* pd3dDevice);
		void RenderLine(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		void RenderTessellatedLine(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		void Release();
	private:

		ID3D11Device*			m_device;
		vector<RZCurveKnot>		m_data;
		UINT					m_length;
		ID3D11Buffer*			m_vertexBuffer;
		ID3D11Buffer*			m_matrixBuffer;

		//shaders
		ID3D11HullShader*		m_lineHS;
		ID3D11DomainShader*		m_lineDS;
		ID3D11VertexShader*		m_lineVS;
		ID3D11GeometryShader*	m_lineGS;
		ID3D11PixelShader*		m_linePS;
		ID3D11InputLayout*		m_layout;

		bool CreateVertexBuffer();
		bool SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, const CHAR*);
	};
};
#endif