/// <summary>
/// InputManager.h
///
/// About:
/// InputManager.h handles all keyboard and mouse 
/// input for the application. The implementation was 
/// guided by RasterTek tutorial 13 and was adapted as 
/// a singleton
///
/// RasterTek tutorial:
/// http://www.rastertek.com/dx11tut13.html
/// </summary>
#ifndef InputManager_h__
#define InputManager_h__

#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

class InputManager
{
public:
	InputManager();
	InputManager(const InputManager&);
	~InputManager();
	static InputManager *Instance()
	{
		if (!m_InputManager)
		{
			m_InputManager = new InputManager;
		}
		return m_InputManager;
	}

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsKeyDown(UCHAR);
	bool IsKeyPressed(UCHAR);

	void GetMouseLocation(float&, float&);
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	unsigned char m_prevKeyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	float m_mouseX, m_mouseY;

	static InputManager *m_InputManager;

	void DeleteInstance();
};

#endif // InputManager_h__
#pragma once
