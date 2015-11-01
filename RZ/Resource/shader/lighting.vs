cbuffer MatrixBuffer  
{  
	float4 lightDir;
    matrix worldMatrix;  
    matrix viewMatrix;  
    matrix projectionMatrix;  
}; 

struct VertexInputType  
{  
    float4 position : POSITION;  
    float4 normal : NORMAL;
	float4 color : COLOR;
	float2 uv:UV;
};

struct PixelInputType  
{  
    float4 position : SV_POSITION;  
    float4 normal : NORMAL;
	float4 color : COLOR;
	float2 uv:UV;   
}; 

PixelInputType ColorVertexShader(VertexInputType input)  
{  
    PixelInputType output;  
      
  
    // Change the position vector to be 4 units for proper matrix calculations.  
    input.position.w = 1.0f;  
	input.normal.w = 1.0f; 
  
    // Calculate the position of the vertex against the world, view, and projection matrices.  
    output.position = mul(input.position, worldMatrix);  
    output.position = mul(output.position, viewMatrix);  
    output.position = mul(output.position, projectionMatrix);  
      
    // Store the input color for the pixel shader to use.  
    output.normal = input.normal;
	
	output.uv=input.uv;  
	output.color=input.color;
    return output;  
}