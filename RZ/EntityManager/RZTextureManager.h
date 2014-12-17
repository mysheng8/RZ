#pragma once
#ifndef _RZTEXTUREMANAGER_H_
#define _RZTEXTUREMANAGER_H_

#include "d3d11.h"
#include "d3dx10math.h"
#include <stdio.h>
#include <string>
#include <map>
#include <d3dx11async.h> 
#include "../Entity/RZTexture.h"

using namespace std;

namespace RZ
{
	class RZTextureManager
	{
	public:
		static RZTextureManager* GetInstance()
		{
			if(!m_instance)
			{
				m_instance= new RZTextureManager();
			}
			return m_instance;
		}
		~RZTextureManager();
		bool Initialize(ID3D11Device* pDevice);
		bool Update(){};
		bool ShotDown();
		RZTexture* GetTexture(string texName){	return m_texMap[texName];};

	private:

		RZTextureManager();
		RZTextureManager(RZTextureManager &other);
		static RZTextureManager*  m_instance;
		map<string,RZTexture*> m_texMap;
	
	};
};



#endif