/// <summary>
/// ComputeBuoyancy.hlsl
///
/// About:
/// ComputeBuoyancy uses the temperature fields of a fluid to create more visually 
/// realistic smoke. This shader ensures that the hot parts of the fluid rise whilst
/// the cold parts fall to the ground. This has a direct effect to the behaviour of the
/// fluid providing the upward force. the buoyancy step needs to be directly computed 
/// fluid velocity fields are advected.
///
/// Based from:
/// cFluid3D.hlsl (BuoyancyComputeShader) - Valentin Hinov - https://github.com/Morji/Fluid-Simulation-DirectX11
/// ApplyBuoyancy.compute - Scrawk Blog - https://scrawkblog.com/2014/01/09/gpu-gems-to-unity-3d-fluid-simulation/
/// </summary>

#define NUM_THREADS 8
cbuffer BuoyancyBuffer : register (b0) 
{
	float ambientTemp;//temperature of the scene
	float buoyancy;//buoyancy of the fluid
    float dt;//current timestep
	float weight;//weight of the fluid
}

RWTexture3D<float3> _buoyancyResult : register(u0);

Texture3D<float3> _Velocity : register(t0);
Texture3D<float> _Density : register(t1);
Texture3D<float> _Temperature : register(t2);

[numthreads(NUM_THREADS,NUM_THREADS,NUM_THREADS)]
void ComputeBuoyancy (int3 id : SV_DispatchThreadID)
{
	//Get cells current temp from fluid field
	float temperatureVal = _Temperature[id];
	//Get cells current density from fluid field
	float densityVal = _Density[id];
	//Get cells current current velocity from fluid field
	float3 result = _Velocity[id];
	
	//if the fluid is hotter than the "ambientTemp"/scene
	if (temperatureVal > ambientTemp)
	{
		//implements http://http.developer.nvidia.com/GPUGems3/elementLinks/0659equ01.jpg
        result += (dt * (temperatureVal - ambientTemp) * buoyancy - densityVal * weight) * float3(0, 1, 0).xyz;
    }
	//set result of the field
	_buoyancyResult[id] = result;
}