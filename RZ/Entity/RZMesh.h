#pragma once
#ifndef RZMESH_H
#define RZMESH_H
#include "windows.h"
#include "d3d11.h"
#include "d3dx10math.h"
#include "RZMaterial.h"
#include <stdio.h>


using namespace std;

namespace RZ
{
	#define RZMESH_FILE_VERSION 101
	#define MAX_VERTEX_ELEMENTS 32
	#define MAX_VERTEX_STREAMS 16
	#define MAX_MESH_NAME 100
	#define MAX_SUBSET_NAME 100
	#define MAX_MATERIAL_NAME 100
	#define MAX_SUBSET_NUM 32
	#define INVALID_FRAME ((UINT)-1)
	#define INVALID_MESH ((UINT)-1)
	#define INVALID_MATERIAL ((UINT)-1)
	#define INVALID_SUBSET ((UINT)-1)
	#define INVALID_ANIMATION_DATA ((UINT)-1)
	#define INVALID_SAMPLER_SLOT ((UINT)-1)
	#define ERROR_RESOURCE_VALUE 1


	template<typename TYPE> BOOL IsErrorResource( TYPE data )
	{
		if( ( TYPE )ERROR_RESOURCE_VALUE == data )
			return TRUE;
		return FALSE;
	}

	#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
	#endif

	enum RZMESH_INDEX_TYPE
	{
		IT_16BIT = 0,
		IT_32BIT,
	};


	struct RZMESH_HEADER
	{
		//Basic Info and sizes
		UINT Version;
		UINT64 HeaderSize;
		UINT64 NonBufferDataSize;
		UINT64 BufferDataSize;

		//Stats
		UINT NumVertexBuffers;
		UINT NumIndexBuffers;
		UINT NumMeshes;
		UINT NumTotalSubsets;
		UINT NumMaterials;

		//Offsets to Data
		UINT64 VertexStreamHeadersOffset;
		UINT64 IndexStreamHeadersOffset;
		UINT64 MeshDataOffset;
		UINT64 MaterialDataOffset;
		UINT64 SubsetDataOffset;
	};

	struct RZMESH_VERTEX_BUFFER_HEADER
	{
		UINT64 NumVertices;
		UINT64 SizeBytes;
		UINT64 StrideBytes;
		union
		{
			UINT64 DataOffset;			//(This also forces the union to 64bits)
			ID3D11Buffer* pVB11;
		};
	};

	struct RZMESH_INDEX_BUFFER_HEADER
	{
		UINT64 NumIndices;
		UINT64 SizeBytes;
		UINT IndexType;
		union
		{
			UINT64 DataOffset;				//(This also forces the union to 64bits)
			ID3D11Buffer* pIB11;
		};
	};

	struct RZMESH_MESH
	{
		char Name[MAX_MESH_NAME];
		UINT VertexBuffer;
		UINT IndexBuffer;
		UINT NumSubsets;

		D3DXMATRIX TransformMatrix;
		union
		{
			UINT64 SubsetOffset;	//Offset to list of subsets (This also forces the union to 64bits)
			UINT* pSubsets;	    //Pointer to list of subsets
		};
	};

	struct RZMESH_SUBSET
	{
		char Name[MAX_SUBSET_NAME];
		UINT MaterialID;
		D3D11_PRIMITIVE_TOPOLOGY PrimitiveType;
		UINT64 IndexStart;
		UINT64 IndexCount;
		UINT64 VertexStart;
		UINT64 VertexCount;
	};

	struct RZMESH_MATERIAL
	{
		char Name[MAX_MATERIAL_NAME];
		UINT NumSubsets;
		UINT Subsets[MAX_SUBSET_NUM];

	};

	class RZVertexType
	{
	public:
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXCOLOR color;
		D3DXVECTOR2 uv;
	};

	class RZMesh
	{
	private:
		char* m_pStaticMeshData;
		char** m_ppVertices;
		char** m_ppIndices;
		char* m_pVertices;
		char* m_pIndices;
		//General mesh info
		RZMESH_HEADER* m_pMeshHeader;
		RZMESH_VERTEX_BUFFER_HEADER* m_pVertexBufferArray;
		RZMESH_INDEX_BUFFER_HEADER* m_pIndexBufferArray;
		RZMESH_MESH* m_pMeshArray;
		RZMESH_MATERIAL* m_pMaterialArray;
		RZMESH_SUBSET* m_pSubsetArray;

		RZMaterial** m_matList;

	public:
		RZMesh();
		RZMesh(const RZMesh& other);
		virtual ~RZMesh();
		
		bool CreateVertexBuffer( ID3D11Device* pd3dDevice,RZMESH_VERTEX_BUFFER_HEADER* pHeader, void* pVertices);
		bool CreateIndexBuffer( ID3D11Device* pd3dDevice,RZMESH_INDEX_BUFFER_HEADER* pHeader,void* pIndices);

		bool CreateFromFile(ID3D11Device* device, char* modelFilename);
		bool CreateFromMemory(ID3D11Device* device, char* modelMeshData);

		bool SetMaterial(UINT id, RZMaterial* pMat);

		bool ReleaseModel();

		bool Render(ID3D11DeviceContext* pd3dDeviceContext,D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

		bool RenderMesh(UINT iMesh,ID3D11DeviceContext* pd3dDeviceContext,D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

		
		//Debug
		void DebugBuffer(ID3D11Device* device,ID3D11DeviceContext* pd3dDeviceContext);

	};
};
#endif