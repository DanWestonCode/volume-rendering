/// <summary>
/// ComputeBoundaryConditions.hlsl
///
/// About:
/// ComputeBoundaryConditions sets the boarders of the velocity field 
/// to be solid. The shader enforces the "no-slip" boundary conditions, dictating all
/// the boarders of the velocity field are not enterable by a fluid (http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html)
///
/// Created from a combinataion of:
/// cFluid3D.hlsl (ObstaclesComputeShader) - Valentin Hinov - https://github.com/Morji/Fluid-Simulation-DirectX11
/// ComputeObstacles.compute - Scrawk Blog - https://scrawkblog.com/2014/01/09/gpu-gems-to-unity-3d-fluid-simulation/
/// </summary>

#define NUM_THREADS 8
//Boundary Conditions texture to write into (velocity field)
RWTexture3D<int> _BoundaryConditions;

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeBoundaryConditions(uint3 id : SV_DispatchThreadID)
{
	//get size of the fluid field
	uint3 _Size;
	_BoundaryConditions.GetDimensions(_Size.x, _Size.y, _Size.z);

    //default of the field is no collisons
    int obstacle = 0;
    
	//if current cell is edge of fluid field set to collision
	if(id.x-1 < 0) obstacle = 1;
    if(id.x+1 > (int)_Size.x-1) obstacle = 1;
    
    if(id.y-1 < 0) obstacle = 1;
    if(id.y+1 > (int)_Size.y-1) obstacle = 1;
    
    if(id.z-1 < 0) obstacle = 1;
    if(id.z+1 > (int)_Size.z-1) obstacle = 1;
    
	//Set the current cell in the field to the calculated obstacle
	_BoundaryConditions[id] = obstacle;
}