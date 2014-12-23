#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()  
{
	m_D3D = 0;
}  
  
GraphicsClass::GraphicsClass(const GraphicsClass& other)  
{  
}  
  
GraphicsClass::~GraphicsClass()  
{  
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd ,InputClass *input)  
{  
	bool result;

	m_D3D = RZD3dRender::GetInstance();
	if(!m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))  
    {  
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
        return false;  
    }

	m_camera = new RZSimpleCamera(input);  
    if(!m_camera)  
    {  
        return false;  
    }


	m_texManager = RZTextureManager::GetInstance();
	if(!m_texManager->Initialize(m_D3D->GetDevice()))
	{
		return false;
	}

	m_shaderManager = RZShaderManager::GetInstance(); 
	if(!m_shaderManager->Initialize(m_D3D->GetDevice(),hwnd))
	{
		return false;
	}

	m_matManager = RZMaterialManager::GetInstance(); 
	if(!m_matManager->Initialize(m_D3D->GetDevice()))
	{
		return false;
	}

	m_entityManager = RZEntityManager::GetInstance(); 
	if(!m_entityManager->Initialize(m_D3D->GetDevice()))
	{
		return false;
	}


	m_lightManager = RZLightManager::GetInstance();  
	if(!m_lightManager->Initialize(m_D3D->GetDevice(),m_D3D->GetDeviceContext()))
	{
		return false;
	}


	BeginScene();

	D3D11_BUFFER_DESC cbbd;
	XMMATRIX cbps[MAXINSTANCENUMBER];
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(XMMATRIX)*MAXINSTANCENUMBER;
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	HRESULT hr = m_D3D->GetDevice()->CreateBuffer(&cbbd, NULL, &cbPerInstanceBuffer);
	
	if(FAILED(hr))  
    {  
        return false;  
    } 
	
	for(map<string,vector<RZPrefab*>>::iterator it=m_prefabMap.begin();it!=m_prefabMap.end();++it)
	{
		for(int i=0;i<it->second.size();++i)
		{
			cbps[i]=XMLoadFloat4x4(&((it->second)[i]->GetTransform()));
		}
	}
	m_D3D->GetDeviceContext()->UpdateSubresource( cbPerInstanceBuffer, 0, NULL, &cbps, 0, 0 );
    return true;  
}

bool GraphicsClass::BeginScene()
{
	XMFLOAT4X4 trans;
	for(int i=0;i<MAXINSTANCENUMBER;++i)
	{
		float posX=(rand() % 2000) / 50.0f;
		float posY=(rand() % 2000) / 500.0f;
		float posZ=(rand() % 2000) / 50.0f;
		XMStoreFloat4x4(&trans,XMMatrixTranslation(posX, posY, posZ ));
		AddPrefab("test",trans);
	}
	return true;
}

void GraphicsClass::AddPrefab(string name, const XMFLOAT4X4 &trans)
{
	RZPrefab *newNode=new  RZPrefab();
	newNode->Initialize(m_entityManager->GetEntity(name),trans);
	m_prefabMap[name].push_back(newNode);
}


void GraphicsClass::Shutdown()  
{  
	if(m_texManager)  
    {  
		m_texManager->ShotDown();  
        m_texManager = 0;  
    }

	if(m_shaderManager)  
    {  
		m_shaderManager->ShotDown();  
        m_shaderManager = 0;  
    } 

	if(m_matManager)  
    {  
		m_matManager->ShotDown();  
        m_matManager = 0;  
    } 

	if(m_entityManager)  
    {  
		m_entityManager->ShotDown();  
        m_entityManager = 0;  
    } 

	if(m_lightManager)  
    {  
		m_lightManager->ShotDown();  
        m_lightManager = 0;  
    } 

  
    if(m_camera)  
    {  
        delete m_camera;  
        m_camera = 0;  
    }

	if(m_D3D)
    {  
        m_D3D->Shutdown();  
        delete m_D3D;  
        m_D3D = 0;  
    }
    return;  
}

bool GraphicsClass::Frame()  
{  
	bool result;  
	m_camera->UpdateTransform();
    // Render the graphics scene.  
    result = Render();  
    if(!result)  
    {  
        return false;  
    } 
    return true;  
}  
  
  
bool GraphicsClass::Render()  
{  
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;

      // Clear the buffers to begin the scene.  
    m_D3D->BeginScene(0.1f, 0.1f, 0.1f, 0.0f);  

	m_camera->Render();  
  
    // Get the world, view, and projection matrices from the camera and d3d objects.  
    m_camera->GetViewMatrix(viewMatrix);  
    m_D3D->GetWorldMatrix(worldMatrix);  
    m_D3D->GetProjectionMatrix(projectionMatrix);  
	
	for(map<string,vector<RZPrefab*>>::iterator it=m_prefabMap.begin();it!=m_prefabMap.end();++it)
	{
		it->second[0]->Render(m_D3D->GetDeviceContext(),worldMatrix, viewMatrix, projectionMatrix);
	
	}

    // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing. 
    //result = m_Entity->Render(m_D3D->GetDeviceContext(),worldMatrix, viewMatrix, projectionMatrix);
	/*
    if(!result)  
    {  
        return false;  
    } */	
    // Present the rendered scene to the screen.  
    m_D3D->EndScene();
    return true;  
}
