//Sebastian Tillgren

Texture2D txDiffuse : register(t0);
SamplerState sampAni : register(s0)
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
	MaxAnisotropy = 4;
	MaxMipLevel = 0;
	MipMapLevelOfDetailBias = 0;
};

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float3 specularAlbedo;
	float specularPower;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
	float4 WorldPos : W_POSITION;
	float4 FaceNormal : NORMAL;
};

struct PS_OUT
{
	float4 Normal : SV_Target0;
	float4 DiffuseAlbedo : SV_Target1;
	float4 SpecularAlbedo : SV_Target2;
	float4 Position : SV_Target3;

};
PS_OUT PS_main(GS_OUT input)
{	
	PS_OUT output;
	
	//return float4(input.Color, 1.0f);

	float3 diffuseAlbedo = txDiffuse.Sample(sampAni, input.Color).rgb; //kan vara fel här någonstans

	float3 normal = normalize(input.FaceNormal.xyz);

	output.Normal = float4(normal, 1.0f);
	output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	output.SpecularAlbedo = float4(specularAlbedo, specularPower);
	output.Position = input.WorldPos;

	return output;
	/*
	float3 newColour = float3(0.2, 0.2, 0.2) * s;

	float4 lightPos = float4(0.0f, 0.0f, -5.0f, 1.0f);
	float4 lightColour = float4(1.0f, 0.5f, 0.0f, 1.0f);
	float4 pixelPos = input.WorldPos;
	float lightIntensity = 2;
	float epsilon = 0.00001;
	float3 pixelNormal = normalize(input.FaceNormal.xyz);

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