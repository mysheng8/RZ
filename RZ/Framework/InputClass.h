#ifndef _INPUTCLASS_H_  
#define _INPUTCLASS_H_  

#include <windows.h>


#define SCREENPOSX		100
#define SCREENPOSY		100
#define SCREENWIDTH		1024
#define SCREENHEIGTH	768


enum RZ_MappingKeys
{
	CTRLDOWN=0,
	ALTDOWN,
	SHIFTDOWN,
	SPACEDOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_UP,
	MOVE_DOWN,
	RESET,
	MAX_KEYS,
	UNKNOWN     = 0xFF
};

#define MOUSE_LEFT_BUTTON   0x01
#define MOUSE_MIDDLE_BUTTON 0x02
#define MOUSE_RIGHT_BUTTON  0x04

typedef struct MoveDelta {
    FLOAT x;
    FLOAT y;
} MoveDelta, *pMoveDelta;


class InputClass  
{  
public:  
    InputClass();  
    InputClass(const InputClass&);  
    ~InputClass();  
  
    void Initialize(); 
	void Update();

	bool IsMouseLButtonDown(){return m_bMouseLButtonDown;};
	bool IsMouseMButtonDown(){return m_bMouseMButtonDown;};
	bool IsMouseRButtonDown(){return m_bMouseRButtonDown;};
	
	int GetMouseWheelDelta(){return m_nMouseWheelDelta;};
	MoveDelta GetMouseDelta(){return m_vMouseDelta;};
    bool IsKeyDown(unsigned int);  
	bool IsMapKeyDown(unsigned int);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private: 

	RECT m_rcScreen;

	
    bool m_keys[256];
	bool m_mapKey[MAX_KEYS];
	

	bool m_bMouseLButtonDown;
	bool m_bMouseMButtonDown;
	bool m_bMouseRButtonDown;

    POINT m_ptLastMousePosition;  // Last absolute position of mouse cursor
    int m_nMouseWheelDelta;     // Amount of middle wheel scroll (+/-) 
    MoveDelta m_vMouseDelta;          // Mouse relative delta smoothed over a few frames
    float m_fFramesToSmoothMouseData; // Number of frames to smooth mouse data over

	void UpdateMouseDelta();
    void KeyDown(unsigned int);  
    void KeyUp(unsigned int);

	void MouseClick(HWND,unsigned int);
	void MouseUp(unsigned int);
	void MouseWheel(unsigned int);
	

	RZ_MappingKeys MapKey(unsigned int nKey);
};  

#endif