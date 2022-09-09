#ifndef _D3D_H_
#define  _D3D_H_

//Specify libraries for linking
#pragma comment(lib, "dxgi.lib") //D3D functionality
#pragma comment(lib, "d3d11.lib") //Tools to interface the hardwares
#pragma comment(lib, "d3dcompiler.lib") //used for compiling shaders

#include <d3d11.h>
#include <DirectXMath.h>

class D3D
{
public:
	D3D();
	~D3D();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	void BeginScene(float*);
	void EndScene();
	ID3D11Device* GetDevice() { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }
	ID3D11DepthStencilView* GetDepthStencilView() { return m_depthStencilView; }

	void EnableAlphaBlending(bool enabled);

	void GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix) const {
		projectionMatrix = DirectX::XMLoadFloat4x4(&m_projectionMatrix);
	}
	void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix) const {
		worldMatrix = DirectX::XMLoadFloat4x4(&m_worldMatrix);
	}
	void GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix) const {
		orthoMatrix = DirectX::XMLoadFloat4x4(&m_orthoMatrix);
	}

public:
	ID3D11RasterizerState* m_backFaceCull;
	ID3D11RasterizerState* m_FrontFaceCull;
	ID3D11BlendState* m_AlphaState;
	ID3D11BlendState* m_NoAlphaState;
	ID3D11RenderTargetView* m_renderTargetView;

	int m_ScreenWidth, m_ScreenHeight;
private:

	DirectX::XMFLOAT4X4 m_projectionMatrix;
	DirectX::XMFLOAT4X4 m_worldMatrix;
	DirectX::XMFLOAT4X4 m_orthoMatrix;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;

	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
};
#endif // !_D3D_H_
#pragma once
