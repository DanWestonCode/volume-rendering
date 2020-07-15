#ifndef MODEL_h__
#define MODEL_h__

#include "Shader.h"
#include <DirectXMath.h>

class Model : public Shader
{
protected:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX mWVP;
	};

public:
	Model();
	~Model();

	virtual HRESULT Initialize(ID3D11Device*, HWND);
	virtual void Shutdown();

	ID3D11Buffer* m_MatrixBuffer;
};
#endif // MODEL_h__
