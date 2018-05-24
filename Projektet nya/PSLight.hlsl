Texture2D NormalTexture :register(t0);
Texture2D DiffuseAlbedoTexture : register(t1);
Texture2D SpecularAlbedoTexture : register(t2);
Texture2D PositionTexture : register(t3);

cbuffer LightParams
{
	float3 LightPos;
	float3 LightColor;
	float3 LightDirection;
	float2 SpotlightAngles;
	float4 LightRange;
};

cbuffer CameraPos
{
	float3 CamPos;
};

void GetGBufferAttributes(in float2 screenPos, out float3 normal, out float3 position,
	out float3 diffuseAlbedo, out float3 specularAlbedo,
	out float specularPower)
{
	int3 sampleIndices = int3 (screenPos.xy, 0);

	normal = NormalTexture.Load(sampleIndices).xyz;
	position = PositionTexture.Load(sampleIndices).xyz;
	diffuseAlbedo = DiffuseAlbedoTexture.Load(sampleIndices).xyz;
	float4 specular = SpecularAlbedoTexture.Load(sampleIndices);

	specularAlbedo = specular.xyz;
	specularPower = specular.w;
}

float3 CalcLighting(in float3 position, in float3 normal, in float3 diffuseAlbedo,
	in float3 specularAlbedo, in float specularPower)
{
	float3 lightVector = 0;
	float attenuation = 1.0f;

	lightVector = LightPos - position; 
	float distance = length(lightVector);
	attenuation = max(0, 1.0f - (distance / LightRange.x));

	lightVector = lightVector / distance;

	float3 lightVector2 = LightDirection;
	float rho = dot(-lightVector, lightVector2);
	attenuation *= saturate((rho - SpotlightAngles.y) / (SpotlightAngles.x - SpotlightAngles.y));

	float nDotL = saturate(dot(normal, lightVector));
	float3 diffuse = nDotL * LightColor * diffuseAlbedo;

	float3 V = CamPos - position;
	float3 H = normalize(lightVector + V);
	float3 specular = pow(saturate(dot(normal, H)), specularPower) * LightColor * specularAlbedo.xyz * nDotL;

	return (diffuse + specular) * attenuation;
}

struct PSL_IN
{
	float4 Position : SV_POSITION;	
};

float4 PSL_main(in PSL_IN input) : SV_Target0
{
	float3 normal;
	float3 position;
	float3 diffuseAlbedo;
	float3 specularAlbedo;
	float specularPower;

	int3 sampleIndices = int3 (input.Position.xy, 0);

	normal = NormalTexture.Load(sampleIndices).xyz;

	//GetGBufferAttributes(input.Position.xy, normal, position,
	//	diffuseAlbedo, specularAlbedo, specularPower);

	//float3 lighting = CalcLighting(position, normal, diffuseAlbedo,
	//	specularAlbedo, specularPower);

	//return float4(lighting, 1.0f);
	return float4(normal, 1.0f);
		//float4(0, 1, 0, 1.f);
}