#pragma once
#ifndef _RZSHADERMANAGER_H_
#define _RZSHADERMANAGER_H_



#include "../Framework/RZCommon.h"
#include "../Entity/RZShader.h"

using namespace std;

namespace RZ
{

	class RZShaderManager
	{
	public:

		static RZShaderManager* GetInstance()
		{
			if(!m_instance)
			{
				m_instance= new RZShaderManager();
			}
			return m_instance;
		}

		~RZShaderManager();
		bool Initialize(ID3D11Device* pDevice,HWND hwnd);
		bool ShotDown();
		RZShader* GetShader(string shaderName){	return m_shaderMap[shaderName];};

	private:
		RZShaderManager();
		RZShaderManager(RZShaderManager &other);
		static RZShaderManager*  m_instance;
		map<string,RZShader*> m_shaderMap;
		ID3D11Buffer* m_matrixBuffer;
	};
};



#endif