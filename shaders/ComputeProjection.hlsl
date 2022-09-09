/// <summary>
/// ComputeProjection.hlsl
///
/// About:
/// ComputeProjection moves the fluid quantities around the fields
///
/// Based from:
/// cFluid3D.hlsl (SubtractGradientComputeShader) - Valentin Hinov - https://github.com/Morji/Fluid-Simulation-DirectX11
/// ComputeProjection.compute - Scrawk Blog - https://scrawkblog.com/2014/01/09/gpu-gems-to-unity-3d-fluid-simulation/
/// </summary>
#define NUM_THREADS 8

Texture3D<float> _Pressure : register (t0);
Texture3D<float3> _Velocity : register (t1);
Texture3D<int> _BoundaryConditions : register (t2);

RWTexture3D<float3> _VelocityResult : register (u0);

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeProjection(uint3 id : SV_DispatchThreadID)
{
	if (_BoundaryConditions[id] > 0)
	{
		_VelocityResult[id] = float3(0,0,0);
		return;
	}
    //get size of fluid field
	uint3 dimensions;
    _Velocity.GetDimensions(dimensions.x, dimensions.y, dimensions.z);

 
    uint3 LeftCell = uint3(max(0, id.x - 1), id.y, id.z);
    uint3 RightCell = uint3(min(id.x + 1, dimensions.x - 1), id.y, id.z);

    uint3 BottomCell = uint3(id.x, max(0, id.y - 1), id.z);
    uint3 TopCell = uint3(id.x, min(dimensions.y - 1, id.y + 1), id.z);

    uint3 DownCell = uint3(id.x, id.y, max(0, id.z - 1));
    uint3 UpCell = uint3(id.x, id.y, min(dimensions.z - 1, id.z + 1));


    float L = _Pressure[LeftCell];
    float R = _Pressure[RightCell];

    float B = _Pressure[BottomCell];
    float T = _Pressure[TopCell];

    float D = _Pressure[DownCell];
    float U = _Pressure[UpCell];

	float C = _Pressure[id];

    float3 mask = float3(1, 1, 1);
	float3 obstV = float3(0, 0, 0);

    if (_BoundaryConditions[LeftCell] > 0.1)
    {
        L = C;
        mask.x = 0;
    }
    if (_BoundaryConditions[RightCell] > 0.1)
    {
        R = C;
        mask.x = 0;
    }
    
    if (_BoundaryConditions[BottomCell] > 0.1)
    {
        B = C;
        mask.y = 0;
    }
    if (_BoundaryConditions[TopCell] > 0.1)
    {
        T = C;
        mask.y = 0;
    }
    
    if (_BoundaryConditions[DownCell] > 0.1)
    {
        D = C;
        mask.z = 0;
    }
    if (_BoundaryConditions[UpCell] > 0.1)
    {
        U = C;
        mask.z = 0;
    }

    float3 v = _Velocity[id] - float3(R - L, T - B, U - D) * 0.5;

    _VelocityResult[id] = v * mask;
}