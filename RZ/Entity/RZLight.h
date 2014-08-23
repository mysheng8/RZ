#ifndef _RZLIGHT_H_
#define _RZLIGHT_H_
#include <d3dx10math.h>
#include "../Framework/InputClass.h"

namespace RZ
{
	class RZLight  
	{  
	public:  
		RZLight(InputClass *input);  
		RZLight(const RZLight&);  
		~RZLight();  

		void UpdateTransform();
		void SetLightDirection(float, float, float);  
		void GetLightDirection(D3DXVECTOR4&);  
  
	private:  
		D3DXVECTOR4 m_lightDir;  
		InputClass *m_pInput;
		float m_rotSpeed;
	};
};

#endif