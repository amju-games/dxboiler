//--------------------------------------------------------------------------------------
// FullScreenShader.fx 
//--------------------------------------------------------------------------------------


Texture2D tex : register(t0);
SamplerState samAniso : register( s0 );

cbuffer ConstantBuffer
{
	float2 MouseCoord;
	float2 Padding;
}

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = input.Pos;
	output.UV = input.UV;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	//float2 uvVector = normalize(MouseCoord - input.UV);
	//float UVdistance = distance(MouseCoord,input.UV);

	//float4 finalColor = 0;
	//float stepLength =  UVdistance/ 64;
	//float4 blurred = 0;

	//for(int i = 0; i < 64; ++i)
	//{
	//	blurred += tex.Sample(samAniso,input.UV+(uvVector*stepLength*i));
	//}

 //   finalColor = blurred / 64;


	//

	////float4 finalColor = float4(stepLength,stepLength,stepLength,1);//tex.Sample(samAniso,MouseCoord);//input.UV);
	//return finalColor;
	float BlurDistance = 0.5f;
	float2 BlurVec = MouseCoord - input.UV;
      
	normalize(BlurVec);
   
	float4 Color = tex.Sample(samAniso,input.UV);
 
	float steps = 128;
   
	for(int i = 0; i < steps; ++i)
	{
		float x = saturate(input.UV.x + BlurVec.x*(BlurDistance/steps)*i*(i/steps));
		float y = saturate(input.UV.y + BlurVec.y*(BlurDistance/steps)*i*(i/steps));

		Color += tex.Sample(samAniso,float2(x,y));	
	}
   
	Color /= steps+1;

	return Color;
}