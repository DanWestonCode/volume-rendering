// Reference:
// http://graphicsrunner.blogspot.de/2009/01/volume-rendering-101.html

#pragma pack_matrix(row_major)

// Textures and samplers
Texture3D<float> txVolume : register(t0);

Texture2D<float4> txPositionFront : register(t1);
Texture2D<float4> txPositionBack  : register(t2);

SamplerState samplerLinear : register(s0);

// Constants and constant buffer variables
static const uint g_iMaxIterations = 128;

// Diagonal of a unit cube has length sqrt(3)
static const float g_fStepSize = sqrt(3.f)/g_iMaxIterations;

// for vertex shader
cbuffer cbEveryFrame : register(b0)
{
	matrix mWVP;
}

// for pixel shader
cbuffer cbImmutable : register(b0)
{
	float2 g_fInvWindowSize;
}

// Structures
struct VSInput
{
	float4 pos : POSITION;
};

struct PSInput
{
	float4 pos : SV_POSITION;
};


// Vertex shader
PSInput RayCastVS(VSInput input)
{
	PSInput output;
	// Change the position vector to be 4 units for proper matrix calculations.
	input.pos.w = 1.0f;
	// transform position
	output.pos = mul(mWVP, input.pos);
	return output;
}

// Pixel shader
float4 RayCastPS(PSInput input) : SV_TARGET
{
	// Get the current pixel location on the screen. 
	// This is used to sample the front and back textures mapped to the cube	
	float2 tex = input.pos.xy * g_fInvWindowSize;

	// Now read the cube frotn to back - "sample from front to back"	
	float3 pos_front = txPositionFront.Sample(samplerLinear, tex);
	float3 pos_back = txPositionBack.Sample(samplerLinear, tex);
 
	// Calculate the direction the ray is cast
	float3 dir = normalize(pos_back - pos_front);

	// Single step: direction times delta step - g_fStepSize is precaluclated
	float3 step = g_fStepSize * dir;

	// The current position - remember we start from the front
	float3 v = pos_front;

	// Accumulate result: value and transparency (alpha)
	float2 result = float2(0, 0);
 
	// iterate for the volume, sampling along the way at equidistant steps 
	for (uint i = 0; i < g_iMaxIterations; ++i)
	{
		// sample the texture accumlating the result as we step through the texture
		float2 src = txVolume.Sample(samplerLinear, v).rr;
		//src.y *= .5f;

		// Front to back blending
		result += (1 - result.y)*src.y * src;

		// Advance the current position
		v += step;
	}
 
	return float4(result.r, result.r, result.r, result.y);
}
