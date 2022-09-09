/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};
Texture2D a: register(t0);
RWTexture2D<float4> b;
SamplerState SampleType : register(s0);

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	// Store the input color for the pixel shader to use.
	output.color = input.color;

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	return input.color;
}

[numthreads(256,1,1)]
void CS(int3 dispatchThreadID : SV_DispatchThreadID)
{
	float4 textureColor = float4(1,1,1,1);

	b[dispatchThreadID.xy] = textureColor;//a[dispatchThreadID.xy];
}