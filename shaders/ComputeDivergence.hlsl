/// <summary>
/// ComputeDivergence.hlsl
///
/// About:
/// ComputeDivergence - computes the divergence according to the finite difference formula http://http.developer.nvidia.com/GPUGems/elementLinks/tab38_1.jpg
/// Divergence is the first part of the projection step. 
///
/// Based from:
/// cFluid3D.hlsl (BuoyancyComputeShader) - Valentin Hinov - https://github.com/Morji/Fluid-Simulation-DirectX11
/// ComputeDivergence.compute - Scrawk Blog - https://scrawkblog.com/2014/01/09/gpu-gems-to-unity-3d-fluid-simulation/
/// </summary>
#define NUM_THREADS 8

RWTexture3D<float> _DivergenceResult : register (u0);

Texture3D<float3> _Velocity : register (t0);
Texture3D<int> _BoundaryConditions : register (t1);

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeDivergence( uint3 id : SV_DispatchThreadID ) 
{
    //get dimensions of fluid field
	uint3 dimensions;
	_Velocity.GetDimensions(dimensions.x, dimensions.y, dimensions.z);
	
	//from the current cell get neighbouring positions
    uint3 LeftCell = uint3(max(0, id.x - 1), id.y, id.z);
	uint3 RightCell = uint3(min(id.x+1,dimensions.x-1), id.y, id.z);
	
    uint3 TopCell = uint3(id.x, min(id.y + 1, dimensions.y - 1), id.z);
    uint3 BottomCell = uint3(id.x, max(id.y - 1, 0), id.z);

	uint3 UpCell = uint3(id.x, id.y, min(id.z+1,dimensions.z-1));
	uint3 DownCell = uint3(id.x, id.y, max(id.z-1,0));

    //using nighbouring positions get the neighbouring velocity
    float3 RightVelocity = _Velocity[RightCell];
    float3 LeftVelocity = _Velocity[LeftCell];

    float3 TopVelocity = _Velocity[TopCell];
    float3 BottomVelocity = _Velocity[BottomCell];

    float3 UpVelocity = _Velocity[UpCell];
    float3 DownVelocity = _Velocity[DownCell];

	// Enforce boundaries
	float3 obstacleVelocity = float3(0,0,0);

    //Check the boundary conditions to remove fields with collisions 
    if (_BoundaryConditions[RightCell] > 0)
    {
        RightVelocity = obstacleVelocity;
    }
    if (_BoundaryConditions[LeftCell] > 0)
    {
        LeftVelocity = obstacleVelocity;
    }
    if (_BoundaryConditions[TopCell] > 0)
    {
        TopVelocity = obstacleVelocity;
    }
    if (_BoundaryConditions[BottomCell] > 0)
    {
        BottomVelocity = obstacleVelocity;
    }    
    if (_BoundaryConditions[UpCell] > 0)
    {
        UpVelocity = obstacleVelocity;
    }
    if (_BoundaryConditions[DownCell] > 0)
    {
        DownVelocity = obstacleVelocity;
    }

	//computes velocity using 'central differences'
    float result = 0.5f * ((RightVelocity.x - LeftVelocity.x)+ 
                            (TopVelocity.y - BottomVelocity.y)+
                            (UpVelocity.z - DownVelocity.z));

    _DivergenceResult[id] = result;
}