//Sebastian Tillgren

cbuffer GS_CONSTANT_BUFFER : register(b0)
{
	matrix theWorld;
	matrix theWorldViewProj;

}; 
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
};
struct GSOutput
{
	float4 Pos : SV_POSITION;	
	float3 Color: COLOR;
	float4 WorldPos : W_POSITION;
	float4 FaceNormal : NORMAL;
};

[maxvertexcount(3)]
void GS_main(
	triangle VS_OUT input[3],
	inout TriangleStream< GSOutput > output
)
{	

	float3 faceEdgeA = input[1].Pos - input[0].Pos;
	float3 faceEdgeB = input[2].Pos - input[0].Pos;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));	

	for (uint i = 0; i < 3; i++)
	{
		GSOutput element = (GSOutput)0;

		element.Pos = mul(float4(input[i].Pos), theWorldViewProj);
		//element.Pos = input[i].Pos;
		element.Color = input[i].Color;
		element.WorldPos = mul(float4(input[i].Pos), theWorld);
		element.FaceNormal = mul(float4(faceNormal, 0.0f), theWorld);

		output.Append(element);

	}
	//output.RestartStrip();

	/*for (uint i = 0; i < 3; i++)
	{
		GSOutput element = (GSOutput)0;

		element.Pos = mul(float4(input[i].Pos) + float4(faceNormal / 2, 0.0f), theWorldViewProj);
		element.Tex = input[i].Tex;
		element.WorldPos = mul(float4(input[i].Pos) + float4(faceNormal / 2, 0.0f), theWorld);
		element.FaceNormal = mul(float4(faceNormal, 0.0f), theWorld);

		output.Append(element);
	}	*/
}