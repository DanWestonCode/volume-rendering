#include "Model.h"

Model::Model()
{
	m_MatrixBuffer = nullptr;
}

Model::~Model() {};

HRESULT Model::Initialize(ID3D11Device* _device, HWND _hwn)
{
	HRESULT result;
	ID3DBlob* blob = nullptr;

#pragma region Vertex Shader
	result = CompileShaderFromFile(L"../VolumeRenderer/model_position.hlsl", "ModelPositionVS", "vs_5_0", &blob);
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
	result = CompileShaderFromFile(L"../VolumeRenderer/model_position.hlsl", "ModelPositionPS", "ps_5_0", &blob);
	result = _device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_PixelShader);
	blob->Release();
#pragma  endregion	

#pragma region Constant Buffers
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MatrixBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	result = _device->CreateBuffer(&bd, NULL, &m_MatrixBuffer);
#pragma endregion

	return result;
}

void Model::Shutdown()
{
	m_MatrixBuffer->Release();
	m_MatrixBuffer = nullptr;

	Shader::Shutdown();
}
