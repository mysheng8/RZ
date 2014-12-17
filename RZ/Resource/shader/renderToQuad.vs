cbuffer cBuffer  
{  
    matrix wvpMatrix; 

}; 

struct VSInputType  
{  
    float4 position		: POSITION;  
	float2 uv			: UV;

};

struct VSOutputType  
{  
    float4 position		: SV_POSITION;  
	float2 uv			: TEXCOORD; 
}; 

VSOutputType RenderToQuadVertexShader(VSInputType input)  
{  
    VSOutputType output;  

    // Change the position vector to be 4 units for proper matrix calculations.  
    input.position.w = 1.0f;  
  
    output.position = mul(input.position, wvpMatrix);  
	output.uv=input.uv;  
    return output;  
}