#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <windows.h>
#include "D3D.h"

#include "VolumeRenderer.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();	
	~Graphics();

	HRESULT Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float);

	HWND _hwnd;
private:
	void Update(float);
	bool Render(float);

	float* m_clearBackBufferColor;
	D3D* m_D3D;
	   
	VolumeRenderer* m_volumeRenderer;
};

#endif