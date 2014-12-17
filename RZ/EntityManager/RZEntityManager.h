#pragma once
#ifndef _RZENTITYMANAGER_H_
#define _RZENTITYMANAGER_H_

#include "d3d11.h"
#include <stdio.h>
#include <string>
#include <map>
#include "../Entity/RZEntity.h"

#include "RZMaterialManager.h"

namespace RZ
{
	class RZEntityManager
	{
	public:
		static RZEntityManager* GetInstance()
		{
			if(!m_instance)
			{
				m_instance= new RZEntityManager();
			}
			return m_instance;
		}

		virtual ~RZEntityManager();
		bool Initialize(ID3D11Device* pDevice);
		bool ShotDown();
		RZEntity* GetEntity(string entityName){	return m_entityMap[entityName];};


	private:
		static RZEntityManager* m_instance;
		RZEntityManager();
		RZEntityManager(const RZEntityManager& other);
		map<string,RZEntity*> m_entityMap;
	

	};
};
#endif