#define NUM_THREADS 8
RWTexture3D<float3> _Clear : register(u0);

[numthreads(NUM_THREADS, NUM_THREADS, NUM_THREADS)]
void ComputeClear(uint3 id : SV_DispatchThreadID)
{
    _Clear[id] = 0;
}