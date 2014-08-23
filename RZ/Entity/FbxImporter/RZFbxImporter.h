#pragma once
#ifndef RZFBXIMPORTER_H
#define RZFBXIMPORTER_H

#include<map>
#include <fbxsdk.h>
#include "..\RZmesh.h"
#include "RZIntermediateMesh.h"
#include<string>


namespace RZ
{
	class RZFbxImporter
	{
	public:

		class VertexData
		{
		public:
			FbxVector4 pos;
			FbxVector4 normal;
			FbxColor color;

			FbxVector2 uv1;
			FbxVector2 uv2;
			FbxVector2 uv3;

			bool operator==(const VertexData &rhs)
			{
				return this->pos==rhs.pos&&this->normal==rhs.normal&&this->color==rhs.color&&this->uv1==rhs.uv1&&this->uv2==rhs.uv2&&this->uv3==rhs.uv3;
			};
			bool operator!=(const VertexData &rhs)
			{
				return !(*this==rhs);
			};
			//convert from maya coordinate to directx coordinate
			void ConvertCoordinateSystem()
			{
				pos[2] = pos[2] * -1.0;
				normal[2]=normal[2]*-1.0;
				uv1[1] = 1.0 -uv1[1];
				uv2[1] = 1.0 -uv2[1];
				uv3[1] = 1.0 -uv3[1];
			};
		};

		class VertexDesc
		{
		public:
			bool CopyDataToBuffer(FbxVector4 pos,VertexData* info);
			bool GetVerticesFromBuffer(void** pVertices);
			bool GetIndicesFromBuffer(void** pIndices);
			int GetVerticesCount(){return m_VertexInfos.size();};
			int GetIndicesCount(){return m_Indices.size();};
			void AddSubsets(){m_SubsetVertexPos.push_back(m_VertexInfos.size());m_SubsetIndexPos.push_back(m_Indices.size());};
			int GetSubsetsCount(){return m_SubsetVertexPos.size();};
			int GetSubsetsVertexPos(int i){return m_SubsetVertexPos[i];};
			int GetSubsetsIndexPos(int i){return m_SubsetIndexPos[i];};
		private:
			map<string,vector<int>> m_VertexDataMap;
			vector<VertexData> m_VertexInfos;
			vector<int> m_Indices;
			vector<int> m_SubsetVertexPos;
			vector<int> m_SubsetIndexPos;
		};

		class MeshInfo
		{
		public:
			int matCount;
			int uvsetCount;
			bool hasVectorColor;
			bool hasNormal;
			bool hasTangent;
			bool hasSkinWeight;
			bool GetMeshDesc(FbxMesh* pFbxMesh);
			vector<string> matList;
		};

		RZFbxImporter(){m_pRZIntermediateMesh=NULL;pVertDesc=new VertexDesc();};
		~RZFbxImporter();
		void init();
		void LoadMeshFromFile(const char* lFilename);
		bool ConvertToRZMesh(FbxNode* pNode,int &currentVertexPos, int &currentIndexPos);
		RZIntermediateMesh* GetMesh(){return m_pRZIntermediateMesh;};

	private:
		FbxManager* lSdkManager;
		FbxScene* lScene;
		MeshInfo* pInfo;
		VertexDesc* pVertDesc;

		bool GetMeshData(FbxMesh* pInMesh, VertexDesc* pVertDesc);
		bool WriteMeshData(INTERMEDIATE_MESH* &pOutMesh,VertexDesc* pVertDesc);
		bool readPos(FbxMesh* pFbxMesh,int ctrlPointIndex,FbxVector4 &outPos);
		bool readUV(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,int layer,FbxVector2 &outUV);
		bool readNM(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,FbxVector4 &outNM);
		bool readColor(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,FbxColor &outColor);
	
	protected:
		RZIntermediateMesh*  m_pRZIntermediateMesh;
	};

};

#endif