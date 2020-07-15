#include "Graphics.h"

Graphics::Graphics()
{	
	m_clearBackBufferColor = new float[4]{ 0.f, 0.f, 0.f, 1.f };
}

Graphics::~Graphics() {}

HRESULT Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	_hwnd = hwnd;
	HRESULT result = S_OK;

#pragma region Create D3D Object/Device
	// Create the Direct3D object.
	m_D3D = new D3D;
	if (!m_D3D)
	{
		return S_FALSE;
	}
	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return S_FALSE;
	}
#pragma endregion

	m_volumeRenderer = new VolumeRenderer;
	m_volumeRenderer->Initialize(m_D3D->GetDevice(), hwnd, screenWidth, screenHeight);

	return result;
}

void Graphics::Shutdown()
{
	//clean up objects
	if (m_volumeRenderer != nullptr)
	{
		m_volumeRenderer->Shutdown();
		delete m_volumeRenderer;
		m_volumeRenderer = nullptr;
	}
	
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}	
	return;
}

bool Graphics::Frame(float dt)
{
	bool result;

	Update(dt);

	result = Render(dt);

	if (!result)
	{
		return false;
	}

	return true;
}

void Graphics::Update(float dt)
{
	m_volumeRenderer->Update(m_D3D->GetDevice(), dt);
}

bool Graphics::Render(float dt)
{
	//Clear back buffer and depth stencil
	m_D3D->BeginScene(m_clearBackBufferColor);

	// let our volume renderer do it's thing :)
	m_volumeRenderer->Render(m_D3D->GetDeviceContext(), m_D3D->m_backFaceCull, m_D3D->m_FrontFaceCull, m_D3D->m_renderTargetView);

	m_D3D->EndScene();
	return true;
}