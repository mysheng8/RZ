#pragma once
#ifndef _RZLIGHTMANAGER_H_
#define _RZLIGHTMANAGER_H_



#include "../Entity/RZLight.h"
using namespace std;

namespace RZ
{
	
	class RZLightManager
	{
	public:

		static RZLightManager* GetInstance()
		{
			if(!m_instance)
			{
				m_instance= new RZLightManager();
			}
			return m_instance;
		}

		~RZLightManager();
		bool Initialize(ID3D11Device* pDevice,ID3D11DeviceContext* pd3dDeviceContext);
		void BeginScene();
		void ShotDown();
		bool Render();
		bool AddDirectionLight(bool isStatic, XMFLOAT3 dir,XMFLOAT3 color);
		bool AddPointLight(bool isStatic, XMFLOAT3 pos,XMFLOAT3 color,float range);
		bool AddSpotLight(bool isStatic, XMFLOAT3 pos, XMFLOAT3 dir,XMFLOAT3 color,float range, float angleX,float angleY);


	private:
		RZLightManager();
		RZLightManager(const RZLightManager& other);
		static RZLightManager* m_instance;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		vector<RZLight*> m_staticList;
		vector<RZLight*> m_dynamicList;

		//ID3D11Buffer*			m_staticBuffer; // constant buffer
		//ID3D11Buffer*			m_dynamicBuffer; // constant buffer


	

	};
};

#endif