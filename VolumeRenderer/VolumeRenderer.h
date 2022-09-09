#ifndef VOLUMERENDERER_H_
#define VOLUMERENDERER_H_

#include <d3d11.h>
#include "Model.h"
#include "RayCastMaterial.h"

class VolumeRenderer
{
public:
	void Initialize(ID3D11Device* const device, const HWND hwnd, const int width, const int height);
	void Update(ID3D11Device* const device, const float dt);
	void Render(ID3D11DeviceContext* const deviceContext, ID3D11RasterizerState* const back, ID3D11RasterizerState* const front, ID3D11RenderTargetView* const rtView);
	void Shutdown();
	   
private:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX mWVP;
	};

	void CreateRenderTexture(ID3D11Device* const device, const int width, const int height);
	void CreateSampler(ID3D11Device* const device);
	void CreateCube(ID3D11Device* const device);
	void LoadVolume(ID3D11Device * const device, const LPCWSTR file);

	// rotation - y-axis (super lazy but I only want to rotate it on this :P)
	float rot = 1;
	DirectX::XMFLOAT4 m_Rot;

	// "materials"
	Model* m_modelShader;
	RayCastMaterial* m_volumeRaycastShader;

	//render textures
	ID3D11Texture2D* m_modelTex2DFront;
	ID3D11ShaderResourceView* m_modelSRVFront;
	ID3D11RenderTargetView*	m_modelRTVFront;
	ID3D11Texture2D* m_modelText2DBack;
	ID3D11ShaderResourceView* m_modelRSVBack;
	ID3D11RenderTargetView*	m_ModelRTVBack;
	//sampler 
	ID3D11SamplerState* m_samplerLinear;
	//volume texture
	ID3D11Texture3D* m_volumeTex3D;
	ID3D11ShaderResourceView* m_volRSV;
	//vertex and index buffers
	ID3D11Buffer* m_cubeVB;
	ID3D11Buffer* m_cubeIB;

	// project matrix for render
	DirectX::XMFLOAT4X4 viewProj;
};

#endif