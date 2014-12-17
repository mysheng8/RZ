#ifndef _RZLIGHT_H_
#define _RZLIGHT_H_
#include <d3dx10math.h>
//#include "../Framework/InputClass.h"

namespace RZ
{
	struct RZLightParams
	{
		D3DXVECTOR3 lightPos;
		D3DXVECTOR3 lightDir;
		D3DXVECTOR3 lightColor;
		D3DXVECTOR3 lightRange;	//x - point light range; yz - spot light Angles 
	
	};

	class RZLight  
	{  
	public:  
		RZLight(int lightType, bool isStatic);  
		RZLight(const RZLight&);

		~RZLight(); 

		void initilize();

		void SetLightDirection(float, float, float);  
		void GetLightDirection(D3DXVECTOR3&);  
		void SetLightPosition(float, float, float); 
		void GetLightPosition(D3DXVECTOR3&);  
		void SetLightColor(float, float, float);  
		void GetLightColor(D3DXVECTOR3&); 
		void SetPointLightRange(float);  
		void GetPointLightRange(float&); 
		void SetSpotLightAngles(float, float);  
		void GetSpotLightAngles(float&, float&); 
		int GetLightType(){return m_lightType;};
  
	private:  
		int m_lightType;	//1 - point light; 2 - spot light; 3 - direction light;
		bool m_isStatic;
		RZLightParams*			m_params;
		
	};


	/*
	class RZLight  
	{  
	public:  
		RZLight(InputClass *input);  
		RZLight(const RZLight&);  
		~RZLight();  

		//void UpdateTransform();
		void SetLightDirection(float, float, float);  
		void GetLightDirection(D3DXVECTOR4&);  
  
	private:  
		int m_lightType;	//1 - point light; 2 - spot light; 3 - direction light;
		D3DXVECTOR4 m_lightDir;  
		InputClass *m_pInput;
		float m_rotSpeed;
	};*/
};

#endif