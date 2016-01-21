#define NUMPOINTS 8

struct VS_OUTPUT  
{  
    float4 position		: SV_POSITION; 
	float4 color		: TEXCOORD0;
	float2 uv			: TEXCOORD1; 
	float4 wpos			: POSITIONWS;
}; 

struct HS_CONSTANT_OUTPUT
{
    float edges[2]             : SV_TessFactor;
};

struct HS_OUTPUT
{
    float4 position		: SV_POSITION; 
	float4 color		: TEXCOORD0;
	float2 uv			: TEXCOORD1; 
	float4 wpos			: POSITIONWS;
};

HS_CONSTANT_OUTPUT HSConst()
{
    HS_CONSTANT_OUTPUT output;

    output.edges[0] = 1.0f; // Detail factor (see below for explanation)
    output.edges[1] = 32.0f; // Density factor

    return output;
}

[domain("isoline")]
[partitioning("integer")]
[outputtopology("line")]
[outputcontrolpoints(NUMPOINTS)]
[patchconstantfunc("HSConst")]
HS_OUTPUT LineHS(InputPatch<VS_OUTPUT, NUMPOINTS> ip, uint id : SV_OutputControlPointID)
{
    HS_OUTPUT output;
    output.position = ip[id].position;
	output.color = ip[id].color;
	output.uv = ip[id].uv;
	output.wpos = ip[id].wpos;
    return output;
}



