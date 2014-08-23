#include "RZFbxImporter.h"
#include "../RZmesh.h"
#include<sstream>
#include "fbxsdk/scene/fbxaxissystem.h"
using namespace RZ;



bool RZFbxImporter::MeshInfo::GetMeshDesc(FbxMesh* pFbxMesh)
{
	uvsetCount=pFbxMesh->GetUVLayerCount();

	matCount=pFbxMesh->GetNode()->GetMaterialCount();
	for(int i=0;i!=matCount;++i)
	{
		FbxSurfaceMaterial* mat=pFbxMesh->GetNode()->GetMaterial(i);
		matList.push_back(mat->GetName());
	}
	hasNormal=(pFbxMesh->GetElementNormalCount() < 1)?false:true;
	hasTangent=(pFbxMesh->GetElementTangentCount() < 1)?false:true;
	hasVectorColor=(pFbxMesh->GetElementVertexColorCount() < 1)?false:true;
	hasSkinWeight=(pFbxMesh->GetDestinationGeometryWeightedMapCount() < 1)?false:true;
	return true;
}


bool RZFbxImporter::VertexDesc::CopyDataToBuffer(FbxVector4 pos,VertexData *info)
{
	stringstream ss;

	ss<<pos.mData[0]<<pos.mData[1]<<pos.mData[2]<<pos.mData[3];
	string t;
	ss>>t;
	ss.clear();

	vector<int> *vertexDataList=&m_VertexDataMap[t];
	int reusedVertex=-1;
	for(vector<int>::iterator it=vertexDataList->begin();it!=vertexDataList->end();++it)
	{
		if(m_VertexInfos[*it]==*info)
		{
			reusedVertex=*it;
		}
	}
	if(reusedVertex!=-1)
	{
		m_Indices.push_back(reusedVertex);
	}
	else
	{
		m_VertexInfos.push_back(*info);
		m_Indices.push_back(m_VertexInfos.size()-1);
		vertexDataList->push_back(m_VertexInfos.size()-1);
	}
	return true;
}

bool RZFbxImporter::VertexDesc::GetVerticesFromBuffer(void** pVertices)
{
	int num_VertexInfo=m_VertexInfos.size();
	*pVertices=new RZVertexType[num_VertexInfo];
	int i=0;

	for(i=0;i!=num_VertexInfo;++i)
	{
		RZVertexType *pRZVert=((RZVertexType*)(*pVertices)+i);
		pRZVert->position[0]=(m_VertexInfos[i]).pos[0];
		pRZVert->position[1]=(m_VertexInfos[i]).pos[1];
		pRZVert->position[2]=(m_VertexInfos[i]).pos[2];

		pRZVert->normal[0]=(m_VertexInfos[i]).normal[0];
		pRZVert->normal[1]=(m_VertexInfos[i]).normal[1];
		pRZVert->normal[2]=(m_VertexInfos[i]).normal[2];

		pRZVert->color.r=(m_VertexInfos[i]).color[0];
		pRZVert->color.g=(m_VertexInfos[i]).color[1];
		pRZVert->color.b=(m_VertexInfos[i]).color[2];
		pRZVert->color.a=(m_VertexInfos[i]).color[3];

		pRZVert->uv[0]=(m_VertexInfos[i]).uv1[0];
		pRZVert->uv[1]=(m_VertexInfos[i]).uv1[1];
	}
	return true;
}

bool RZFbxImporter::VertexDesc::GetIndicesFromBuffer(void** pIndices)
{
	int num_Indices=m_Indices.size();
	*pIndices=new DWORD[num_Indices];
	int i=0;

	for(i=0;i!=num_Indices;++i)
	{
		*((DWORD*)(*pIndices)+i)=m_Indices[i];
	}
	return true;
}

void RZFbxImporter::init()
{
	lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

}

RZ::RZFbxImporter::~RZFbxImporter()
{
	lScene->Destroy();
	lSdkManager->Destroy();
}

bool RZFbxImporter::readPos(FbxMesh* pFbxMesh,int ctrlPointIndex,FbxVector4 &outPos)
{
	//get point pos
	FbxVector4* pCtrlPoint = pFbxMesh->GetControlPoints(0); 
	outPos=pCtrlPoint[ctrlPointIndex];
	return true;
}

bool RZFbxImporter::readUV(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,int layer,FbxVector2 &outUV)
{
	//get UV point
	outUV=FbxVector2(0,0);
	FbxGeometryElementUV* pVertexUV = pFbxMesh->GetElementUV(layer); 
	switch(pVertexUV->GetMappingMode()) 
	{ 
	case  FbxGeometryElement::eByControlPoint: 
		{ 
			switch(pVertexUV->GetReferenceMode()) 
			{ 
			case FbxGeometryElement::eDirect: 
				{ 
					outUV=pVertexUV->GetDirectArray().GetAt(ctrlPointIndex);
				} 
				break; 
 
			case FbxGeometryElement::eIndexToDirect: 
				{ 
					int id = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex); 
					outUV = pVertexUV->GetDirectArray().GetAt(id); 
				} 
				break; 
 
			default: 
				break; 
			} 
		} 
		break; 
 
	case FbxGeometryElement::eByPolygonVertex: 
		{ 
			switch (pVertexUV->GetReferenceMode()) 
			{ 
			case FbxGeometryElement::eDirect: 
				{
					outUV = pVertexUV->GetDirectArray().GetAt(vertexIndex);
				}
				break;
			case FbxGeometryElement::eIndexToDirect: 
				{ 
					int id = pVertexUV->GetIndexArray().GetAt(vertexIndex); 
					outUV = pVertexUV->GetDirectArray().GetAt(id); 
				} 
				break; 
 
			default: 
				break; 
			} 
		} 
		break; 
	}
	return true;
}

bool RZFbxImporter::readNM(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,FbxVector4 &outNM)
{
	//get vertex normal point
	FbxGeometryElementNormal* leNormal = pFbxMesh->GetElementNormal();
	//read normal
	switch(leNormal->GetMappingMode()) 
	{ 
	case FbxGeometryElement::eByControlPoint: 
		{ 
			switch(leNormal->GetReferenceMode()) 
			{ 
			case FbxGeometryElement::eDirect: 
				{ 
					outNM = leNormal->GetDirectArray().GetAt(ctrlPointIndex); 
				} 
				break; 
 
			case FbxGeometryElement::eIndexToDirect: 
				{ 
					int id = leNormal->GetIndexArray().GetAt(ctrlPointIndex); 
					outNM = leNormal->GetDirectArray().GetAt(id); 
				} 
				break; 
 
			default: 
				break; 
			} 
		} 
		break; 
 
	case FbxGeometryElement::eByPolygonVertex: 
		{ 
			switch(leNormal->GetReferenceMode()) 
			{ 
			case FbxGeometryElement::eDirect: 
				{ 
					outNM = leNormal->GetDirectArray().GetAt(vertexIndex); 
				} 
				break; 
 
			case FbxGeometryElement::eIndexToDirect: 
				{ 
					int id = leNormal->GetIndexArray().GetAt(vertexIndex); 
					outNM = leNormal->GetDirectArray().GetAt(id); 
				} 
				break; 
 
			default: 
				break; 
			} 
		} 
		break; 
	}
	return true;
}

bool RZFbxImporter::readColor(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,FbxColor &outColor)
{
	//get vertex normal point
	FbxGeometryElementVertexColor* vColor = pFbxMesh->GetElementVertexColor();
	//read normal
	switch(vColor->GetMappingMode()) 
	{ 
	case FbxGeometryElement::eByControlPoint: 
		{ 
			switch(vColor->GetReferenceMode()) 
			{ 
			case FbxGeometryElement::eDirect: 
				{ 
					outColor = vColor->GetDirectArray().GetAt(ctrlPointIndex); 
				} 
				break; 
 
			case FbxGeometryElement::eIndexToDirect: 
				{ 
					int id = vColor->GetIndexArray().GetAt(ctrlPointIndex); 
					outColor = vColor->GetDirectArray().GetAt(id); 
				} 
				break; 
 
			default: 
				break; 
			} 
		} 
		break; 
 
	case FbxGeometryElement::eByPolygonVertex: 
		{ 
			switch(vColor->GetReferenceMode()) 
			{ 
			case FbxGeometryElement::eDirect: 
				{ 
					outColor = vColor->GetDirectArray().GetAt(vertexIndex); 
				} 
				break; 
 
			case FbxGeometryElement::eIndexToDirect: 
				{ 
					int id = vColor->GetIndexArray().GetAt(vertexIndex); 
					outColor = vColor->GetDirectArray().GetAt(id); 
				} 
				break; 
 
			default: 
				break; 
			} 
		} 
		break; 
	}
	return true;
}

bool RZFbxImporter::GetMeshData(FbxMesh* pInMesh, VertexDesc* pVertDesc)
{

	const int triangleCount  = pInMesh->GetPolygonCount();
	const int pointCount=3;
	int i,j;
	int subID=pVertDesc->GetSubsetsCount();

	int vertexIndex=0;
	VertexData* vList=new VertexData[pointCount];
	for(i=0;i!=triangleCount;++i)
	{
		for(j=0;j!=pointCount;++j)
		{
			int ctrlPointIndex=pInMesh->GetPolygonVertex(i , j); 
			VertexData vd;
			//read position
			readPos(pInMesh,ctrlPointIndex,vd.pos);

			//read uv
			if(pInfo->uvsetCount>0)
				readUV(pInMesh,ctrlPointIndex,vertexIndex,0,vd.uv1);
			if(pInfo->uvsetCount>1)
				readUV(pInMesh,ctrlPointIndex,vertexIndex,0,vd.uv2);
			if(pInfo->uvsetCount>2)
				readUV(pInMesh,ctrlPointIndex,vertexIndex,0,vd.uv3);

			//read normal
			if(pInfo->hasNormal)
				readNM(pInMesh,ctrlPointIndex,vertexIndex,vd.normal);

			//read color
			if(pInfo->hasVectorColor)
				readColor(pInMesh,ctrlPointIndex,vertexIndex,vd.color);

			vList[j]=vd;
			//pVertDesc->CopyDataToBuffer(vd.pos,&vd);
			++vertexIndex;
		}
		
		//convert coordinate system
		for(j=0;j!=pointCount;++j)
		{
			vList[pointCount-1-j].ConvertCoordinateSystem();
			pVertDesc->CopyDataToBuffer(vList[pointCount-1-j].pos,&(vList[pointCount-1-j]));
		}
		
	}
	delete[] vList;

	pVertDesc->AddSubsets();
	return true;
}

bool RZFbxImporter::WriteMeshData(INTERMEDIATE_MESH* &pOutMesh,VertexDesc* pVertDesc)
{
	int numVertices=pVertDesc->GetVerticesCount();
	int numIndex=pVertDesc->GetIndicesCount();
	int dwVertexStride=sizeof(RZVertexType);

	//convert to array
	void* pVertices = NULL;
	void* pIndices = NULL;
	
	pVertDesc->GetVerticesFromBuffer(&pVertices);
	pVertDesc->GetIndicesFromBuffer(&pIndices);

	// Get the Vertex Info into the Mesh
	m_pRZIntermediateMesh->AddVertexBuffer(pOutMesh, 0, numVertices, numVertices*dwVertexStride, dwVertexStride, pVertices );

	// Get the Indices into the Mesh
    m_pRZIntermediateMesh->AddIndexBuffer(pOutMesh, numIndex, numIndex*sizeof(DWORD), IT_32BIT, pIndices );

	// Get the subset
	int currentIndexPos=0;
	int subsetIndexCount=0;

	for(int i=0;i!=pVertDesc->GetSubsetsCount();++i)
	{
		subsetIndexCount=pVertDesc->GetSubsetsIndexPos(i)-currentIndexPos;

		m_pRZIntermediateMesh->AddSubset( pOutMesh,"",i,D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,currentIndexPos,subsetIndexCount,0,numVertices);

		currentIndexPos+=subsetIndexCount;
	}
	return true;
}

bool RZFbxImporter::ConvertToRZMesh(FbxNode* pNode,int &currentVertexPos, int &currentIndexPos)
{
	//Get Mesh
	FbxMesh* pOrgMesh=pNode->GetMesh();

	pInfo=new MeshInfo();
	pInfo->GetMeshDesc(pOrgMesh);

	if (m_pRZIntermediateMesh==NULL)
		m_pRZIntermediateMesh = new RZIntermediateMesh();

	double* pMatrix = pNode->EvaluateGlobalTransform();

	D3DXMATRIX pTransformMatrix = D3DXMATRIX(	pMatrix[0],pMatrix[1],pMatrix[2],pMatrix[3],
												pMatrix[4],pMatrix[5],pMatrix[6],pMatrix[7],
												pMatrix[8],pMatrix[9],pMatrix[10],pMatrix[11],
												pMatrix[12],pMatrix[13],pMatrix[14],pMatrix[15]);

	INTERMEDIATE_MESH* pMesh = m_pRZIntermediateMesh->AddMesh(pNode->GetName(),pTransformMatrix);

	


	FbxGeometryConverter converter(lSdkManager);

	//triangluation
	if(!pOrgMesh->IsTriangleMesh())
    {
        pOrgMesh=(FbxMesh*)converter.Triangulate(pOrgMesh,true);
    }

	


	//Get Material List
	for(int i=0;i!=pInfo->matCount;++i)
	{
		m_pRZIntermediateMesh->AddMaterial(pInfo->matList[i].c_str());
	}

	VertexDesc* pVertDesc=new VertexDesc();

	if(pInfo->matCount==1)
	{
		FbxSurfaceMaterial* mat=pNode->GetMaterial(0);
		INTERMEDIATE_MATERIAL*  pMaterial = m_pRZIntermediateMesh->AddMaterial(mat->GetName());
		GetMeshData(pOrgMesh,pVertDesc);
		WriteMeshData(pMesh,pVertDesc);
	}
	else if(pInfo->matCount>1)
	{
		//split mesh per material
		bool result=converter.SplitMeshPerMaterial(pOrgMesh,true);
		if(!result)
			return false;
		int attrNum=pNode->GetNodeAttributeCount();
		vector<FbxMesh*> pMeshList;
		int i,j;
		for (i=0;i!=attrNum;++i)
		{
			FbxNodeAttribute *FbxNodeAttr=pNode->GetNodeAttributeByIndex(i);
			if(FbxNodeAttr->GetAttributeType()==FbxNodeAttribute::eMesh)
			{
				pMeshList.push_back((FbxMesh*)FbxNodeAttr);
			}
		}

		for(j=0;j!=pMeshList.size();++j)
		{
			FbxLayerElementMaterial* layerElement=pMeshList[j]->GetElementMaterial(0);
			int index = layerElement->GetIndexArray()[0];
			FbxSurfaceMaterial* mat = pNode->GetMaterial(index);
			GetMeshData(pMeshList[j],pVertDesc);
		}
		WriteMeshData(pMesh,pVertDesc);
	}
	delete pVertDesc;
	return true;
}


void RZ::RZFbxImporter::LoadMeshFromFile(const char* lFilename)
{
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
	if(!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) 
	{ 
        printf("Call to FbxImporter::Initialize() failed.\n"); 
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString()); 
        exit(-1);
	}
	lScene = FbxScene::Create(lSdkManager,"myScene");
	
	
	lImporter->Import(lScene);
	lImporter->Destroy();

	FbxAxisSystem directXAxisSys(FbxAxisSystem::EUpVector::eYAxis,
    FbxAxisSystem::EFrontVector::eParityOdd,
    FbxAxisSystem::eLeftHanded);
	//directXAxisSys.ConvertScene(lScene);
	FbxNode* lRootNode = lScene->GetRootNode();

	int i;
	int	vertexPos=0;
	int indexPos=0;
	for( i = 0; i < lRootNode->GetChildCount(); i++)
    {
        FbxNode* pNode=lRootNode->GetChild(i);
		if(pNode->GetNodeAttribute()->GetAttributeType()==FbxNodeAttribute::eMesh)
		{
			ConvertToRZMesh(pNode,vertexPos, indexPos);
		}
    }
}