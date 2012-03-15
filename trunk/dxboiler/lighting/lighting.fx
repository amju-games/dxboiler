//--------------------------------------------------------------------------------------
// lighting.fx 
//--------------------------------------------------------------------------------------

// Constant (Uniform) variables:
// Modelview and projection matrices, light dir and colour for 2 lights.
// This struct needs to match the one in the code!
cbuffer ConstantBuffer 
{
	matrix View;
	matrix Projection;
	float4 vLightDir[2];
	float4 vLightColor[2];
}

struct VS_INPUT
{
    float4 Pos : POSITION;
	float3 Norm : NORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
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
    float4 finalColor = 0;
    
    //do NdotL lighting for 2 lights
    for(int i=0; i<2; i++)
    {
        finalColor += saturate( dot( (float3)vLightDir[i],input.Norm) * vLightColor[i] );
    }
    finalColor.a = 1;
    return finalColor;
}


