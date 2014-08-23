#include "RZEntity.h"


using namespace RZ;
RZEntity::RZEntity()
{
	pMesh=0;
}

RZEntity::RZEntity(const RZEntity& other)
{
}


RZEntity::~RZEntity()
{
}

bool RZEntity::Initialize(ID3D11Device* device,HWND hwnd,char* meshFileName,char* texFileName)
{
	bool result;
	pMesh=new RZMesh();
	result = pMesh->CreateFromFile(device,meshFileName); 
    if(!result)  
    {  
        MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);  
        return false;  
    }
	//pMesh->DebugBuffer(device,m_D3D->GetDeviceContext());
	RZShader* pShader=new RZShader();
	pShader->Initialize(device,hwnd);
	pShaderList.push_back(pShader);
	pMesh->RegisterMaterial(0,pShader);



	RZShader* pShader1=new RZShader();
	pShader1->InitializeShader(device,hwnd,"E:\\mine\\RZ\\RZ\\Resource\\shader\\test.vs", "E:\\mine\\RZ\\RZ\\Resource\\shader\\test.ps");
	pShaderList.push_back(pShader1);
	RZTexture* pTex=new RZTexture();
	pTex->Initialize(device,texFileName);

	pMesh->RegisterMaterial(1,pShader1);
	pMesh->RegisterTexture(1,pTex);



}

void RZEntity::Shutdown()
{
	pMesh->ReleaseModel();
	for(int i=0;i!=pShaderList.size();++i )
	{
		pShaderList[i]->Shutdown();
	}
}

bool RZEntity::Render(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX worldViewProjection, D3DXVECTOR4 lightDir, D3DXMATRIX worldMatrix,D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;
	result=pMesh->Render(pd3dDeviceContext,worldViewProjection,lightDir,worldMatrix,viewMatrix,projectionMatrix);
	return result;
}

