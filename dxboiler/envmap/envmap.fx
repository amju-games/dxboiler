//--------------------------------------------------------------------------------------
// envmap.fx - really simple shader
// Texturing + "fake" phong lighting effect using the 
//  interpolated vertex normal
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

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
	float2 Tex: TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex: TEXCOORD1;
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

	output.Tex = input.Tex;
    
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    // Sphere map
	float3 view = float3(0, 0, -1);
	input.Norm = normalize(input.Norm);
	float3 uv = reflect(view, input.Norm);
	uv.z += 1.0;
	uv = normalize(uv);
	float2 uv2 = uv.xy * 0.5 + 0.5;

    float4 c =  txDiffuse.Sample(samLinear, uv2);

	// Final colour for pixel is normal * texel.
	float s = max(0.5, min(1, input.Norm.x + input.Norm.y));
    return float4(s * c.r, s * c.g, s * c.b, 1.0f * c.a);
}


