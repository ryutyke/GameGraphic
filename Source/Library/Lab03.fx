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

cbuffer cbChangeOnCameraMovement  : register(b0)
{
	matrix View;
	float4 CameraPosition;
};
cbuffer cbProjection : register(b1)
{
	matrix Projection;
};
cbuffer cbChangesEveryFrame : register(b2)
{
	matrix World;
	float4 OutputColor;
};

cbuffer cbLights : register(b3) {
	float4 LightPositions[2];
	float4 LightColors[2];
}

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
	float3 Normal : NORMAL;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 WorldPosition : WORLDPOS;
};

PS_INPUT VS(VS_INPUT input) {
	PS_INPUT output = (PS_INPUT)0;

	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Normal = normalize(mul(float4(input.Normal, 0.0f), World).xyz);
	output.WorldPosition = mul(input.Position, World);
	
	output.TexCoord = input.TexCoord;
	return output;
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

float4 PS(PS_INPUT input) : SV_Target
{
	float4 color = txDiffuse.Sample(samLinear, input.TexCoord);
	float3 diffuse = 0;
	for (uint i = 0; i < 2; i++)
	{
		float3 LightDirection = normalize(input.WorldPosition - LightPositions[i].xyz);
		diffuse += (dot(input.Normal, -LightDirection) * LightColors[i]).xyz;
	}

	float3 ambient = 0;
	for (uint i = 0; i < 2; i++) 
	{
		ambient += float3(0.1f, 0.1f, 0.1f);
	}

	float3 viewDirection = normalize(CameraPosition.xyz - input.WorldPosition);
	float3 specular = 0;
	for (uint i = 0; i < 2; i++)
	{
		float3 LightDirection = normalize(input.WorldPosition - LightPositions[i].xyz);
		specular += (pow(dot(reflect(LightDirection, input.Normal), viewDirection), 4.0f) * LightColors[i]).rgb;
	}

	return float4(saturate((diffuse + specular + ambient) * color.rgb), color.a);
}

float4 PSSolid(PS_INPUT input) : SV_Target
{
	return OutputColor;
}

