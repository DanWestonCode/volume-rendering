#ifndef VolumeRaycastShader_h__
#define VolumeRaycastShader_h__

#include "Shader.h"

class RayCastMaterial : public Shader
{
protected:
	struct WindowSizeBuffer
	{
		float windowSize[2];
		float dummy[2];
	};

public:
	RayCastMaterial();
	RayCastMaterial(const RayCastMaterial&);
	~RayCastMaterial();

	virtual HRESULT Initialize(ID3D11Device*, HWND, int, int);
	virtual void Shutdown();

	ID3D11Buffer* m_WindowSizeCB;
};

#endif // VolumeRaycastShader_h__
