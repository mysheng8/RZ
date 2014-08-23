#ifndef _GRAPHICSCLASS_H_  
#define _GRAPHICSCLASS_H_ 

#include <windows.h>
#include "D3dClass.h"
#include "InputClass.h"
#include "../Entity/RZcamera.h"
#include "../Entity/RZEntity.h"
#include "../Entity/RZLight.h"
#include "../Entity/ModelClass.h"
#include "../Entity/ColorShaderClass.h"

using namespace RZ;

const bool FULL_SCREEN = false;  
const bool VSYNC_ENABLED = true;  
const float SCREEN_DEPTH = 1000.0f;  
const float SCREEN_NEAR = 0.1f;

class GraphicsClass  
{  
public:  
    GraphicsClass();  
    GraphicsClass(const GraphicsClass&);  
    ~GraphicsClass();  
  
    bool Initialize(int, int, HWND, InputClass*);
    void Shutdown();  
    bool Frame();  
  
private:  
    bool Render();  

private:
	D3DClass* m_D3D;
	RZCameraBase* m_Camera;  
    RZEntity* m_Entity;  
	RZLight* m_Light;
}; 

#endif