/// <summary>
/// ComputeImpulse.hlsl
///
/// About:
/// ComputeImpulse applies density and temperature to a given position
///
/// Based from:
/// cFluid3D.hlsl (ImpulseComputeShader) - Valentin Hinov - https://github.com/Morji/Fluid-Simulation-DirectX11
/// ApplyImpulse.compute - Scrawk Blog - https://scrawkblog.com/2014/01/09/gpu-gems-to-unity-3d-fluid-simulation/
/// </summary>

#define NUM_THREADS 8

cbuffer ImpulseBuffer : register (b0) 
{
    float3 sourcePos;
    float radius;
	float amount;
    float dt;
    float3 padding1;
	float3 padding2;
}

RWTexture3D<float3> impulseResult : register(u0);
Texture3D<float3> impulseInitial : register(t0);

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeImpulse(uint3 id : SV_DispatchThreadID)
{
	//work out currnent pos
    float3 pos = id - sourcePos; //float3(32.0, 12.8, 32.0); //sourcePos for somereason placing fluid in weird place
	float mag = pos.x*pos.x + pos.y*pos.y + pos.z*pos.z;
    float rad2 = radius * radius;
	 
	float3 _amount = exp(-mag/rad2) * amount * dt;
	impulseResult[id] = impulseInitial[id] + _amount;
}

