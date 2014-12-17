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
		//define class
		class RZInVertex
		{
		public:
			FbxVector4 pos;
			FbxVector4 normal;
			FbxVector4 tangent;
			FbxColor color;

			FbxVector2 uv1;
			FbxVector2 uv2;
			FbxVector2 uv3;

			bool operator==(const RZInVertex &rhs)
			{
				return this->pos==rhs.pos&&this->normal==rhs.normal&&this->tangent==rhs.tangent&&this->color==rhs.color&&this->uv1==rhs.uv1&&this->uv2==rhs.uv2&&this->uv3==rhs.uv3;
			};
			bool operator!=(const RZInVertex &rhs)
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

		class RZVertexReader
		{
		public:
			
			bool CopyDataToBuffer(FbxVector4 pos,RZInVertex* v); //read data from fbx mesh
			bool GetVerticesFromBuffer(void** pVertices); //write date to RZVertexType
			bool GetIndicesFromBuffer(void** pIndices);
			int GetVerticesCount(){return m_vList.size();};
			int GetIndicesCount(){return m_Indices.size();};
			void AddSubsets(){m_SubsetVertexPos.push_back(m_vList.size());m_SubsetIndexPos.push_back(m_Indices.size());};
			int GetSubsetsCount(){return m_SubsetVertexPos.size();};
			int GetSubsetsVertexPos(int i){return m_SubsetVertexPos[i];};
			int GetSubsetsIndexPos(int i){return m_SubsetIndexPos[i];};
		private:
			map<string,vector<int>> m_RZInVertexMap;
			vector<RZInVertex> m_vList;
			vector<int> m_Indices;
			vector<int> m_SubsetVertexPos;
			vector<int> m_SubsetIndexPos;
		};

		class RZMeshDesc
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

	public:

		RZFbxImporter(){m_pRZIntermediateMesh=NULL;};
		~RZFbxImporter();
		void init();
		void LoadMeshFromFile(const char* lFilename);
		bool ConvertToRZMesh(FbxNode* pNode,int &currentVertexPos, int &currentIndexPos);
		RZIntermediateMesh* GetMesh(){return m_pRZIntermediateMesh;};

	private:
		FbxManager* m_sdkManager;
		FbxScene* m_scene;
		


		bool GetMeshData(FbxMesh* pInMesh, RZVertexReader* pReader, RZMeshDesc* pDesc);
		bool WriteMeshData(INTERMEDIATE_MESH* &pOutMesh,RZVertexReader* pReader, RZMeshDesc* pDesc);
		bool readPos(FbxMesh* pFbxMesh,int ctrlPointIndex,FbxVector4 &outPos);
		bool readUV(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,int layer,FbxVector2 &outUV);
		bool readNM(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,FbxVector4 &outNM);
		bool readTG(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,FbxVector4 &outTG);
		bool readColor(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,FbxColor &outColor);
		

	
	protected:
		RZIntermediateMesh*  m_pRZIntermediateMesh;

	};

};

#endif