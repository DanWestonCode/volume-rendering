#define NUM_THREADS 8

cbuffer AdvectionBuffer : register (b0)
{
	float dissipation;
	float dt;
	float decay;
	float padding;
}

//SRVS
Texture3D<float3> _AdvectionTargetReadA : register (t0);
Texture3D<float3> _AdvectionTargetReadB : register (t1);
Texture3D<float3> _AdvectionTargetReadC : register (t2);
Texture3D<int>  _BoundaryConditions  : register (t3);
Texture3D<float3> _Velocity : register (t4);

//UAV
RWTexture3D<float3> _AdvectionTargetWrite : register (u0);

// Samplers
SamplerState linearSampler : register (s0);

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeAdvectionMacCormack(uint3 id : SV_DispatchThreadID)
{
	//if there's an obstacle bail
	if (_BoundaryConditions[id] > 0)
	{
		_AdvectionTargetWrite[id] = float3(0, 0, 0);
		return;
	}

	uint3 _Size;
	//get dimensions of texture
	_Velocity.GetDimensions(_Size.x, _Size.y, _Size.z);
	float3 prevPos = id - dt * _Velocity[id];
	uint j = (uint3) prevPos;

	// Get the values of nodes that contribute to the interpolated value.  
	float3 r0 = _AdvectionTargetReadA[j + uint3(0, 0, 0)];
	float3 r1 = _AdvectionTargetReadA[j + uint3(1, 0, 0)];
	float3 r2 = _AdvectionTargetReadA[j + uint3(0, 1, 0)];
	float3 r3 = _AdvectionTargetReadA[j + uint3(1, 1, 0)];
	float3 r4 = _AdvectionTargetReadA[j + uint3(0, 0, 1)];
	float3 r5 = _AdvectionTargetReadA[j + uint3(1, 0, 1)];
	float3 r6 = _AdvectionTargetReadA[j + uint3(0, 1, 1)];
	float3 r7 = _AdvectionTargetReadA[j + uint3(1, 1, 1)];

	// Determine a valid range for the result.
	float3 lmin = min(r0, min(r1, min(r2, min(r3, min(r4, min(r5, min(r6, r7)))))));
	float3 lmax = max(r0, max(r1, max(r2, max(r3, max(r4, max(r5, max(r6, r7)))))));

	float3 phi_n_1_hat = _AdvectionTargetReadA.SampleLevel(linearSampler, prevPos, 0);
	float3 phi_n_hat = _AdvectionTargetReadB.SampleLevel(linearSampler, prevPos, 0);
	float3 phi_n = _AdvectionTargetReadC.SampleLevel(linearSampler, prevPos, 0);

	float3 s = phi_n_1_hat + 0.5f*(phi_n - phi_n_hat);

	// clamp results to desired range
	s = clamp(s, lmin, lmax);

	float3 finalResult = s*dissipation;
	if (decay > 0.0f) 
	{
		finalResult.x = max(0, finalResult.x - decay);
	}
	_AdvectionTargetWrite[id] = finalResult;
}