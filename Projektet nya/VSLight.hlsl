cbuffer VSL_CONSTANT_BUFFER : register(b0)
{
	matrix theView;
	matrix theProj;
}; 
struct VSL_IN
{	
	float4 Position : POSITION;
};

struct PSL_IN
{
	float4 screenPos : SV_POSITION;
};

PSL_IN VSL_main(VSL_IN pos) : SV_POSITION
{
	PSL_IN output;

	output.screenPos = pos.Position;

	return output;
}