/// <summary>
/// ComputeConfinement.hlsl
///
/// About:
/// Low visc fluids (smoke) usually contain rotational flows. Advection stages of fluid simulation can 
/// smooth out the flow of a fluid making it "un-smoke" like. Rotational flow is called vorticity.
/// This shader is the second step to injecting back in the swirling motion of fluid flow, computing 
/// the normalized vorticity vector field(http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html)
///
/// Based from:
/// cFluid3D.hlsl (ConfinementComputeShader) - Valentin Hinov - https://github.com/Morji/Fluid-Simulation-DirectX11
/// ComputeConfinement.compute - Scrawk Blog - https://scrawkblog.com/2014/01/09/gpu-gems-to-unity-3d-fluid-simulation/
/// </summary>
#define NUM_THREADS 8

cbuffer ConfinementBuffer : register (b0)
{
	float dt;
	float VorticityStrength;
    float2 padding;
}

Texture3D<int> _BoundaryConditions : register (t0);
Texture3D<float4> _Vorticity : register (t1);
Texture3D<float3> _Velocity : register (t2);

RWTexture3D<float3> _VelocityResult : register (u0);

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeConfinement(uint3 id : SV_DispatchThreadID)
{
    //Check if current cell is within a boundary
	if (_BoundaryConditions[id] > 0)
	{
		return;
	}

    //get dimensions of the fluid filed
	uint3 dimensions;
	_Velocity.GetDimensions(dimensions.x, dimensions.y, dimensions.z);

    //from the current cell get neighbouring positions
    uint3 LeftCell = uint3(max(0, id.x - 1), id.y, id.z);
    uint3 RightCell = uint3(min(id.x + 1, dimensions.x - 1), id.y, id.z);
	
    uint3 TopCell = uint3(id.x, min(id.y + 1, dimensions.y - 1), id.z);
    uint3 BottomCell = uint3(id.x, max(id.y - 1, 0), id.z);

    uint3 UpCell = uint3(id.x, id.y, min(id.z + 1, dimensions.z - 1));
    uint3 DownCell = uint3(id.x, id.y, max(id.z - 1, 0));

    //Get length of the surrounding vorticities
    float omegaT = length(_Vorticity[TopCell]);
    float omegaB = length(_Vorticity[BottomCell]);

    float omegaR = length(_Vorticity[RightCell]);
    float omegaL = length(_Vorticity[LeftCell]);

    float omegaU = length(_Vorticity[UpCell]);
    float omegaD = length(_Vorticity[DownCell]);

    //Get the current Cell vorticity value
	float3 omega = _Vorticity[id].xyz;

	float3 eta = 0.5f * float3(omegaR - omegaL, omegaT - omegaB, omegaU - omegaD);
	eta = normalize(eta + float3(0.001f, 0.001f, 0.001f));

    float3 force = dt * VorticityStrength * float3((eta.y * omega.z - eta.z * omega.y), (eta.z * omega.x - eta.x * omega.z), (eta.x * omega.y - eta.y * omega.x));
	
	_VelocityResult[id] = _Velocity[id] + force;
}