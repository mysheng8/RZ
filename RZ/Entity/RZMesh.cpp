#include "RZmesh.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace RZ;

ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Device* m_pDevice,ID3D11DeviceContext* m_pDeviceContext,ID3D11Buffer* pGBuffer)
{
 D3D11_BUFFER_DESC bufferDesc;
 ZeroMemory(&bufferDesc,sizeof(bufferDesc));
 pGBuffer->GetDesc(&bufferDesc);
 bufferDesc.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
 bufferDesc.Usage=D3D11_USAGE_STAGING;
 bufferDesc.BindFlags=0;
 bufferDesc.MiscFlags=0;
 
 ID3D11Buffer* pDebugBuffer=NULL;
 if( SUCCEEDED(m_pDevice->CreateBuffer(&bufferDesc,NULL,&pDebugBuffer)) )
 {
  m_pDeviceContext->CopyResource(pDebugBuffer,pGBuffer);
 }
 return pDebugBuffer;
}


RZMesh::RZMesh()
{
}


RZMesh::RZMesh(const RZMesh& other)
{
}


RZMesh::~RZMesh()
{
}

bool RZMesh::CreateFromFile(ID3D11Device* device, const char* modelFilename)
{
	ifstream  fin;
    // Open the file
	fin.open(modelFilename,ios::binary);

	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

    // Get the file size
    fin.seekg(0, ios::end);
    int ps = fin.tellg();
	ps+=44;
	//size_t 
	m_pStaticMeshData = new char[ ps ];

	fin.seekg(0, ios::beg);

	//fin>>m_pStaticMeshData;
	fin.read(m_pStaticMeshData,ps);
	fin.close();

	if(!CreateFromMemory(device,m_pStaticMeshData))
		return false;

	return true;
}

bool RZMesh::CreateFromMemory(ID3D11Device* device,char* modelMeshData)
{
	m_pStaticMeshData = modelMeshData;
	m_pMeshHeader = ( RZMESH_HEADER* )m_pStaticMeshData;
    m_pVertexBufferArray = ( RZMESH_VERTEX_BUFFER_HEADER* )( m_pStaticMeshData + m_pMeshHeader->VertexStreamHeadersOffset );
    m_pIndexBufferArray = ( RZMESH_INDEX_BUFFER_HEADER* )( m_pStaticMeshData + m_pMeshHeader->IndexStreamHeadersOffset );
	m_pMeshArray = ( RZMESH_MESH* )( m_pStaticMeshData + m_pMeshHeader->MeshDataOffset );
	m_pMaterialArray = ( RZMESH_MATERIAL* )( m_pStaticMeshData + m_pMeshHeader->MaterialDataOffset );
	m_pSubsetArray = ( RZMESH_SUBSET* )( m_pStaticMeshData + m_pMeshHeader->SubsetDataOffset );

    // Setup subsets
    for( UINT i = 0; i < m_pMeshHeader->NumMeshes; i++ )
    {
        m_pMeshArray[i].pSubsets = ( UINT* )( m_pStaticMeshData + m_pMeshArray[i].SubsetOffset );
    }

	// Setup buffer data pointer
    char* pBufferData = modelMeshData + m_pMeshHeader->HeaderSize + m_pMeshHeader->NonBufferDataSize;

    // Get the start of the buffer data
    UINT BufferDataStart = m_pMeshHeader->HeaderSize + m_pMeshHeader->NonBufferDataSize;

	//Setup Material
	UINT NumMaterials=m_pMeshHeader->NumMaterials;
	m_matList=new RZMaterial*[NumMaterials];

	// Create VBs
    m_ppVertices = new char*[m_pMeshHeader->NumVertexBuffers];
    for( UINT i = 0; i < m_pMeshHeader->NumVertexBuffers; i++ )
    {
        char* pVertices = NULL;
        pVertices = ( char* )( pBufferData + ( m_pVertexBufferArray[i].DataOffset - BufferDataStart ) );

        CreateVertexBuffer( device, &m_pVertexBufferArray[i], pVertices );

        m_pVertices = pVertices;
    }

    // Create IBs
    m_ppIndices = new char*[m_pMeshHeader->NumIndexBuffers];
    for( UINT i = 0; i < m_pMeshHeader->NumIndexBuffers; i++ )
    {
        char* pIndices = NULL;
        pIndices = ( char* )( pBufferData + ( m_pIndexBufferArray[i].DataOffset - BufferDataStart ) );

		CreateIndexBuffer( device, &m_pIndexBufferArray[i], pIndices);

        m_pIndices = pIndices;
    }
	
	return true;


}

void RZMesh::DebugBuffer(ID3D11Device* device,ID3D11DeviceContext* pd3dDeviceContext)
{
	ID3D11Buffer* debugVB=NULL;
	ID3D11Buffer* debugIB=NULL;
	debugVB=CreateAndCopyToDebugBuf(device,pd3dDeviceContext,m_pVertexBufferArray[0].pVB11);
	debugIB=CreateAndCopyToDebugBuf(device,pd3dDeviceContext,m_pIndexBufferArray[0].pIB11);
	D3D11_MAPPED_SUBRESOURCE mappedVBResource;
	D3D11_MAPPED_SUBRESOURCE mappedIBResource;
	RZVertexType* pVB=NULL;
	DWORD* pIB=NULL;
	pd3dDeviceContext->Map(debugVB,0,D3D11_MAP_READ,0,&mappedVBResource);
	pd3dDeviceContext->Map(debugIB,0,D3D11_MAP_READ,0,&mappedIBResource);
	pVB=(RZVertexType*)mappedVBResource.pData;
	pIB=(DWORD*)mappedIBResource.pData;
	//pVB=(RZVertexType*)m_pVertices;
	//pIB=(DWORD*)m_pIndices;
	char logFile[]="E:\\mine\\RZ\\RZ\\log.txt";
	ofstream fout;

	// Open the file
	fout.open(logFile);
	
	for(int i=0;i<m_pVertexBufferArray[0].NumVertices;++i)
	{
		fout<<pVB[i].position.x<<","
			<<pVB[i].position.y<<","
			<<pVB[i].position.z<<","
			<<pVB[i].normal.x<<","
			<<pVB[i].normal.y<<","
			<<pVB[i].normal.z<<","
			<<pVB[i].color.x<<","
			<<pVB[i].color.y<<","
			<<pVB[i].color.z<<","
			<<pVB[i].color.w<<","
			<<pVB[i].uv.x<<","
			<<pVB[i].uv.y<<",";
		fout<<endl;
	};

	for(int i=0;i<m_pIndexBufferArray[0].NumIndices;++i)
	{
		fout<<pIB[i]<<",";
;
		fout<<endl;
	};
	fout.close();
}

bool RZMesh::CreateVertexBuffer(ID3D11Device* pd3dDevice,RZMESH_VERTEX_BUFFER_HEADER* pHeader, void* pVertices)
{
	bool hr;
	D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = ( UINT )( pHeader->SizeBytes );
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = pVertices;
    hr=pd3dDevice->CreateBuffer( &bufferDesc, &InitData, &pHeader->pVB11 );
	return hr;
}

bool RZMesh::CreateIndexBuffer(ID3D11Device* pd3dDevice,RZMESH_INDEX_BUFFER_HEADER* pHeader, void* pIndices)
{
	HRESULT hr;

	pHeader->DataOffset = 0;
    //Index Buffer
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = ( UINT )( pHeader->SizeBytes );
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = pIndices;
    hr = pd3dDevice->CreateBuffer( &bufferDesc, &InitData, &pHeader->pIB11 );
	if(FAILED(hr))  
    {  
        return false;  
    }
	return true;

}

bool RZMesh::SetMaterial(UINT id, RZMaterial* pMat)
{
	m_matList[id]=pMat;
	return true;
}

bool RZMesh::ReleaseModel()
{
	for( UINT64 i = 0; i < m_pMeshHeader->NumVertexBuffers; i++ )
    {
        if( !IsErrorResource( m_pVertexBufferArray[i].pVB11 ) )
            SAFE_RELEASE( m_pVertexBufferArray[i].pVB11 );
    }

    for( UINT64 i = 0; i < m_pMeshHeader->NumIndexBuffers; i++ )
    {
        if( !IsErrorResource( m_pIndexBufferArray[i].pIB11 ) )
            SAFE_RELEASE( m_pIndexBufferArray[i].pIB11 );
    }

	return true;
}



#define MAX_D3D11_VERTEX_STREAMS D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
bool RZMesh::RenderMesh(UINT iMesh,ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	RZMESH_MESH* pMesh = &m_pMeshArray[iMesh];

    UINT Strides[MAX_D3D11_VERTEX_STREAMS];
    UINT Offsets[MAX_D3D11_VERTEX_STREAMS];
    ID3D11Buffer* pVB[MAX_D3D11_VERTEX_STREAMS];


	pVB[0] = m_pVertexBufferArray[ pMesh->VertexBuffer ].pVB11;
    Strides[0] = ( UINT )m_pVertexBufferArray[ pMesh->VertexBuffer ].StrideBytes;
    Offsets[0] = 0;

    RZMESH_INDEX_BUFFER_HEADER* pIndexBufferArray;
    pIndexBufferArray = m_pIndexBufferArray;

    ID3D11Buffer* pIB = pIndexBufferArray[ pMesh->IndexBuffer ].pIB11;
    DXGI_FORMAT ibFormat = DXGI_FORMAT_R16_UINT;
    switch( pIndexBufferArray[ pMesh->IndexBuffer ].IndexType )
    {
    case IT_16BIT:
        ibFormat = DXGI_FORMAT_R16_UINT;
        break;
    case IT_32BIT:
        ibFormat = DXGI_FORMAT_R32_UINT;
        break;
    };


	pd3dDeviceContext->IASetVertexBuffers( 0, 1, pVB, Strides, Offsets );
	pd3dDeviceContext->IASetIndexBuffer( pIB, ibFormat, 0 );

    RZMESH_SUBSET* pSubset = NULL;
    D3D11_PRIMITIVE_TOPOLOGY PrimType;

	XMMATRIX localMatrix=XMLoadFloat4x4(&(m_pMeshArray[iMesh].TransformMatrix));
	XMMATRIX WM=worldMatrix*localMatrix;

    for( int subset = 0; subset < pMesh->NumSubsets; subset++ )
    {
		pSubset = &m_pSubsetArray[ subset];
        PrimType = ( D3D11_PRIMITIVE_TOPOLOGY )pSubset->PrimitiveType;
        pd3dDeviceContext->IASetPrimitiveTopology( PrimType );

        UINT IndexCount = ( UINT )pSubset->IndexCount;
        UINT IndexStart = ( UINT )pSubset->IndexStart;
        UINT VertexStart = ( UINT )pSubset->VertexStart;

		RZMaterial* pMaterial=m_matList[pSubset->MaterialID];

        pMaterial->Render( pd3dDeviceContext, IndexCount, IndexStart, VertexStart, WM, viewMatrix, projectionMatrix );
    }
	return true;
}


bool RZMesh::Render(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	if( !m_pStaticMeshData || !m_pMeshHeader )
        return false;

	for (int i=0;i!=m_pMeshHeader->NumMeshes;++i)
	{
		RenderMesh( i,pd3dDeviceContext,worldMatrix,viewMatrix,projectionMatrix);
	}
	return true;
}





  