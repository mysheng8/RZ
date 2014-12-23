#include "RZIntermediateMesh.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
using namespace std;
using namespace RZ;

//--------------------------------------------------------------------------------------
// Helper to align offsets to 4k
//--------------------------------------------------------------------------------------
UINT64 Align4k( UINT64 Offset )
{
    UINT64 floor = Offset / 4096;
    return (floor+1)*4096;

}


//--------------------------------------------------------------------------------------
// Add a vertex buffer to the mesh
//--------------------------------------------------------------------------------------
INTERMEDIATE_VERTEX_BUFFER* RZIntermediateMesh::AddVertexBuffer( INTERMEDIATE_MESH* pMesh,
                                                                UINT StreamID, 
                                                                UINT64 NumVertices,
                                                                UINT64 SizeBytes,
                                                                UINT64 StrideBytes,
                                                                void* pBinaryData )
{
    if( StreamID >= MAX_VERTEX_STREAMS )
        return NULL;
    
    // Create a new VB structure
    INTERMEDIATE_VERTEX_BUFFER* pVertexBuffer = new INTERMEDIATE_VERTEX_BUFFER;
    if( !pVertexBuffer )
        return NULL;

    pVertexBuffer->NumVertices = NumVertices;
    pVertexBuffer->SizeBytes = SizeBytes;
    pVertexBuffer->StrideBytes = StrideBytes;
    pVertexBuffer->DataOffset = 0;	//TBD later
    pVertexBuffer->pVertices = new BYTE[ (size_t)SizeBytes ];
    if( !pVertexBuffer->pVertices )
        return NULL;

    // Grab memory
    CopyMemory( pVertexBuffer->pVertices, (BYTE*)pBinaryData, (size_t)SizeBytes );

    // Update the mesh object
    pMesh->VertexBuffer = m_VertexBufferArray.size();

    // Add the VB to the list
	m_VertexBufferArray.push_back( pVertexBuffer );
    return pVertexBuffer;
}

//--------------------------------------------------------------------------------------
// Add an index buffer to the mesh
//--------------------------------------------------------------------------------------
INTERMEDIATE_INDEX_BUFFER* RZIntermediateMesh::AddIndexBuffer( INTERMEDIATE_MESH* pMesh,
                                                              UINT64 NumIndices,
                                                              UINT64 SizeBytes,
                                                              UINT IndexType,
                                                              void* pBinaryData )
{
    // Create a new IB structure
    INTERMEDIATE_INDEX_BUFFER* pIndexBuffer = new INTERMEDIATE_INDEX_BUFFER;
    if( !pIndexBuffer )
        return NULL;

    // Load it out
    pIndexBuffer->NumIndices = NumIndices;
    pIndexBuffer->SizeBytes = SizeBytes;
    pIndexBuffer->IndexType = IndexType;
    pIndexBuffer->DataOffset = 0; //TBD later
    pIndexBuffer->pIndices = new BYTE[ (size_t)SizeBytes ];
    if( !pIndexBuffer->pIndices )
        return NULL;

    // Grab memory
    CopyMemory( pIndexBuffer->pIndices, (BYTE*)pBinaryData, (size_t)SizeBytes );
    
   // Update the mesh object
    pMesh->IndexBuffer = m_IndexBufferArray.size();

    // Add the VB to the list
	m_IndexBufferArray.push_back( pIndexBuffer );
    return pIndexBuffer;
}

//--------------------------------------------------------------------------------------
// Add a mesh
//--------------------------------------------------------------------------------------
INTERMEDIATE_MESH* RZIntermediateMesh::AddMesh( const char* pszName, const XMFLOAT4X4 &TransformMatrix )
{
    INTERMEDIATE_MESH* pMesh = new INTERMEDIATE_MESH;
    if( !pMesh )
        return NULL;

    ZeroMemory( pMesh, sizeof(INTERMEDIATE_MESH) );

    pMesh->Name[0] = 0;
    if( pszName )
        strncpy ( pMesh->Name, pszName, MAX_MESH_NAME );
	pMesh->TransformMatrix=TransformMatrix;
	
	m_MeshArray.push_back( pMesh );
    return pMesh;
}

//--------------------------------------------------------------------------------------
// Add a material
//--------------------------------------------------------------------------------------
INTERMEDIATE_MATERIAL* RZIntermediateMesh::AddMaterial(const char* pszName)
{

	INTERMEDIATE_MATERIAL* pMaterial = GetMaterial(pszName);
	if(pMaterial)
		return pMaterial;
	else
	{
		pMaterial = new INTERMEDIATE_MATERIAL;
		if( !pMaterial )
			return NULL;

		ZeroMemory( pMaterial, sizeof(INTERMEDIATE_MATERIAL) );

		pMaterial->Name[0] = 0;
		if( pszName )
			strncpy ( pMaterial->Name, pszName, MAX_MESH_NAME );
		pMaterial->matID=m_MaterialArray.size();
		m_MaterialArray.push_back( pMaterial );
	
		return pMaterial;
	}
}

//--------------------------------------------------------------------------------------
// Get a material
//--------------------------------------------------------------------------------------
INTERMEDIATE_MATERIAL* RZIntermediateMesh::GetMaterial(const char* pszName)
{
	
	for(int i=0;i!=m_MaterialArray.size();++i)
	{
		int cmp=strcmp(pszName,m_MaterialArray[i]->Name);
		if(cmp==0)
			return m_MaterialArray[i];
	}
    return NULL;
}

//--------------------------------------------------------------------------------------
// Add a subset to a mesh
//--------------------------------------------------------------------------------------
INTERMEDIATE_SUBSET* RZIntermediateMesh::AddSubset( INTERMEDIATE_MESH* pMesh,
                                                   char* pszName,
                                                   UINT MaterialID,
                                                   D3D11_PRIMITIVE_TOPOLOGY PrimitiveType,
                                                   UINT64 IndexStart,
                                                   UINT64 IndexCount,
                                                   UINT64 VertexStart,
                                                   UINT64 VertexCount )
{
    INTERMEDIATE_SUBSET* pSubset = new INTERMEDIATE_SUBSET;
    if( !pSubset )
        return NULL;

    pSubset->Name[0] = 0;
    if( pszName )
        strncpy( pSubset->Name, pszName, MAX_SUBSET_NAME );
    pSubset->MaterialID = MaterialID;
    pSubset->PrimitiveType = PrimitiveType;
    pSubset->IndexStart = IndexStart;
    pSubset->IndexCount = IndexCount;
    pSubset->VertexStart = VertexStart;
    pSubset->VertexCount = VertexCount;
	
	//add into mesh subset list;
    pMesh->SubsetIDArray.push_back( m_SubsetArray.size() );
	pMesh->NumSubsets = pMesh->SubsetIDArray.size();

	//add into material subset list;
	m_MaterialArray[MaterialID]->SubsetIDArray.push_back(m_SubsetArray.size());
	m_MaterialArray[MaterialID]->NumSubsets = m_MaterialArray[MaterialID]->SubsetIDArray.size();

    m_SubsetArray.push_back( pSubset );
    

	
    return pSubset;
}


bool RZIntermediateMesh::SaveAsRZMesh( const char* szFileName )
{
    ofstream fout;

	// Open the file
	fout.open(szFileName,ios::binary);

	// If it could not open the file then exit.
	if(fout.fail())
	{
		return false;
	}
	bool bRetVal = false;

    UINT64 HeaderSize = 0;
    HeaderSize += sizeof( RZMESH_HEADER );
	HeaderSize += m_VertexBufferArray.size()*sizeof(RZMESH_VERTEX_BUFFER_HEADER);
    HeaderSize += m_IndexBufferArray.size()*sizeof(RZMESH_INDEX_BUFFER_HEADER);

    // Calculate the size for everything else
    UINT64 StaticDataSize = 0;
    StaticDataSize += m_MeshArray.size()*sizeof(RZMESH_MESH);
	StaticDataSize += m_MaterialArray.size()*sizeof(RZMESH_MATERIAL);
    StaticDataSize += m_SubsetArray.size()*sizeof(RZMESH_SUBSET);


    UINT64 NonBufferDataSize = StaticDataSize;

    // Calculate all vertex and index data size
    UINT64 BufferDataSize = 0;
    for( UINT i=0; i<m_VertexBufferArray.size(); i++ )
    {
		UINT64 iSize = (m_VertexBufferArray[i])->SizeBytes;
        BufferDataSize += Align4k( iSize );
    }
    for( UINT i=0; i<m_IndexBufferArray.size(); i++ )
    {
        UINT64 iSize = (m_IndexBufferArray[i])->SizeBytes;
        BufferDataSize += Align4k( iSize );
    }

	RZMESH_HEADER fileheader;
    fileheader.Version = RZMESH_FILE_VERSION;
    fileheader.HeaderSize = HeaderSize;
    fileheader.NonBufferDataSize = NonBufferDataSize; 
    fileheader.BufferDataSize = BufferDataSize;
    fileheader.NumVertexBuffers = m_VertexBufferArray.size();
    fileheader.NumIndexBuffers = m_IndexBufferArray.size();
    fileheader.NumMeshes = m_MeshArray.size();
	fileheader.NumMaterials= m_MaterialArray.size();
    fileheader.NumTotalSubsets = m_SubsetArray.size();
    fileheader.VertexStreamHeadersOffset = sizeof( RZMESH_HEADER );
    fileheader.IndexStreamHeadersOffset = fileheader.VertexStreamHeadersOffset + m_VertexBufferArray.size() * sizeof(RZMESH_VERTEX_BUFFER_HEADER);
    fileheader.MeshDataOffset = fileheader.IndexStreamHeadersOffset + m_IndexBufferArray.size() * sizeof(RZMESH_INDEX_BUFFER_HEADER);
	fileheader.MaterialDataOffset = fileheader.MeshDataOffset + m_MeshArray.size() * sizeof(RZMESH_MESH);
    fileheader.SubsetDataOffset = fileheader.MaterialDataOffset + m_MaterialArray.size() * sizeof(RZMESH_MATERIAL);


	/////////////////////////////
    //Write out the header

	bool hr=fout.write((char*)&fileheader, sizeof(RZMESH_HEADER));
	if(!hr)
		return false;


	/////////////////////////////
    //VB headers
    UINT64 BufferDataOffset = HeaderSize + NonBufferDataSize;
    for( UINT i=0; i<m_VertexBufferArray.size(); i++ )
    {
        RZMESH_VERTEX_BUFFER_HEADER* pVBH = m_VertexBufferArray[i];
        pVBH->DataOffset = BufferDataOffset;
		hr=fout.write((char*)pVBH, sizeof(RZMESH_VERTEX_BUFFER_HEADER));
		if(!hr)
			return false;
        BufferDataOffset += Align4k( pVBH->SizeBytes );
    }

	/////////////////////////////
    //IB headers
    for( UINT i=0; i<m_IndexBufferArray.size(); i++ )
    {
        RZMESH_INDEX_BUFFER_HEADER* pIBH = m_IndexBufferArray[i];
        pIBH->DataOffset = BufferDataOffset;
		hr=fout.write((char*)pIBH, sizeof(RZMESH_INDEX_BUFFER_HEADER));
		if(!hr)
			return false;
        BufferDataOffset += Align4k( pIBH->SizeBytes );
    }

    /////////////////////////////
    //Meshes
    UINT64 SubsetListDataOffset = HeaderSize + StaticDataSize;
    for( UINT i=0; i<m_MeshArray.size(); i++ )
    {
        RZMESH_MESH* pMesh = m_MeshArray[i];
        pMesh->SubsetOffset = SubsetListDataOffset;
        SubsetListDataOffset += pMesh->NumSubsets*sizeof(UINT);
		hr=fout.write((char*)pMesh, sizeof(RZMESH_MESH));
		if(!hr)
			return false;
    }

	/////////////////////////////
	//Materials
	for( UINT i=0; i<m_MaterialArray.size(); i++ )
	{
	    RZMESH_MATERIAL* pMaterial = m_MaterialArray[i];
		hr=fout.write((char*)pMaterial, sizeof(RZMESH_MATERIAL));
		if(!hr)
			return false;	
	}

    /////////////////////////////
    //Subsets
    for( UINT i=0; i<m_SubsetArray.size(); i++ )
    {
        INTERMEDIATE_SUBSET* pSubset = m_SubsetArray[i];
		hr=fout.write((char*)pSubset, sizeof(RZMESH_SUBSET));
		if(!hr)
			return false;
    }

	/////////////////////////////
    //VertexData
    for( UINT i=0; i<m_VertexBufferArray.size(); i++ )
    {
        INTERMEDIATE_VERTEX_BUFFER* pVBH = m_VertexBufferArray[i];
		hr=fout.write((char*)pVBH->pVertices, (size_t)pVBH->SizeBytes);
		if(!hr)
			return false;

        //pad
        UINT64 alignsize = Align4k( pVBH->SizeBytes );
        BYTE nothing = 0;
        for( UINT64 b=0; b<alignsize - pVBH->SizeBytes; b++ )
        {
			hr=fout.write((char*)&nothing, sizeof(BYTE));
			if(!hr)
				return false;
        }
    }

    /////////////////////////////
    //IndexData
    for( UINT i=0; i<m_IndexBufferArray.size(); i++ )
    {
        INTERMEDIATE_INDEX_BUFFER* pIBH = m_IndexBufferArray[i];
		//hr=fout.write((char*)pIBH->pIndices, (size_t)pIBH->SizeBytes);
		hr=fout.write((char*)pIBH->pIndices, (size_t)pIBH->SizeBytes);

        //pad
        UINT64 alignsize = Align4k( pIBH->SizeBytes );
        BYTE nothing = 0;
        for( UINT64 b=0; b<alignsize - pIBH->SizeBytes; b++ )
        {
			hr=fout.write((char*)&nothing, sizeof(BYTE));
			if(!hr)
				return false;
        }
    }
	fout.close();
	return true;
}


bool RZIntermediateMesh::SaveAsRZMeshASCII(const char* szFileName )
{
    ofstream fout;

	// Open the file
	fout.open(szFileName);

	// If it could not open the file then exit.
	if(fout.fail())
	{
		return false;
	}
	bool bRetVal = false;

    UINT64 HeaderSize = 0;
    HeaderSize += sizeof( RZMESH_HEADER );
	HeaderSize += m_VertexBufferArray.size()*sizeof(RZMESH_VERTEX_BUFFER_HEADER);
    HeaderSize += m_IndexBufferArray.size()*sizeof(RZMESH_INDEX_BUFFER_HEADER);

    // Calculate the size for everything else
    UINT64 StaticDataSize = 0;
    StaticDataSize += m_MeshArray.size()*sizeof(RZMESH_MESH);
	StaticDataSize += m_MaterialArray.size()*sizeof(RZMESH_MATERIAL);
    StaticDataSize += m_SubsetArray.size()*sizeof(RZMESH_SUBSET);

    UINT64 NonBufferDataSize = StaticDataSize;
    for( UINT i=0; i<m_MeshArray.size(); i++ )
    {
        RZMESH_MESH* pMesh = m_MeshArray[i];
        NonBufferDataSize += pMesh->NumSubsets * sizeof(UINT);
    }

    // Calculate all vertex and index data size
    UINT64 BufferDataSize = 0;
    for( UINT i=0; i<m_VertexBufferArray.size(); i++ )
    {
		UINT64 iSize = (m_VertexBufferArray[i])->SizeBytes;
        BufferDataSize += Align4k( iSize );
    }
    for( UINT i=0; i<m_IndexBufferArray.size(); i++ )
    {
        UINT64 iSize = (m_IndexBufferArray[i])->SizeBytes;
        BufferDataSize += Align4k( iSize );
    }

	RZMESH_HEADER fileheader;
    fileheader.Version = RZMESH_FILE_VERSION;
    fileheader.HeaderSize = HeaderSize;
    fileheader.NonBufferDataSize = NonBufferDataSize; 
    fileheader.BufferDataSize = BufferDataSize;
    fileheader.NumVertexBuffers = m_VertexBufferArray.size();
    fileheader.NumIndexBuffers = m_IndexBufferArray.size();
    fileheader.NumMeshes = m_MeshArray.size();
	fileheader.NumMaterials= m_MaterialArray.size();
    fileheader.NumTotalSubsets = m_SubsetArray.size();
    fileheader.VertexStreamHeadersOffset = sizeof( RZMESH_HEADER );
    fileheader.IndexStreamHeadersOffset = fileheader.VertexStreamHeadersOffset + m_VertexBufferArray.size() * sizeof(RZMESH_VERTEX_BUFFER_HEADER);
    fileheader.MeshDataOffset = fileheader.IndexStreamHeadersOffset + m_IndexBufferArray.size() * sizeof(RZMESH_INDEX_BUFFER_HEADER);
	fileheader.MaterialDataOffset = fileheader.MeshDataOffset + m_MeshArray.size() * sizeof(RZMESH_MESH);
    fileheader.SubsetDataOffset = fileheader.MaterialDataOffset + m_MaterialArray.size() * sizeof(RZMESH_MATERIAL);


	/////////////////////////////
    //Write out the header
	fout<<"RZMESH_HEADER: "
		<< fileheader.Version<<" "
		<<fileheader.HeaderSize<<" "
		<<fileheader.NonBufferDataSize<<" "
		<<fileheader.BufferDataSize<<" "
		<<fileheader.NumVertexBuffers<<" "
		<<fileheader.NumIndexBuffers<<" "
		<<fileheader.NumMeshes<<" "
		<<fileheader.NumMaterials<<" "
		<<fileheader.NumTotalSubsets<<" "
		<<fileheader.VertexStreamHeadersOffset<<" "
		<<fileheader.IndexStreamHeadersOffset<<" "
		<<fileheader.MeshDataOffset<<" "
		<<fileheader.SubsetDataOffset<<endl; 


	/////////////////////////////
    //VB headers
    UINT64 BufferDataOffset = HeaderSize + NonBufferDataSize;
    for( UINT i=0; i<m_VertexBufferArray.size(); i++ )
    {
        RZMESH_VERTEX_BUFFER_HEADER* pVBH = m_VertexBufferArray[i];
        pVBH->DataOffset = BufferDataOffset;

		fout<<"RZMESH_VERTEX_BUFFER_HEADER: "
			<<pVBH->NumVertices<<" "
			<<pVBH->SizeBytes<<" "
			<<pVBH->StrideBytes<<" "
			<<pVBH->DataOffset<<endl;

        BufferDataOffset += Align4k( pVBH->SizeBytes );
    }

	/////////////////////////////
    //IB headers
    for( UINT i=0; i<m_IndexBufferArray.size(); i++ )
    {
        RZMESH_INDEX_BUFFER_HEADER* pIBH = m_IndexBufferArray[i];
        pIBH->DataOffset = BufferDataOffset;

		fout<<"RZMESH_INDEX_BUFFER_HEADER: "
			<<pIBH->NumIndices<<" "
			<<pIBH->SizeBytes<<" "
			<<pIBH->IndexType<<" "
			<<pIBH->DataOffset<<endl;
        BufferDataOffset += Align4k( pIBH->SizeBytes );
    }

    /////////////////////////////
    //Meshes
    UINT64 SubsetListDataOffset = HeaderSize + StaticDataSize;
    for( UINT i=0; i<m_MeshArray.size(); i++ )
    {
        RZMESH_MESH* pMesh = m_MeshArray[i];
        pMesh->SubsetOffset = SubsetListDataOffset;
        SubsetListDataOffset += pMesh->NumSubsets*sizeof(UINT);

		fout<<"RZMESH_MESH: "
			<<pMesh->Name<<" "
			<<pMesh->VertexBuffer<<" "
			<<pMesh->IndexBuffer<<" "
			<<pMesh->NumSubsets<<" ";

		for(int i=0;i!=4;++i)
		{
			for(int j=0;j!=4;++j)
			{
				fout<<pMesh->TransformMatrix.m[i][j]<<" ";
			}
		}

		fout<<pMesh->SubsetOffset<<endl;
    }

	/////////////////////////////
	//Materials
	for( UINT i=0; i<m_MaterialArray.size(); i++ )
	{
		RZMESH_MATERIAL* pMaterial = m_MaterialArray[i];
		fout<<"RZMESH_MATERIAL: "
			<<pMaterial->Name<<" "
			<<pMaterial->NumSubsets<<" ";
		for(int j=0;j!=pMaterial->NumSubsets;++j)
			fout<<pMaterial->Subsets[j]<<" ";
		fout<<endl;
	}

    /////////////////////////////
    //Subsets
    for( UINT i=0; i<m_SubsetArray.size(); i++ )
    {
        INTERMEDIATE_SUBSET* pSubset = m_SubsetArray[i];
		fout<<"RZMESH_SUBSET: "
			<<pSubset->Name<<" "
			<<pSubset->MaterialID<<" "
			<<pSubset->PrimitiveType<<" "
			<<pSubset->IndexStart<<" "
			<<pSubset->IndexCount<<" "
			<<pSubset->VertexStart<<" "
			<<pSubset->VertexCount<<endl;
    }

	/////////////////////////////
    //VertexData
    for( UINT i=0; i<m_VertexBufferArray.size(); i++ )
    {
        INTERMEDIATE_VERTEX_BUFFER* pVBH = m_VertexBufferArray[i];
		fout<<"RZMESH_VERTEX_BUFFER: ";
		for(UINT i=0;i!=pVBH->NumVertices;++i)
		{
			fout<<((RZVertexType*)pVBH->pVertices)[i].position.x<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].position.y<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].position.z<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].normal.x<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].normal.y<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].normal.z<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].color.x<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].color.y<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].color.z<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].color.w<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].uv.x<<" "
				<<((RZVertexType*)pVBH->pVertices)[i].uv.y<<" ";
			
		}
		fout<< endl;
        //pad
        UINT64 alignsize = Align4k( pVBH->SizeBytes );
        BYTE nothing = 0;
		/*
        for( UINT64 b=0; b<alignsize - pVBH->SizeBytes; b++ )
        {
			hr=fout.write((char*)&nothing, sizeof(BYTE));
			if(!hr)
				return false;
        }
		*/
    }

    /////////////////////////////
    //IndexData
    for( UINT i=0; i<m_IndexBufferArray.size(); i++ )
    {
        INTERMEDIATE_INDEX_BUFFER* pIBH = m_IndexBufferArray[i];
		fout<<"RZMESH_INDEX_BUFFER: ";
		for(UINT i=0;i!=pIBH->NumIndices;++i)
		{
			fout<<((DWORD*)pIBH->pIndices)[i]<<" ";
		}
		fout<< endl;

        //pad
        UINT64 alignsize = Align4k( pIBH->SizeBytes );
        BYTE nothing = 0;
		/*
        for( UINT64 b=0; b<alignsize - pIBH->SizeBytes; b++ )
        {
			hr=fout.write((char*)&nothing, sizeof(BYTE));
			if(!hr)
				return false;
        }
		*/
    }
	fout.close();
	return true;
}