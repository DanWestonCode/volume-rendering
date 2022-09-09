/// <summary>
/// ComputeAdvection.hlsl
///
/// About:
/// ComputeAdvection implements the implicit Advection method from Stams(1999) stable solver.
/// http://http.developer.nvidia.com/GPUGems/elementLinks/653equ01.jpg
/// The shader computes velocities backwards in time to their former positions, copying the 
/// quantities at that position to the new texture. This can be used to solve numerous 
/// fields including the velocity and the density. This method is stable for large timesteps
///
/// Based from:
/// cFluid3D.hlsl (AdvectComputeShader) - Valentin Hinov - https://github.com/Morji/Fluid-Simulation-DirectX11
/// ApplyAdvection.compute - Scrawk Blog - https://scrawkblog.com/2014/01/09/gpu-gems-to-unity-3d-fluid-simulation/
/// Example 30-3. Modified Simulation Kernels to Account for Boundary Conditions - Keenan Crane - http://http.developer.nvidia.com/GPUGems3/gpugems3_ch30.html/// 
///</summary>

#define NUM_THREADS 8

//Buffer containing
cbuffer AdvectionBuffer : register (b0) 
{
    float dissipation; //fields resistance to flow
	float dt;//current frames timestep
    float decay;//currently unsure
    float forward;//used to adapt shader for MacCormack scheme (back and forwards advection)
}

//SRVS
Texture3D<float3> _AdvectionTargetRead : register (t0); //previous state
Texture3D<int>  _BoundaryConditions  : register (t1); //collisions in fluid field
Texture3D<float3> _Velocity : register (t2); //fluid fields current velocity

//UAV
RWTexture3D<float3> _AdvectionTargetWrite : register (u0); //new state

// Samplers
SamplerState linearSampler : register (s0);

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeAdvection(uint3 id : SV_DispatchThreadID)
{
    //Check if current cell is within a boundary
	//if(_BoundaryConditions[id] > 0)
	//{
	//	//if so kill advection for current cell
 //       _AdvectionTargetWrite[id] = float3(0,0,0);
 //       return;
 //   }

	//get dimensions of fluid field
	uint3 _Size;
	_Velocity.GetDimensions(_Size.x, _Size.y, _Size.z);

    //trace backwards through the velocity field
	float3 prevPos = id - forward * dt * _Velocity[id];
	//ensure position is at cell centre
    prevPos = (prevPos+0.5f)/_Size;

	//stop negative values entering the fluid field (max returns greater of two values)
    _AdvectionTargetWrite[id] = _AdvectionTargetRead.SampleLevel(linearSampler, prevPos.xyz, 0) * dissipation; //max(0, _AdvectionTargetRead.SampleLevel(linearSampler, prevPos.xyz, 0) * dissipation);
}