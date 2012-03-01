//--------------------------------------------------------------------------------------
// simplest.fx - really simple shader
// No texturing or lighting, but "fake" phong lighting effect using the 
//  interpolated vertex normal
//--------------------------------------------------------------------------------------

// Constant (Uniform) variables:
// Modelview and projection matrices.
cbuffer ConstantBuffer 
{
	matrix View;
	matrix Projection;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
	float3 Norm : NORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;

	// Transform vertex by modelview * projection matrices    
    output.Pos = mul( input.Pos, View );
    output.Pos = mul( output.Pos, Projection );

	// Transform normal by modelview matrix
    output.Norm = mul( input.Norm, View );
    
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	// Final colour for pixel just uses the normal.
	float s = min(1, input.Norm.x + input.Norm.y);
    return float4(s, s, s, 1.0f );
}


