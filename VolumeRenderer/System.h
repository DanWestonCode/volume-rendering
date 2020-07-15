/// <summary>
/// System class encapsulates the application.
/// </summary>

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//lean and mean preprocessor speeds up
//building process via reducing size of
//Win32 headers and excluding some
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Graphics.h"
#include "InputManager.h"
#include "Time.h"

class System
{
public:
	struct SMALL_RECT {
		SHORT Left;
		SHORT Top;
		SHORT Right;
		SHORT Bottom;
	};
	System();
	System(const System&);
	~System();

	bool Initialize();
	void Shutdown();
	void Run();


	//handles windows messages sent to app
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Graphics* m_Graphics;	
	Time* m_Time;
};
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* ApplicationHandle = 0;
#endif // !_SYSTEM_H_
