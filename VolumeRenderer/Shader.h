#ifndef SHADER_h__
#define SHADER_h__

#include <d3d11.h>

class Shader
{
public:
	Shader();
	~Shader();

	virtual HRESULT Initialize(ID3D11Device*, HWND) { return S_OK; };
	virtual void Shutdown();

	HRESULT CompileShaderFromFile(const LPCWSTR, const LPCSTR, const LPCSTR, ID3DBlob ** const);

	//Getters for VS, PS and InputLayout
	ID3D11VertexShader* GetVertexShader() { return m_VertexShader; }
	ID3D11PixelShader* GetPixelShader() { return m_PixelShader; }
	ID3D11InputLayout* GetInputLayout() { return m_InputLayout; }

protected:

	ID3D11VertexShader* m_VertexShader;
	ID3D11InputLayout* m_InputLayout;
	ID3D11PixelShader* m_PixelShader;
};
#endif // Shader_h__
