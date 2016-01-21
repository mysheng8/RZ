#pragma once
#ifndef _RZLINE_H_
#define _RZLINE_H_

#include "..\Framework\RZCommon.h";

#include "RZShader.h"
#include <fstream>


using namespace std;
namespace RZ
{
	struct LineCBT
	{
		XMMATRIX wvp;
		XMFLOAT4 campos;
		XMFLOAT4 lineDir;
	};

	const int numCopys = 128;

	struct InstanceCBT
	{
		XMFLOAT2 linesPos[numCopys];
	};

	class RZLineKnot
	{
	public:
		XMFLOAT4 position;
		XMFLOAT4 color;
		XMFLOAT2 uv;

		RZLineKnot();
		RZLineKnot(float px,float py,float pz, float cx,float cy, float cz, float u, float v);
		RZLineKnot(const RZLineKnot& other);
		RZLineKnot& operator = (const RZLineKnot&);
		~RZLineKnot();

	};


	class RZLine
	{
	public:
		RZLine();
		RZLine(const RZLine& other);
		virtual ~RZLine();
		bool Initialize(ID3D11Device* pd3dDevice, HWND hwnd);
		bool CreateLine(UINT length, RZLineKnot *data);

		//bool CreateIndexBuffer(ID3D11Device* pd3dDevice);
		void RenderLine(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		void RenderLineCluster(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		void RenderTessellatedLine(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		void RenderTessellatedLineCluster(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		void Release();
	private:

		ID3D11Device*			m_device;
		vector<RZLineKnot>		m_data;
		UINT					m_length;
		ID3D11Buffer*			m_vertexBuffer;
		ID3D11Buffer*			m_matrixBuffer;
		ID3D11Buffer*			m_instanceBuffer;

		//shaders
		ID3D11HullShader*		m_lineHS;
		ID3D11DomainShader*		m_lineDS;
		ID3D11VertexShader*		m_lineVS;
		ID3D11VertexShader*		m_lineClusterVS;//for draw instance
		ID3D11GeometryShader*	m_lineGS;
		ID3D11PixelShader*		m_linePS;
		ID3D11InputLayout*		m_layout;

		bool CreateVertexBuffer();
		bool SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, const CHAR*);
	};
};
#endif