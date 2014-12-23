#pragma once
#ifndef _RZLIGHT_H_
#define _RZLIGHT_H_

#include "../Framework/RZRenderMRTs.h"



namespace RZ
{
	class RZLight  
	{  
	public:  
		RZLight();  
		RZLight(const RZLight&);

		~RZLight(); 

		void Initialize(int lightType, bool isStatic);

		void SetLightDirection(float, float, float);  
		void GetLightDirection(XMFLOAT3&); 

		void SetLightPosition(float, float, float); 
		void GetLightPosition(XMFLOAT3&); 

		void SetLightColor(float, float, float);  
		void GetLightColor(XMFLOAT3&); 

		void SetLightRange(float);  
		void GetLightRange(float&); 

		void SetSpotLightAngles(float, float);  
		void GetSpotLightAngles(float&, float&); 

		int GetLightType(){return m_lightType;};

		void Render(ID3D11DeviceContext* pDeviceContext);
  
	private:  
		int m_lightType;	//1 - point light; 2 - spot light; 3 - direction light;
		bool m_isStatic;
		RZLightParams*			m_params;
		RZRenderMRTs*			m_rMRTs;		
		
	};


};

#endif