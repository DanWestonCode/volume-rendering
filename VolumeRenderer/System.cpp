#include "System.h"
#include <iostream>


System::System()
{
	//init pointers
	m_Graphics = nullptr;
	//m_Time = nullptr;
}
System::System(const System& other)
{
}

bool System::Initialize()
{
	int screenWidth, screenHeight;
	HRESULT result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	//create new game timer
	m_Time = new Time;
	if (!m_Time)
	{
		return false;
	}

	// Initialize the input object.
	InputManager::Instance()->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new Graphics;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void System::Run()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			// Otherwise do the frame processing.
			//Update time
			m_Time->Tick();
			
			if (!result)
			{
				done = true;
			}
		}

	}

	return;

}

//Process the application, end process when Esc pressed
bool System::Frame()
{
	bool result;

	// Do the input frame processing.
	result = InputManager::Instance()->Frame();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (InputManager::Instance()->IsKeyPressed(DIK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(m_Time->DeltaTime());
	if (!result)
	{
		return false;
	}

	return true;
}

//Where windows system messages are directed
LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	//if (TwEventWin(hwnd, umsg, wparam, lparam)) // send event message to AntTweakBar
	//	return 0; // event has been handled by AntTweakBar
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

//Create WIN32 Window holding DX 
void System::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Volume Renderer - Daniel Weston";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,  //extended style 
		m_applicationName, //name of window class
		m_applicationName, //name in title
		WS_OVERLAPPEDWINDOW, //WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, //window style (WS_OVERLAPPEDWINDOW)
		posX, posY, //Top left corners for window
		screenWidth, //width of window
		screenHeight, //height of window
		NULL, //handle to window parent
		NULL, //handle to menu	
		m_hinstance, //instance of current program
		NULL //used for MDI Client window
	);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	//REMEMBER YOU HAVE ADDED PREPROCESSOR TO 
	//IGNORE DEPRECATION OF freopen
	//DEBUG CONSOLE WINDOW
#ifdef _DEBUG
	if (AllocConsole())
	{
		freopen("CONOUT$", "w", stdout);
		//Might be required? Might not work. Who knows.
		freopen("CONOUY$", "w", stderr);
		SetConsoleTitle(L"Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
#endif
}

//where windows messages are sent to
//if everything is okay they are direcred into MessageHandler class
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{


	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}

void System::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

void System::Shutdown()
{
	//// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	// Release the input object.
	InputManager::Instance()->Shutdown();
	if (m_Time)
	{
		delete m_Time;
		m_Time = nullptr;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

System::~System()
{
}

