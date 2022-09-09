#ifndef _FLUID_H_
#define _FLUID_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

class Fluid {
public:
	void Initialize(int, ID3D11Device*, ID3D11DeviceContext*, HWND);
	void Run(float, ID3D11DeviceContext*);
	void Shutdown();
	void Clear(ID3D11DeviceContext*);

private:
	void CompileShaders(ID3D11Device*, HWND);
	void CreateResources(ID3D11Device*);

	void ComputeBoundaryConditions(ID3D11DeviceContext*);
	void ComputeAdvection(ID3D11DeviceContext*, ID3D11UnorderedAccessView*, ID3D11ShaderResourceView*, float, float);
	void ComputeImpulse(ID3D11DeviceContext*, ID3D11UnorderedAccessView*, ID3D11ShaderResourceView*, float);
	void ComputeBuoyancy(ID3D11DeviceContext*);
	void ComputeVorticity(ID3D11DeviceContext*);
	void ComputeConfinement(ID3D11DeviceContext*);
	void ComputeDivergence(ID3D11DeviceContext*);
	void ComputeJacobi(ID3D11DeviceContext*);
	void ComputeProjection(ID3D11DeviceContext*);

	/*Helper function for shader compilation ~ Christian B. Mendl, 2012 - editied by Daniel Weston */
	HRESULT CompileShaderFromFile(const LPCWSTR ShaderFileName, LPCSTR ShaderEntryPoint, LPCSTR ShaderModel, ID3DBlob **ppBlobOut)
	{
		HRESULT hr;

		ID3DBlob *pErrorBlob;
		hr = D3DCompileFromFile(ShaderFileName, NULL, NULL, ShaderEntryPoint, ShaderModel, D3D10_SHADER_ENABLE_STRICTNESS, 0,
			ppBlobOut, &pErrorBlob);
		return hr;
	}

public:
	struct AdvectionBuffer
	{
		float dissipation;
		float dt;
		float decay;
		float forward;
	};
	struct ImpulseBuffer
	{
		DirectX::XMFLOAT3 sourcePos;
		float radius;
		float amount;
		float dt;
		float padding1[3];
		float padding2[3];
	};
	struct BuoyancyBuffer
	{
		float ambientTemperature;
		float buoyancy;
		float dt;
		float weight;
	};
	struct ConfinementBuffer
	{
		float dt;
		float VorticityStrength;
		float padding1;
		float padding2;
	};
	struct SimulationVars
	{
		float m_impulseRadius,
			m_densityAmount,
			m_TemperatureAmount,
			m_timeStep,
			m_densityDissipation,
			m_velocityDissipation,
			m_temperatureDissipation,
			m_decay,
			m_ambientTemperature,
			m_buoyancy,
			m_weight,
			m_VorticityStrength;
	};

	SimulationVars m_GPUFluidVars;

	//shaders
	ID3D11ComputeShader* m_BoundaryConditionsCS;
	ID3D11ComputeShader* m_AdvectionCS;
	ID3D11ComputeShader* m_BuoyancyCS;
	ID3D11ComputeShader* m_ImpulseCS;
	ID3D11ComputeShader* m_VorticityCS;
	ID3D11ComputeShader* m_ConfinementCS;
	ID3D11ComputeShader* m_DivergenceCS;
	ID3D11ComputeShader* m_JacobiCS;
	ID3D11ComputeShader* m_ProjectionCS;
	ID3D11ComputeShader* m_ClearCS;

	//Textures
	ID3D11Texture3D* m_BoundaryConditions;
	ID3D11Texture3D* m_Velocity[2];
	ID3D11Texture3D* m_Density[2];
	ID3D11Texture3D* m_Vorticity;
	ID3D11Texture3D* m_Temperature[2];
	ID3D11Texture3D* m_Divergence;
	ID3D11Texture3D* m_Pressure[2];
	ID3D11Texture3D* m_TempTexture[2];

	//UAVs
	ID3D11UnorderedAccessView* m_BoundaryConditionsUAV;
	ID3D11UnorderedAccessView* m_VelocityUAV[2];
	ID3D11UnorderedAccessView* m_DensityUAV[2];
	ID3D11UnorderedAccessView* m_VorticityUAV;
	ID3D11UnorderedAccessView* m_TemperatureUAV[2];
	ID3D11UnorderedAccessView* m_DivergenceUAV;
	ID3D11UnorderedAccessView* m_PressureUAV[2];
	ID3D11UnorderedAccessView* m_TempUAV[2];

	//SRVs
	ID3D11ShaderResourceView* m_BoundaryConditionsSRV;
	ID3D11ShaderResourceView* m_VelocitySRV[2];
	ID3D11ShaderResourceView* m_DensitySRV[2];
	ID3D11ShaderResourceView* m_VorticitySRV;
	ID3D11ShaderResourceView* m_TemperatureSRV[2];
	ID3D11ShaderResourceView* m_DivergenceSRV;
	ID3D11ShaderResourceView* m_PressureSRV[2];
	ID3D11ShaderResourceView* m_TempSRV[2];

	//buffer
	ID3D11Buffer* m_DensityBuffer;
	ID3D11Buffer* m_AdvectionBuffer;
	ID3D11Buffer* m_BuoyancyBuffer;
	ID3D11Buffer* m_ConfinementBuffer;

	//sampler
	ID3D11SamplerState* m_Sampler;

private:
	int FluidSize;
	float m_timeStep;
};

#endif // !_FLUID_H_

