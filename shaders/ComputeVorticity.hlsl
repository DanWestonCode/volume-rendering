/// <summary>
/// ComputeVorticity.hlsl
///
/// About:
/// Low visc fluids (smoke) usually contain rotational flows. Advection stages of fluid simulation can 
/// smooth out the flow of a fluid making it "un-smoke" like. Rotational flow is called vorticity.
/// This shader is the first step to injecting back in the swirling motion of fluid flow, computing 
/// the vorticity (http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html)
///
/// Based from:
/// cFluid3D.hlsl (VorticityComputeShader) - Valentin Hinov - https://github.com/Morji/Fluid-Simulation-DirectX11
/// ComputeVorticity.compute - Scrawk Blog - https://scrawkblog.com/2014/01/09/gpu-gems-to-unity-3d-fluid-simulation/
/// </summary>
#define NUM_THREADS 8
RWTexture3D<float4> _VorticityResult : register (u0);

Texture3D<float3> _Velocity : register (t0);

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeVorticity( uint3 id : SV_DispatchThreadID )
 {

    //get dimensions of the fluid field
	uint3 dimensions;	
	_Velocity.GetDimensions(dimensions.x, dimensions.y, dimensions.z);

    //from the current cell get neighbouring positions
    uint3 LeftCell = uint3(max(0, id.x - 1), id.y, id.z);
    uint3 RightCell = uint3(min(id.x + 1, dimensions.x - 1), id.y, id.z);

	uint3 BottomCell = uint3(id.x, max(id.y - 1, 0), id.z);
    uint3 TopCell = uint3(id.x, min(id.y + 1, dimensions.y - 1), id.z); 
       
    uint3 DownCell = uint3(id.x, id.y, max(id.z - 1, 0));
    uint3 UpCell = uint3(id.x, id.y, min(id.z + 1, dimensions.z - 1));
    
    //using nighbouring positions get the neighbouring velocity
    float3 RightVelocity = _Velocity[RightCell];
    float3 LeftVelocity = _Velocity[LeftCell];

    float3 TopVelocity = _Velocity[TopCell];
    float3 BottomVelocity = _Velocity[BottomCell];

    float3 UpVelocity = _Velocity[UpCell];
    float3 DownVelocity = _Velocity[DownCell];

	//computes velocity using 'central differences'
    float3 result = 0.5f * float3(((TopVelocity.z - BottomVelocity.z) - (UpVelocity.y - DownVelocity.y)),
								   ((UpVelocity.x - DownVelocity.x) - (RightVelocity.z - LeftVelocity.z)),
								   ((RightVelocity.y - LeftVelocity.y) - (TopVelocity.x - BottomVelocity.x)));

	float lresult = length(result);
    _VorticityResult[id] = float4(result, lresult);

}