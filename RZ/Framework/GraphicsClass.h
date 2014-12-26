#pragma once
#ifndef _GRAPHICSCLASS_H_  
#define _GRAPHICSCLASS_H_ 

#include <windows.h>
#include "RZCommon.h"
#include "RZD3dRender.h"
#include "InputClass.h"
#include "../Entity/RZcamera.h"
#include "../Entity/RZPrefab.h"
#include "../EntityManager/RZEntityManager.h"
#include "../EntityManager/RZTextureManager.h"
#include "../EntityManager/RZShaderManager.h"
#include "../EntityManager/RZMaterialManager.h"
#include "../EntityManager/RZLightManager.h"


using namespace RZ;


const bool VSYNC_ENABLED = true;  
const float SCREEN_DEPTH = 1000.0f;  
const float SCREEN_NEAR = 0.1f;
const int MAXINSTANCENUMBER = 64;
class GraphicsClass  
{  
public:  
    GraphicsClass();  
    GraphicsClass(const GraphicsClass&);  
    ~GraphicsClass();  
  
    bool Initialize(int, int, HWND, InputClass*);

	bool BeginScene();
    void Shutdown();  
    bool Frame();  
  
private:  
    bool Render();  

private:
	RZD3dRender* m_D3D;
	RZCameraBase* m_camera;  
    RZEntityManager* m_entityManager;  
	RZTextureManager* m_texManager;
	RZShaderManager* m_shaderManager;
	RZMaterialManager* m_matManager;
	RZLightManager* m_lightManager;
	map<string,vector<RZPrefab*> > m_prefabMap;


	void AddPrefab(string name, const XMFLOAT4X4  &trans);
	ID3D11Buffer* cbPerInstanceBuffer;

}; 

#endif