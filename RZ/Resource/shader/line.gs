#define THICKNESS 0.01f

cbuffer cBuffer  
{  
    matrix wvp;  
    float4 camPos;  
    float4 lineDir; 
}; 


struct VS_OUT  
{  
    float4 position		: SV_POSITION; 
	float4 color		: TEXCOORD0;
	float2 uv			: TEXCOORD1;
	float4 wpos			: POSITIONWS;
}; 

struct GS_OUT  
{  
    float4 position		: SV_POSITION;
	float4 color		: TEXCOORD0;
	float4 normal		: NORMALWS; 
	float2 uv			: TEXCOORD1;
	float4 wpos			: POSITIONWS;
}; 


[maxvertexcount(4)]
void LineGS(line VS_OUT gIn[2],inout TriangleStream<GS_OUT> triStream)
{
	/*
	float3 campos=float3(0,0,-10);
	campos.x = -(viewMatrix[0][0] * viewMatrix[3][0] + viewMatrix[0][1] * viewMatrix[3][1] + viewMatrix[0][2] * viewMatrix[3][2]);
    campos.y = -(viewMatrix[1][0] * viewMatrix[3][0] + viewMatrix[1][1] * viewMatrix[3][1] + viewMatrix[1][2] * viewMatrix[3][2]);
    campos.z = -(viewMatrix[2][0] * viewMatrix[3][0] + viewMatrix[2][1] * viewMatrix[3][1] + viewMatrix[2][2] * viewMatrix[3][2]);
	*/

	float3 planeNormal = gIn[1].wpos.xyz - camPos.xyz;
	//planeNormal.y = 0.0f;
	planeNormal = normalize(planeNormal);

	float4 pos1=gIn[0].wpos;
	float4 pos2=gIn[1].wpos;
	float3 tangent = pos2.xyz - pos1.xyz; 
	tangent = normalize(tangent);



	float3 sideVec = normalize(cross(planeNormal, tangent)); 
	float3 width1 = sideVec*THICKNESS; //* strandWidth[0]; 
	float3 width2 = sideVec*THICKNESS; //* strandWidth[1]; 


	float4 wv[4];
	wv[0] = float4( pos1.xyz + width1, 1 ); 
	wv[1] = float4( pos1.xyz - width1, 1 ); 
	wv[2] = float4( pos2.xyz + width2, 1 ); 
	wv[3] = float4( pos2.xyz - width2, 1 ); 

	// Get billboards texture coordinates
	float2 texCoord[4];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);

	float4 color[4];
	color[0] = gIn[0].color;
	color[1] = gIn[0].color;
	color[2] = gIn[1].color;
	color[3] = gIn[1].color;

	GS_OUT gOut;
	for(int i = 0; i < 4; i++)
	{
		gOut.position = mul(wv[i], wvp);  

		gOut.wpos = wv[i];
		gOut.normal = float4(planeNormal,1);

		// These will not be used for billboards
		gOut.uv = texCoord[i];
		gOut.color = color[i];

		triStream.Append(gOut);
	}
	triStream.RestartStrip();



}