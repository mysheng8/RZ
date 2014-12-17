#pragma once
#ifndef _RZSCENE_H_  
#define _RZSCENE_H_

namespace RZ
{
	class RZScene
	{
	public:  
		RZScene();  
		RZScene(const RZScene&);  
		~RZScene(); 
		bool Initilize();
		bool ShotDown();
		void BeginScene();
		void Render();
		void EndScene();
		
	private:
		RZLightManager*		m_lights;
		RZEntityManager*	m_Entitys;
		RZCameraBase*		m_Camera;
	
	}
	
}

#endif