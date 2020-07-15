#include "RayCastMaterial.h"

RayCastMaterial::RayCastMaterial()
{
	m_WindowSizeCB = nullptr;
}
RayCastMaterial::RayCastMaterial(const RayCastMaterial& other) {}
RayCastMaterial::~RayCastMaterial() {};

HRESULT RayCastMaterial::Initialize(ID3D11Device* _device, HWND _hwn, int _windowWidth, int _windowHeight)
{
	// standard DX shader compilation jargon below!
	HRESULT result;
	ID3DBlob* blob = nullptr;

#pragma region Vertex Shader
	result = CompileShaderFromFile(L"../VolumeRenderer/raycast.hlsl", "RayCastVS", "vs_5_0", &blob);
	result = _device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_VertexShader);
#pragma endregion	

#pragma region Input Layout
	// Shader Input Layout
	D3D11_INPUT_ELEMENT_DESC m_layout[1];
	m_layout[0].SemanticName = "POSITION"; //match name in VS shader
	m_layout[0].SemanticIndex = 0;
	m_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	m_layout[0].InputSlot = 0;
	m_layout[0].AlignedByteOffset = 0;
	m_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_layout[0].InstanceDataStepRate = 0;

	UINT numElements = ARRAYSIZE(m_layout);

	result = _device->CreateInputLayout(m_layout, numElements, blob->GetBufferPointer(), blob->GetBufferSize(), &m_InputLayout);
	blob->Release();
#pragma endregion	

#pragma region Pixel Shader
	blob = nullptr;
	// Compile and create the pixel shader
	result = CompileShaderFromFile(L"../VolumeRenderer/raycast.hlsl", "RayCastPS", "ps_5_0", &blob);
	result = _device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_PixelShader);
	blob->Release();
#pragma  endregion	

#pragma region Constant Buffers
	WindowSizeBuffer m_windowCB;
	m_windowCB.windowSize[0] = 1.f / _windowWidth;
	m_windowCB.windowSize[1] = 1.f / _windowHeight;

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA BufferInitData;
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&BufferInitData, sizeof(BufferInitData));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WindowSizeBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	BufferInitData.pSysMem = &m_windowCB;
	result = _device->CreateBuffer(&bd, &BufferInitData, &m_WindowSizeCB);
#pragma endregion

	return result;
}

void RayCastMaterial::Shutdown()
{
	m_WindowSizeCB->Release();
	m_WindowSizeCB = nullptr;

	Shader::Shutdown();
}


