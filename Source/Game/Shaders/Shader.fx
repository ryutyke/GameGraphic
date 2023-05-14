#define NUM_LIGHTS (1)

Texture2D txDiffuse : register(t0);
Texture2D txSpecular : register(t1);
SamplerState samLinear : register(s0);
SamplerState samSpecularLinear : register(s1);

cbuffer cbChangeOnCamaraMovement : register(b0)
{
	matrix View;
	float4 CameraPosition;
}

cbuffer cbChangeOnResize : register(b1)
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b2)
{
	matrix World;
	float4 OutputColor;
};

cbuffer cbLights : register(b3)
{
	float4 LightPositions[NUM_LIGHTS];
	float4 LightColors[NUM_LIGHTS];
};

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

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = normalize(mul(float4(input.Normal, 0.0f), World).xyz);
	output.WorldPosition = mul(input.Position, World);

	output.TexCoord = input.TexCoord;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float3 viewDirection = normalize(CameraPosition.xyz - input.WorldPosition);
	float4 color = txDiffuse.Sample(samLinear, input.TexCoord);
	float4 specularColor = txSpecular.Sample(samSpecularLinear, input.TexCoord);

	float3 ambient = 0;
	float3 diffuse = 0;
	float3 specular = 0;

	for (uint i = 0; i < NUM_LIGHTS; i++)
	{
		float3 LightDirection = normalize(input.WorldPosition - LightPositions[i].xyz);
		diffuse += (max(dot(input.Normal, -LightDirection), 0.0f) * LightColors[i]).xyz;
		specular += (pow(max(dot(reflect(LightDirection, input.Normal), viewDirection), 0.0f), 4.0f) * LightColors[i] * specularColor).rgb;
		ambient += float3(0.1f, 0.1f, 0.1f) * LightColors[i].rgb;
	}

	return float4(saturate((diffuse + specular + ambient) * color.rgb), color.a);
}

float4 PSSolid(PS_INPUT input) : SV_Target
{
	return OutputColor;
}