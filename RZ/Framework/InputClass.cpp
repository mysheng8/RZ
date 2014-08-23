#include "InputClass.h" 

InputClass::InputClass()  
{  
}

InputClass::InputClass(const InputClass& other)  
{  
}  

InputClass::~InputClass()  
{  
}

void InputClass::Initialize()  
{ 


	//define screen rect
	SetRect( &m_rcScreen, SCREENPOSX, SCREENPOSY, SCREENPOSX+SCREENWIDTH, SCREENPOSY+SCREENHEIGTH );

	
  	m_bMouseLButtonDown=false;
	m_bMouseMButtonDown=false;
	m_bMouseRButtonDown=false;

    m_nMouseWheelDelta=0;     
	m_vMouseDelta.x=0;
	m_vMouseDelta.y=0;     
	m_fFramesToSmoothMouseData = 2.0f;
	
	int i;  
    // Initialize all the keys to being released and not pressed.  
    for(i=0; i<256; i++)  
    {  
        m_keys[i] = false;  
    }  

	for(i=0; i<MAX_KEYS; i++)  
    {  
        m_mapKey[i] = false;  
    } 
  
    return;  
}

void InputClass::Update()
{
	if(m_bMouseLButtonDown||m_bMouseMButtonDown||m_bMouseRButtonDown)
		UpdateMouseDelta();
}

void InputClass::KeyDown(unsigned int input)  
{  
	RZ_MappingKeys aKey=MapKey(input);
	m_mapKey[aKey]=true;
    // If a key is pressed then save that state in the key array.  
    m_keys[input] = true;  
    return;  
}

void InputClass::KeyUp(unsigned int input)  
{  
	RZ_MappingKeys aKey=MapKey(input);
	m_mapKey[aKey]=false;
    // If a key is released then clear that state in the key array.  
    m_keys[input] = false;  
    return;  
}

void InputClass::MouseClick(HWND hWnd, unsigned int nMsg)
{

		GetCursorPos( &m_ptLastMousePosition );

        // Update member var state
        if( ( nMsg == WM_LBUTTONDOWN || nMsg == WM_LBUTTONDBLCLK ) && PtInRect( &m_rcScreen, m_ptLastMousePosition ) )
        {
            m_bMouseLButtonDown = true;
        }
        if( ( nMsg == WM_MBUTTONDOWN || nMsg == WM_MBUTTONDBLCLK ) && PtInRect( &m_rcScreen, m_ptLastMousePosition ) )
        {
            m_bMouseMButtonDown = true;
        }
        if( ( nMsg == WM_RBUTTONDOWN || nMsg == WM_RBUTTONDBLCLK ) && PtInRect( &m_rcScreen, m_ptLastMousePosition ) )
        {
            m_bMouseRButtonDown = true;
        }

        // Capture the mouse, so if the mouse button is 
        // released outside the window, we'll get the WM_LBUTTONUP message
        SetCapture( hWnd );
}


void InputClass::MouseUp(unsigned int nMsg)
{
	if( nMsg == WM_LBUTTONUP )
    {
        m_bMouseLButtonDown = false;
    }
    if( nMsg == WM_MBUTTONUP )
    {
        m_bMouseMButtonDown = false;
    }
    if( nMsg == WM_RBUTTONUP )
    {
        m_bMouseRButtonDown = false;
    }

    // Release the capture if no mouse buttons down
    if( !m_bMouseLButtonDown &&
        !m_bMouseRButtonDown &&
        !m_bMouseMButtonDown )
    {
        ReleaseCapture();
    }
}

void InputClass::MouseWheel(unsigned int input)
{
	m_nMouseWheelDelta += ( short )HIWORD( input );
}



bool InputClass::IsKeyDown(unsigned int key)  
{  
    // Return what state the key is in (pressed/not pressed).  
    return m_keys[key];  
}

bool InputClass::IsMapKeyDown(unsigned int key)  
{  
    // Return what state the key is in (pressed/not pressed).  
    return m_mapKey[key];  
}

void InputClass::UpdateMouseDelta()
{
    POINT ptCurMouseDelta;
    POINT ptCurMousePos;

    // Get current position of mouse
    GetCursorPos( &ptCurMousePos );

    // Calc how far it's moved since last frame
    ptCurMouseDelta.x = ptCurMousePos.x - m_ptLastMousePosition.x;
    ptCurMouseDelta.y = ptCurMousePos.y - m_ptLastMousePosition.y;

	m_ptLastMousePosition=ptCurMousePos;

	// Smooth the relative mouse data over a few frames so it isn't 
    // jerky when moving slowly at low frame rates.
    float fPercentOfNew = 1.0f / m_fFramesToSmoothMouseData;
    float fPercentOfOld = 1.0f - fPercentOfNew;
    m_vMouseDelta.x = m_vMouseDelta.x * fPercentOfOld + ptCurMouseDelta.x * fPercentOfNew;
    m_vMouseDelta.y = m_vMouseDelta.y * fPercentOfOld + ptCurMouseDelta.y * fPercentOfNew;



}

LRESULT CALLBACK InputClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			KeyDown((unsigned int)wparam);
			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			KeyUp((unsigned int)wparam);
			break;
		}
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
		case WM_LBUTTONDBLCLK: 
		{
			MouseClick(hwnd,umsg);
			break;
		}
		case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_LBUTTONUP:
		{
			MouseUp(umsg);
			break;
		}
		case WM_CAPTURECHANGED:
		{
			MouseUp(umsg);
		}
		case WM_MOUSEWHEEL:
		{
			MouseWheel(wparam);
			break;
		}
		default:
		{
			break;
		}

	}
	return 0;
}

RZ_MappingKeys InputClass::MapKey(unsigned int nKey)
{
	switch( nKey )
    {
        case VK_CONTROL:
            return CTRLDOWN;
		case VK_SHIFT:
			return SHIFTDOWN;
		case VK_MENU:
            return ALTDOWN;
		case VK_SPACE:
            return SPACEDOWN;
        case VK_LEFT:
            return MOVE_LEFT;
        case VK_RIGHT:
            return MOVE_RIGHT;
        case VK_UP:
            return MOVE_UP;
        case VK_DOWN:
            return MOVE_DOWN;
        case VK_PRIOR:
            return MOVE_FORWARD;        // pgup
        case VK_NEXT:
            return MOVE_BACKWARD;      // pgdn

        case 'A':
            return MOVE_LEFT;
        case 'D':
            return MOVE_RIGHT;
        case 'W':
            return MOVE_UP;
        case 'S':
            return MOVE_DOWN;
        case 'Q':
            return MOVE_FORWARD;
        case 'E':
            return MOVE_BACKWARD;

        case VK_NUMPAD4:
            return MOVE_LEFT;
        case VK_NUMPAD6:
            return MOVE_RIGHT;
        case VK_NUMPAD8:
            return MOVE_UP;
        case VK_NUMPAD2:
            return MOVE_DOWN;
        case VK_NUMPAD9:
            return MOVE_BACKWARD;
        case VK_NUMPAD7:
            return MOVE_FORWARD;

        case 'Z':
            return RESET;
	}
	return UNKNOWN;
}

