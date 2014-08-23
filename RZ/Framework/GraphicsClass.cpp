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

	m_D3D = new D3DClass;
	if(!m_D3D)  
    {  
        return false;  
    }
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)  
    {  
        MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);  
        return false;  
    }

	m_Camera = new RZSimpleCamera(input);  
    if(!m_Camera)  
    {  
        return false;  
    }

	m_Entity = new RZEntity;  
    if(!m_Entity)  
    {  
        return false;  
    }
	
	result = m_Entity->Initialize(m_D3D->GetDevice(),hwnd,"E:\\mine\\RZ\\RZ\\Resource\\model\\test.rz","E:\\mine\\RZ\\RZ\\Resource\\tex\\test.png");  
    
  

	m_Light = new RZLight(input);  
    if(!m_Light)  
    {  
        return false;  
    }  


    return true;  
}

void GraphicsClass::Shutdown()  
{  
	if(m_Light)  
    {  
        delete m_Light;  
        m_Light = 0;  
    } 
  
    // Release the model object.  
    if(m_Entity)  
    {  
        m_Entity->Shutdown();  
        delete m_Entity;  
        m_Entity = 0;  
    }  
  
    // Release the camera object.  
    if(m_Camera)  
    {  
        delete m_Camera;  
        m_Camera = 0;  
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
	m_Light->UpdateTransform();
	m_Camera->UpdateTransform();
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
	D3DXMATRIX worldViewProjection, viewMatrix, projectionMatrix, worldMatrix;
	D3DXVECTOR4 lightDir;
	bool result;
      // Clear the buffers to begin the scene.  
    m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 0.0f);  

	m_Camera->Render();  
  
    // Get the world, view, and projection matrices from the camera and d3d objects.  
    m_Camera->GetViewMatrix(viewMatrix);  
    m_D3D->GetWorldMatrix(worldMatrix);  
    m_D3D->GetProjectionMatrix(projectionMatrix);  
	worldViewProjection=worldMatrix * viewMatrix * projectionMatrix;
	m_Light->GetLightDirection(lightDir);
	
    // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing. 
    result = m_Entity->Render(m_D3D->GetDeviceContext(),worldViewProjection, lightDir, worldMatrix, viewMatrix, projectionMatrix);  
    if(!result)  
    {  
        return false;  
    } 	
    // Present the rendered scene to the screen.  
    m_D3D->EndScene();
    return true;  
}
