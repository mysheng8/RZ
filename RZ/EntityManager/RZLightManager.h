#pragma once
#ifndef _RZLIGHTMANAGER_H_
#define _RZLIGHTMANAGER_H_

#include "d3d11.h"
#include "d3dx10math.h"
#include <stdio.h>
#include <vector>
#include <d3dx11async.h> 
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

		virtual ~RZLightManager();
		bool Initialize(ID3D11Device* pDevice,ID3D11DeviceContext* pd3dDeviceContext);
		void BeginScene();
		void ShotDown();
		bool Render();
		bool AddDirectionLight(bool isStatic, D3DXVECTOR3 dir,D3DXVECTOR3 color);
		bool AddPointLight(bool isStatic, D3DXVECTOR3 pos,D3DXVECTOR3 color,float range);
		bool AddSpotLight(bool isStatic, D3DXVECTOR3 pos, D3DXVECTOR3 dir,D3DXVECTOR3 color,float angleX,float angleY);


	private:
		RZLightManager();
		RZLightManager(const RZLightManager& other);
		static RZLightManager* m_instance;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		vector<RZLight*> m_staticList;
		vector<RZLight*> m_dynamicList;

		ID3D11Buffer*			m_staticBuffer; // constant buffer
		ID3D11Buffer*			m_dynamicBuffer; // constant buffer

		//shaders
		ID3D11VertexShader*		m_pointLightVS;
		ID3D11PixelShader*		m_pointLightPS;

		ID3D11VertexShader*		m_spotLightVS;
		ID3D11PixelShader*		m_spotLightPS;

		ID3D11VertexShader*		m_directionLightVS;
		ID3D11PixelShader*		m_directionLightPS;
	

	};
};

#endif