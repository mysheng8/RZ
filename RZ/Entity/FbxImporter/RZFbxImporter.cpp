#include "RZFbxImporter.h"
#include "../RZmesh.h"
#include<sstream>
#include "fbxsdk/scene/fbxaxissystem.h"
using namespace RZ;



bool RZFbxImporter::RZMeshDesc::GetMeshDesc(FbxMesh* pFbxMesh)
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


bool RZFbxImporter::RZVertexReader::CopyDataToBuffer(FbxVector4 pos,RZInVertex *info)
{
	stringstream ss;

	ss<<pos.mData[0]<<pos.mData[1]<<pos.mData[2]<<pos.mData[3];
	string t;
	ss>>t;
	ss.clear();

	vector<int> *RZInVertexList=&m_RZInVertexMap[t];
	int reusedVertex=-1;
	for(vector<int>::iterator it=RZInVertexList->begin();it!=RZInVertexList->end();++it)
	{
		if(m_vList[*it]==*info)
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
		m_vList.push_back(*info);
		m_Indices.push_back(m_vList.size()-1);
		RZInVertexList->push_back(m_vList.size()-1);
	}
	return true;
}

bool RZFbxImporter::RZVertexReader::GetVerticesFromBuffer(void** pVertices)
{
	int num_VertexInfo=m_vList.size();
	*pVertices=new RZVertexType[num_VertexInfo];
	int i=0;

	for(i=0;i!=num_VertexInfo;++i)
	{
		RZVertexType *pRZVert=((RZVertexType*)(*pVertices)+i);
		pRZVert->position.x=(m_vList[i]).pos[0];
		pRZVert->position.y=(m_vList[i]).pos[1];
		pRZVert->position.z=(m_vList[i]).pos[2];

		pRZVert->normal.x=(m_vList[i]).normal[0];
		pRZVert->normal.y=(m_vList[i]).normal[1];
		pRZVert->normal.z=(m_vList[i]).normal[2];

		pRZVert->tangent.x=(m_vList[i]).tangent[0];
		pRZVert->tangent.y=(m_vList[i]).tangent[1];
		pRZVert->tangent.z=(m_vList[i]).tangent[2];

		pRZVert->color.x=(m_vList[i]).color[0];
		pRZVert->color.y=(m_vList[i]).color[1];
		pRZVert->color.z=(m_vList[i]).color[2];
		pRZVert->color.w=(m_vList[i]).color[3];

		pRZVert->uv.x=(m_vList[i]).uv1[0];
		pRZVert->uv.y=(m_vList[i]).uv1[1];
	}
	return true;
}

bool RZFbxImporter::RZVertexReader::GetIndicesFromBuffer(void** pIndices)
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
	m_sdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(m_sdkManager, IOSROOT);
    m_sdkManager->SetIOSettings(ios);

}

RZ::RZFbxImporter::~RZFbxImporter()
{
	m_scene->Destroy();
	m_sdkManager->Destroy();
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

bool RZFbxImporter::readTG(FbxMesh* pFbxMesh,int ctrlPointIndex,int vertexIndex,FbxVector4 &outTG)
{
	//get vertex normal point
	FbxGeometryElementTangent* leTangent = pFbxMesh->GetElementTangent();
	//read normal
	switch(leTangent->GetMappingMode()) 
	{ 
	case FbxGeometryElement::eByControlPoint: 
		{ 
			switch(leTangent->GetReferenceMode()) 
			{ 
			case FbxGeometryElement::eDirect: 
				{ 
					outTG = leTangent->GetDirectArray().GetAt(ctrlPointIndex); 
				} 
				break; 
 
			case FbxGeometryElement::eIndexToDirect: 
				{ 
					int id = leTangent->GetIndexArray().GetAt(ctrlPointIndex); 
					outTG = leTangent->GetDirectArray().GetAt(id); 
				} 
				break; 
 
			default: 
				break; 
			} 
		} 
		break; 
 
	case FbxGeometryElement::eByPolygonVertex: 
		{ 
			switch(leTangent->GetReferenceMode()) 
			{ 
			case FbxGeometryElement::eDirect: 
				{ 
					outTG = leTangent->GetDirectArray().GetAt(vertexIndex); 
				} 
				break; 
 
			case FbxGeometryElement::eIndexToDirect: 
				{ 
					int id = leTangent->GetIndexArray().GetAt(vertexIndex); 
					outTG = leTangent->GetDirectArray().GetAt(id); 
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

bool RZFbxImporter::GetMeshData(FbxMesh* pInMesh, RZVertexReader* pReader, RZMeshDesc* pDesc)
{

	const int triangleCount  = pInMesh->GetPolygonCount();
	const int pointCount=3;
	int i,j;
	int subID=pReader->GetSubsetsCount();

	int vertexIndex=0;
	RZInVertex* vList=new RZInVertex[pointCount];
	for(i=0;i!=triangleCount;++i)
	{
		for(j=0;j!=pointCount;++j)
		{
			int ctrlPointIndex=pInMesh->GetPolygonVertex(i , j); 
			RZInVertex vd;
			//read position
			readPos(pInMesh,ctrlPointIndex,vd.pos);

			//read uv
			if(pDesc->uvsetCount>0)
				readUV(pInMesh,ctrlPointIndex,vertexIndex,0,vd.uv1);
			if(pDesc->uvsetCount>1)
				readUV(pInMesh,ctrlPointIndex,vertexIndex,0,vd.uv2);
			if(pDesc->uvsetCount>2)
				readUV(pInMesh,ctrlPointIndex,vertexIndex,0,vd.uv3);

			//read normal
			if(pDesc->hasNormal)
				readNM(pInMesh,ctrlPointIndex,vertexIndex,vd.normal);

			//read tangent
			if(pDesc->hasTangent)
				readTG(pInMesh,ctrlPointIndex,vertexIndex,vd.tangent);

			//read color
			if(pDesc->hasVectorColor)
				readColor(pInMesh,ctrlPointIndex,vertexIndex,vd.color);

			vList[j]=vd;
			//pReader->CopyDataToBuffer(vd.pos,&vd);
			++vertexIndex;
		}
		
		//convert coordinate system
		for(j=0;j!=pointCount;++j)
		{
			vList[pointCount-1-j].ConvertCoordinateSystem();
			pReader->CopyDataToBuffer(vList[pointCount-1-j].pos,&(vList[pointCount-1-j]));
		}
		
	}
	delete[] vList;

	pReader->AddSubsets();
	return true;
}

bool RZFbxImporter::WriteMeshData(INTERMEDIATE_MESH* &pOutMesh,RZVertexReader* pReader, RZMeshDesc* pDesc)
{
	int numVertices=pReader->GetVerticesCount();
	int numIndex=pReader->GetIndicesCount();
	int dwVertexStride=sizeof(RZVertexType);

	//convert to array
	void* pVertices = NULL;
	void* pIndices = NULL;
	
	pReader->GetVerticesFromBuffer(&pVertices);
	pReader->GetIndicesFromBuffer(&pIndices);

	// Get the Vertex Info into the Mesh
	m_pRZIntermediateMesh->AddVertexBuffer(pOutMesh, 0, numVertices, numVertices*dwVertexStride, dwVertexStride, pVertices );

	// Get the Indices into the Mesh
    m_pRZIntermediateMesh->AddIndexBuffer(pOutMesh, numIndex, numIndex*sizeof(DWORD), IT_32BIT, pIndices );

	// Get the subset
	int currentIndexPos=0;
	int subsetIndexCount=0;

	for(int i=0;i!=pReader->GetSubsetsCount();++i)
	{
		subsetIndexCount=pReader->GetSubsetsIndexPos(i)-currentIndexPos;

		m_pRZIntermediateMesh->AddSubset( pOutMesh,"",i,D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,currentIndexPos,subsetIndexCount,0,numVertices);

		currentIndexPos+=subsetIndexCount;
	}
	return true;
}

bool RZFbxImporter::ConvertToRZMesh(FbxNode* pNode,int &currentVertexPos, int &currentIndexPos)
{
	//Get Mesh
	FbxMesh* pOrgMesh=pNode->GetMesh();

	RZMeshDesc* desc=new RZMeshDesc();
	desc->GetMeshDesc(pOrgMesh);

	if (m_pRZIntermediateMesh==NULL)
		m_pRZIntermediateMesh = new RZIntermediateMesh();

	double* pMatrix = pNode->EvaluateGlobalTransform();

	XMFLOAT4X4 pTransformMatrix = XMFLOAT4X4(	pMatrix[0],pMatrix[1],pMatrix[2],pMatrix[3],
												pMatrix[4],pMatrix[5],pMatrix[6],pMatrix[7],
												pMatrix[8],pMatrix[9],pMatrix[10],pMatrix[11],
												pMatrix[12],pMatrix[13],pMatrix[14],pMatrix[15]);

	INTERMEDIATE_MESH* pMesh = m_pRZIntermediateMesh->AddMesh(pNode->GetName(),pTransformMatrix);

	


	FbxGeometryConverter converter(m_sdkManager);

	//triangluation
	if(!pOrgMesh->IsTriangleMesh())
    {
        pOrgMesh=(FbxMesh*)converter.Triangulate(pOrgMesh,true);
    }

	//generate Tangent
	if(!desc->hasTangent)
    {
        pOrgMesh->GenerateTangentsData(0);
    }
	
	//Get Material List
	for(int i=0;i!=desc->matCount;++i)
	{
		m_pRZIntermediateMesh->AddMaterial(desc->matList[i].c_str());
	}

	RZVertexReader* reader=new RZVertexReader();

	if(desc->matCount==1)
	{
		FbxSurfaceMaterial* mat=pNode->GetMaterial(0);
		INTERMEDIATE_MATERIAL*  pMaterial = m_pRZIntermediateMesh->AddMaterial(mat->GetName());
		GetMeshData(pOrgMesh,reader,desc);
		WriteMeshData(pMesh,reader,desc);
	}
	else if(desc->matCount>1)
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
			GetMeshData(pMeshList[j],reader,desc);
		}
		WriteMeshData(pMesh,reader,desc);
	}
	delete reader;
	delete desc;
	return true;
}


void RZ::RZFbxImporter::LoadMeshFromFile(const char* lFilename)
{
	FbxImporter* lImporter = FbxImporter::Create(m_sdkManager,"");
	if(!lImporter->Initialize(lFilename, -1, m_sdkManager->GetIOSettings())) 
	{ 
        printf("Call to FbxImporter::Initialize() failed.\n"); 
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString()); 
        exit(-1);
	}
	m_scene = FbxScene::Create(m_sdkManager,"myScene");
	
	
	lImporter->Import(m_scene);
	lImporter->Destroy();

	FbxAxisSystem directXAxisSys(FbxAxisSystem::EUpVector::eYAxis,
    FbxAxisSystem::EFrontVector::eParityOdd,
    FbxAxisSystem::eLeftHanded);
	//directXAxisSys.ConvertScene(m_scene);
	FbxNode* lRootNode = m_scene->GetRootNode();

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