cbuffer cBuffer  
{  
    matrix worldMatrix;  
    matrix viewMatrix;  
    matrix projectionMatrix; 
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
  
	output.position = mul(input.position, worldMatrix);  
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.color=input.color;
	output.uv=input.uv; 
	output.wpos=input.position;
    return output;  
}