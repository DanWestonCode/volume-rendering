#ifndef _ADVECTION_H_
#define _ADVECTION_H_
#include <d3d11.h>

class Advection
{
	struct AdvectionBuffer
	{
		float dissipation;
		float dt;
		float decay;
		float forward;
	};

	void Initialize(int, ID3D11Device*, ID3D11DeviceContext*, HWND);
};
#endif 