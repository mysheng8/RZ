#pragma once
#ifndef _RZCamera_H_
#define _RZCamera_H_

#include "../Framework/RZCommon.h"
#include "../Framework/InputClass.h"

namespace RZ
{


	class RZCameraBase  
	{  
	public:  
		RZCameraBase();  
		RZCameraBase(const RZCameraBase&);  
		virtual ~RZCameraBase();  
  
		virtual void SetPosition(float, float, float);  
		virtual void SetRotation(float, float, float);  
  
		virtual XMFLOAT3 GetPosition();  
		virtual XMFLOAT3 GetRotation();  

		virtual void UpdateTransform()=0;
		virtual void Render()=0;  
		virtual void GetViewMatrix(XMMATRIX& outMatrix){outMatrix = XMLoadFloat4x4(&m_viewMatrix);};  

 	protected:  
		float m_positionX, m_positionY, m_positionZ;  
		float m_rotationX, m_rotationY, m_rotationZ;  
		XMFLOAT4X4 m_viewMatrix; 
	};

	class RZSimpleCamera: public RZCameraBase
	{  
	public:  
		RZSimpleCamera(InputClass *input);  
		RZSimpleCamera(const RZSimpleCamera&){};  
		virtual ~RZSimpleCamera(){};
  

		virtual void UpdateTransform();

		virtual void Render();  
		virtual void GetViewMatrix(XMMATRIX&);  
  
	private:
		InputClass* m_pInput;
		float m_camMoveSpeed;
		float m_camRotSpeed;
		float m_focusDistance;

	};


};
#endif