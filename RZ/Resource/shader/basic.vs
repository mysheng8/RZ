cbuffer cBuffer  
{  
    matrix worldMatrix;  
    matrix viewMatrix;  
    matrix projectionMatrix; 
}; 
struct VSInputType  
{  
    float4 position		: POSITION;  
    float4 normal		: NORMAL;
	float4 tangent		: TANGENT;
	float4 color		: COLOR;
	float2 uv			: UV;
};

struct VSOutputType  
{  
    float4 position		: SV_POSITION;  
    float3 normal		: NORMALWS;
	float2 uv			: TEXCOORD; 
	float4 wPos			: POSITIONWS;
	float3 tangent		: TANGENTWS;
	float3 binormal		: BINORMALWS;
	float4 color		: COLOR;
}; 

VSOutputType BasicVertexShader(VSInputType input)  
{  
    VSOutputType output;  
      
  
    // Change the position vector to be 4 units for proper matrix calculations.  
    input.position.w = 1.0f;  
	input.normal.w = 1.0f; 
  
    output.position = mul(input.position, worldMatrix);  
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);



    output.normal = normalize(mul(input.normal,worldMatrix)).xyz;
	output.tangent = normalize(mul(input.tangent,worldMatrix)).xyz;
	
	output.binormal = normalize(cross(output.normal,output.tangent))*input.tangent.w;


	output.wPos=mul(input.position,worldMatrix);
	output.uv=input.uv; 
	output.color=input.color; 
    return output;  
}