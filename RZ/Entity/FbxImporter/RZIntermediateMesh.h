#pragma once
#ifndef RZINTERMEDIATEMESH_H
#define RZINTERMEDIATEMESH_H

#include <stdio.h>
#include <vector>
#include"../RZMesh.h"

using namespace std;

#define WRITE_ASCII 0;

namespace RZ
{
	struct INTERMEDIATE_VERTEX_BUFFER : public RZMESH_VERTEX_BUFFER_HEADER
	{
		void* pVertices;
	};

	struct INTERMEDIATE_INDEX_BUFFER : public RZMESH_INDEX_BUFFER_HEADER
	{
		void* pIndices;
	};


	struct INTERMEDIATE_MESH : public RZMESH_MESH
	{
		vector<UINT> SubsetIDArray;
	};

	struct INTERMEDIATE_SUBSET : public RZMESH_SUBSET
	{
	};

	struct INTERMEDIATE_MATERIAL : public RZMESH_MATERIAL
	{
		UINT matID;
		vector<UINT> SubsetIDArray;
	};

	class RZIntermediateMesh
	{
	public:
		RZIntermediateMesh(){};
		virtual ~RZIntermediateMesh(){};
		bool SaveAsRZMesh( char* szFileName );
		bool SaveAsRZMeshASCII( char* szFileName );
		INTERMEDIATE_VERTEX_BUFFER* AddVertexBuffer(   INTERMEDIATE_MESH* pMesh,UINT StreamID,UINT64 NumVertices,UINT64 SizeBytes,UINT64 StrideBytes,void* pBinaryData );
		INTERMEDIATE_INDEX_BUFFER* AddIndexBuffer(	 INTERMEDIATE_MESH* pMesh,UINT64 NumIndices,UINT64 SizeBytes,UINT IndexType,void* pBinaryData );
		INTERMEDIATE_MESH* AddMesh( const char* pszName, D3DXMATRIX TransformMatrix);
		INTERMEDIATE_MATERIAL* AddMaterial(const char* pszName);
		INTERMEDIATE_MATERIAL* GetMaterial(const char* pszName);
		INTERMEDIATE_SUBSET* AddSubset( INTERMEDIATE_MESH* pMesh,char* pszName,UINT MaterialID,D3D11_PRIMITIVE_TOPOLOGY PrimitiveType,UINT64 IndexStart,UINT64 IndexCount,UINT64 VertexStart,UINT64 VertexCount );

	private:
		vector<INTERMEDIATE_VERTEX_BUFFER*>	m_VertexBufferArray;
		vector<INTERMEDIATE_INDEX_BUFFER*>  m_IndexBufferArray;
		vector<INTERMEDIATE_MESH*>			m_MeshArray;
		vector<INTERMEDIATE_MATERIAL*>		m_MaterialArray;
		vector<INTERMEDIATE_SUBSET*>		m_SubsetArray;
	};
};

#endif