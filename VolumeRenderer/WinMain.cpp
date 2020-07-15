/// <summary>
/// WinMain is the entry point to the program
/// </summary>
#include "System.h"

//First function called from the framework
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	//Create a new system class 
	System* m_system;
	bool result;

	// Create the system object.
	m_system = new System;

	// Initialize and run the system object.
	result = m_system->Initialize();
	if (result)
	{
		m_system->Run();
	}

	// Shutdown and release the system object.
	m_system->Shutdown();
	delete m_system;
	m_system = 0;

	return 0;
}