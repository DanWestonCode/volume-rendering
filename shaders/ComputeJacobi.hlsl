/// <summary>
/// ComputeJacobi.hlsl
///
/// About:
/// ComputeJacobi - Solves the pressure poisson system
/// Jacobi is the second part of the projection step. 
///
/// Based from:
/// cFluid3D.hlsl (BuoyancyComputeShader) - Valentin Hinov - https://github.com/Morji/Fluid-Simulation-DirectX11
/// ComputeDivergence.compute - Scrawk Blog - https://scrawkblog.com/2014/01/09/gpu-gems-to-unity-3d-fluid-simulation/
/// </summary>
#define NUM_THREADS 8

RWTexture3D<float> _pressureResult : register (u0);

Texture3D<float> _Pressure : register(t0);
Texture3D<int> _BoundaryConditions : register(t1);
Texture3D<float> _Divergence : register(t2);

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeJacobi( uint3 id : SV_DispatchThreadID )
{
    //Get dimensions of the fluid field
	uint3 dimensions;
	_Pressure.GetDimensions(dimensions.x, dimensions.y, dimensions.z);

	//For the current cell get neighbouring positions
    uint3 LeftCell = uint3(max(0, id.x - 1), id.y, id.z);
    uint3 RightCell = uint3(min(id.x + 1, dimensions.x - 1), id.y, id.z);
	
    uint3 TopCell = uint3(id.x, min(id.y + 1, dimensions.y - 1), id.z);
    uint3 BottomCell = uint3(id.x, max(id.y - 1, 0), id.z);

    uint3 UpCell = uint3(id.x, id.y, min(id.z + 1, dimensions.z - 1));
    uint3 DownCell = uint3(id.x, id.y, max(id.z - 1, 0));

    //Get the current pressure at the cell
	float CurrentCellPressure = _Pressure[id];

    //Get surrounding cell pressures
    float TopCellPressure = _Pressure[TopCell];
    float BottomCellPressure = _Pressure[BottomCell];

    float RightCellPressure = _Pressure[RightCell];
    float LeftCellPressure = _Pressure[LeftCell];

    float UpCellPressure = _Pressure[UpCell];
    float DownCellPressure = _Pressure[DownCell];

    //Check the boundary conditions to remove fields with collisions 
    if (_BoundaryConditions[TopCell] > 0)
    {
        TopCellPressure = CurrentCellPressure;
    }
    if (_BoundaryConditions[BottomCell] > 0)
    {
        BottomCellPressure = CurrentCellPressure;
    }
    
    if (_BoundaryConditions[RightCell] > 0)
    {
        RightCellPressure = CurrentCellPressure;
    }
    if (_BoundaryConditions[LeftCell] > 0)
    {
        LeftCellPressure = CurrentCellPressure;
    }
    
    if (_BoundaryConditions[UpCell] > 0)
    {
        UpCellPressure = CurrentCellPressure;
    }
    if (_BoundaryConditions[DownCell] > 0)
    {
        DownCellPressure = CurrentCellPressure;
    }

	// Get the current divergence
	float divergence = _Divergence[id];

	_pressureResult[id] = (LeftCellPressure + RightCellPressure + BottomCellPressure + TopCellPressure + UpCellPressure + DownCellPressure - divergence) / 6.0;
}