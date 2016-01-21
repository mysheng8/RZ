#define NUMLINES 128
#define RADIUS 0.4

cbuffer matrixCBuffer  
{  
    matrix wvp;  
    float4 camPos;  
    float4 lineDir; 
}; 

cbuffer instanceCBuffer  
{  
	float2 randomPos[NUMLINES]; 
};

struct VSInputType  
{  
    float4 position		: POSITION;  
	float4 color		: COLOR;
	float2 uv			: UV;

};

struct VSOutputType  
{  
    float4 position		: SV_POSITION; 
	float4 color		: TEXCOORD0;
	float2 uv			: TEXCOORD1; 
	float4 wpos			: POSITIONWS;
}; 

VSOutputType LineVS(VSInputType input)  
{  
    VSOutputType output;  

    // Change the position vector to be 4 units for proper matrix calculations.  
    input.position.w = 1.0f;  
  
	output.position = mul(input.position, wvp);  
	output.color=input.color;
	output.uv=input.uv; 
	output.wpos=input.position;
    return output;  
}

VSOutputType LineClusterVS(VSInputType input,uint id : SV_InstanceID)  
{  
    VSOutputType output;  

    // Change the position vector to be 4 units for proper matrix calculations.  
    input.position.w = 1.0f;  
	float2 offset=randomPos[id];

	float3 up=float3(0,1,0);
	float3 sideVec=normalize(cross(up,lineDir.xyz));
	float3 upVec=normalize(cross(sideVec,lineDir.xyz));
	float3 pos=input.position.xyz + RADIUS*sideVec*offset.x+RADIUS*upVec*offset.y;
	output.position = mul(float4(pos,0), wvp);  
	output.color=input.color;
	output.uv=input.uv; 
	output.wpos=float4(pos,0);
    return output;  
}