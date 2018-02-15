//Sebastian Tillgren

Texture2D txDiffuse : register(t0);
SamplerState sampAni
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
	MaxAnisotropy = 4;
	MaxMipLevel = 0;
	MipMapLevelOfDetailBias = 0;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
	float4 WorldPos : W_POSITION;
	float4 FaceNormal : NORMAL;
};

float4 PS_main(GS_OUT input) : SV_Target
{	
	return float4(input.Color, 1.0f);

	/*float3 s = txDiffuse.Sample(sampAni, input.Color).xyz;
	float3 newColour = float3(0.2, 0.2, 0.2) * s;

	float4 lightPos = float4(0.0f, 0.0f, -5.0f, 1.0f);
	float4 lightColour = float4(1.0f, 0.5f, 0.0f, 1.0f);
	float4 pixelPos = input.WorldPos;
	float lightIntensity = 2;
	float epsilon = 0.00001;
	float3 pixelNormal = input.FaceNormal.xyz;

	float distanceToPoint = distance(pixelPos.xyz, lightPos.xyz);

	float cosOfAngle = dot(normalize(pixelNormal), normalize(lightPos.xyz - pixelPos.xyz));
	if (cosOfAngle > 0.0)
	{
		float3 mixColour = cosOfAngle*(lightColour.xyz*s)*lightIntensity*(1.0 / distanceToPoint);
		newColour += mixColour;
	}

	newColour = min(newColour, float3(1.0, 1.0, 1.0));

	return float4(newColour, 1.0f);*/
};