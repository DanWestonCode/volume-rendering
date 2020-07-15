#pragma pack_matrix(row_major)

// Constant buffer variables
cbuffer cbEveryFrame : register(b0)
{
	matrix mWVP;
}

// Structures
struct VSInput
{
	float4 pos : POSITION;
};


struct PSInput
{
	float4 pos : SV_POSITION;
	float4 tex : TEXCOORD0;		// Position in model coordinates
};
// Vertex shader
PSInput ModelPositionVS(VSInput input)
{
	PSInput output;
	output.pos = mul(mWVP, input.pos);

	// Pass position (in model coordinates) to texture coordinate register
	output.tex = 0.5*(input.pos + 1);

	return output;
}

// Pixel shaders
float4 ModelPositionPS(PSInput input) : SV_TARGET
{
	// Make use of linear interpolation
	return input.tex;
}
