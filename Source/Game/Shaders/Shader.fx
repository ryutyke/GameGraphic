#define NUM_LIGHTS (2)

Texture2D aTextures[2] : register(t0);
SamplerState aSamplers[2] : register(s0);

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
	bool HasNormalMap;
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
	float3 Tangent : TANGENT;
	float3 Bitangent : BITANGENT;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 WorldPosition : WORLDPOS;
	float3 Tangent : TANGENT;
	float3 Bitangent : BITANGENT;
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

	if (HasNormalMap)
	{
		output.Tangent = normalize(mul(float4(input.Tangent, 0.0f), World).xyz);
		output.Bitangent = normalize(mul(float4(input.Bitangent, 0.0f), World).xyz);
	}

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float3 normal = normalize(input.Normal);

	if (HasNormalMap)
	{
		// Sample the pixel in the bump map.
		float4 bumpMap = aTextures[1].Sample(aSamplers[1], input.TexCoord);
		// Expand the range of the normal value from (0, +1) to (-1, +1).
		bumpMap = (bumpMap * 2.0f) - 1.0f;
		// Calculate the normal from the data in the bump map.
		float3 bumpNormal = (bumpMap.x * input.Tangent) + (bumpMap.y * input.Bitangent) + (bumpMap.z * normal);
		// Normalize the resulting bump normal and replace existing normal
		normal = normalize(bumpNormal);
	}

	float3 viewDirection = normalize(CameraPosition.xyz - input.WorldPosition);
	float4 color = aTextures[0].Sample(aSamplers[0], input.TexCoord);

	float3 ambient = 0;
	float3 diffuse = 0;
	float3 specular = 0;

	for (uint i = 0; i < NUM_LIGHTS; i++)
	{
		float3 LightDirection = normalize(input.WorldPosition - LightPositions[i].xyz);
		diffuse += (max(dot(normal, -LightDirection), 0.0f) * LightColors[i]).xyz;
		specular += (pow(max(dot(reflect(LightDirection, normal), viewDirection), 0.0f), 4.0f) * LightColors[i]).rgb;
		ambient += float3(0.1f, 0.1f, 0.1f) * LightColors[i].rgb;
	}

	return float4(saturate((diffuse + specular + ambient) * color.rgb), color.a);
}

float4 PSSolid(PS_INPUT input) : SV_Target
{
	return OutputColor;
}