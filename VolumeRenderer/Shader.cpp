#include "Shader.h"
#include <d3dcompiler.h>

Shader::Shader()
{
	m_PixelShader = nullptr;
	m_VertexShader = nullptr;
}

Shader::~Shader() {};

HRESULT Shader::CompileShaderFromFile(const LPCWSTR ShaderFileName, const LPCSTR ShaderEntryPoint, const LPCSTR ShaderModel, ID3DBlob ** const ppBlobOut)
{
	HRESULT hr;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob *pErrorBlob;
	hr = D3DCompileFromFile(ShaderFileName, NULL, NULL, ShaderEntryPoint, ShaderModel, dwShaderFlags, 0,
		ppBlobOut, &pErrorBlob);
	return hr;
}

void Shader::Shutdown()
{
	if (m_VertexShader != nullptr) {
		m_VertexShader->Release();
		m_VertexShader = nullptr;
	}

	if (m_PixelShader != nullptr) {
		m_PixelShader->Release();
		m_PixelShader = nullptr;
	}

	if (m_InputLayout != nullptr) {
		m_InputLayout->Release();
		m_InputLayout = nullptr;
	}
}
