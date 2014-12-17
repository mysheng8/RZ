#pragma once
#ifndef _RZMATERIALMANAGER_H_
#define _RZMATERIALMANAGER_H_

#include "d3d11.h"
#include "d3dx10math.h"
#include <stdio.h>
#include <string>
#include <map>
#include <d3dx11async.h> 
#include "../Entity/RZMaterial.h"
#include "RZShaderManager.h"
#include "RZTextureManager.h"

using namespace std;

namespace RZ
{

	class RZMaterialManager
	{
	public:

		static RZMaterialManager* GetInstance()
		{
			if(!m_instance)
			{
				m_instance= new RZMaterialManager();
			}
			return m_instance;
		}

		~RZMaterialManager();
		bool Initialize(ID3D11Device* pDevice);
		bool ShotDown();
		RZMaterial* GetMaterial(string matName){	return m_matMap[matName];};

	private:
		RZMaterialManager();
		RZMaterialManager(RZMaterialManager &other);
		
		static RZMaterialManager* m_instance;

		map<string,RZMaterial*> m_matMap;
	};
};



#endif