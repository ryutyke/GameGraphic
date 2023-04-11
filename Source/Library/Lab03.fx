float4 PS(float4 Pos : SV_POSITION) : SV_TARGET
{
   return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

float4 VS(float4 Pos : POSITION) : SV_POSITION{
   return Pos;
}

//cbuffer ConstantBuffer : register(b0)
//{
//   matrix World;
//   matrix View;
//   matrix Projection;
//}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

cbuffer cbView : register(b0)
{
	matrix View;
};
cbuffer cbProjection : register(b1)
{
	matrix Projection;
};
cbuffer cbWorld : register(b2)
{
	matrix World;
};

VS_OUTPUT VS(float4 Pos:POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Color = Color;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
   return input.Color;
}

struct VS_INPUT
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input) {
	PS_INPUT output = (PS_INPUT)0;

	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	output.TexCoord = input.TexCoord;
	return output;
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

float4 PS(PS_INPUT input) : SV_Target
{
   return txDiffuse.Sample(samLinear, input.TexCoord);
}