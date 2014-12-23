#include"RZShader.h"

using namespace std;
using namespace RZ;

RZShader::RZShader()  
{  
    m_vertexShader = 0;  
    m_pixelShader = 0;  
    m_layout = 0;  
    m_matrixBuffer = 0;  
}

RZShader::RZShader(const RZShader& other)  
{  
}  
  
  
RZShader::~RZShader()  
{  
}

bool RZShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, int vertexStart, const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)  
{  
    bool result;  
  
  
    // Set the shader parameters that it will use for rendering.  
    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);  
    if(!result)  
    {  
        return false;  
    }  
  
    // Now render the prepared buffers with the shader.  
    RenderShader(deviceContext, indexCount,indexStart,vertexStart);  
  
    return true;  
}

bool RZShader::Initialize(ID3D11Device* device, HWND hwnd, ID3D11Buffer* matrixBuffer, const char* vsFilename, const char* psFilename)  
{  
    HRESULT result;  
    ID3D10Blob* errorMessage;  
    ID3D10Blob* vertexShaderBuffer;  
    ID3D10Blob* pixelShaderBuffer;  
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
    unsigned int numElements;  
    D3D11_BUFFER_DESC matrixBufferDesc;  
	m_matrixBuffer=matrixBuffer;
  
    // Initialize the pointers this function will use to null.  
    errorMessage = 0;  
    vertexShaderBuffer = 0;  
    pixelShaderBuffer = 0;  
  
 
    // Compile the vertex shader code. 
    result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "BasicVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &vertexShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {   
        if(errorMessage)  
        {  
            OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);  
        }  
        else  
        {  
            MessageBox(hwnd, vsFilename, "Missing Shader File", MB_OK);  
        }  
  
        return false;  
    }  
  
    // Compile the pixel shader code.  
    result = D3DX11CompileFromFile(psFilename, NULL, NULL, "BasicPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,   
                       &pixelShaderBuffer, &errorMessage, NULL);  
    if(FAILED(result))  
    {    
        if(errorMessage)  
        {  
            OutputShaderErrorMessage(errorMessage, hwnd, psFilename);  
        }  
        else  
        {  
            MessageBox(hwnd, psFilename, "Missing Shader File", MB_OK);  
        }  
  
        return false;  
    }  
  
    // Create the vertex shader from the buffer.  
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);  
    if(FAILED(result))  
    {  
        return false;  
    }  
  
    // Create the pixel shader from the buffer.  
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);  
    if(FAILED(result))  
    {  
        return false;  
    }  
  
    // Now setup the layout of the data that goes into the shader.  
    // This setup needs to match the VertexType stucture in the ModelClass and in the shader.  
    polygonLayout[0].SemanticName = "POSITION";  
    polygonLayout[0].SemanticIndex = 0;  
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  
    polygonLayout[0].InputSlot = 0;  
    polygonLayout[0].AlignedByteOffset = 0;  
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;  
    polygonLayout[0].InstanceDataStepRate = 0;  

    polygonLayout[1].SemanticName = "NORMAL";  
    polygonLayout[1].SemanticIndex = 0;  
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;  
    polygonLayout[1].InputSlot = 0;  
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;  
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;  
    polygonLayout[1].InstanceDataStepRate = 0;  

	polygonLayout[2].SemanticName = "TANGENT";  
    polygonLayout[2].SemanticIndex = 0;  
    polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;  
    polygonLayout[2].InputSlot = 0;  
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;  
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;  
    polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "COLOR";  
    polygonLayout[3].SemanticIndex = 0;  
    polygonLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  
    polygonLayout[3].InputSlot = 0;  
    polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;  
    polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;  
    polygonLayout[3].InstanceDataStepRate = 0; 

	polygonLayout[4].SemanticName = "UV";  
    polygonLayout[4].SemanticIndex = 0;  
    polygonLayout[4].Format = DXGI_FORMAT_R32G32_FLOAT;  
    polygonLayout[4].InputSlot = 0;  
    polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;  
    polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;  
    polygonLayout[4].InstanceDataStepRate = 0;  

    // Get a count of the elements in the layout.  
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);  
  
    // Create the vertex input layout.  
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);  
    if(FAILED(result))  
    {  
        return false;  
    }  
  
    // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.  
    vertexShaderBuffer->Release();  
    vertexShaderBuffer = 0;  
  
    pixelShaderBuffer->Release();  
    pixelShaderBuffer = 0;  
  
    return true;  
}

void RZShader::Shutdown()  
{  
    // Release the layout.  
    if(m_layout)  
    {  
        m_layout->Release();  
        m_layout = 0;  
    }  
  
    // Release the pixel shader.  
    if(m_pixelShader)  
    {  
        m_pixelShader->Release();  
        m_pixelShader = 0;  
    }  
  
    // Release the vertex shader.  
    if(m_vertexShader)  
    {  
        m_vertexShader->Release();  
        m_vertexShader = 0;  
    }  
  
	m_matrixBuffer=0;

    return;  
}

void RZShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd,const CHAR* shaderFilename)  
{  
    char* compileErrors;  
    unsigned long bufferSize, i;  
    ofstream fout;  
  
  
    // Get a pointer to the error message text buffer.  
    compileErrors = (char*)(errorMessage->GetBufferPointer());  
  
    // Get the length of the message.  
    bufferSize = errorMessage->GetBufferSize();  
  
    // Open a file to write the error message to.  
    fout.open("shader-error.txt");  
  
    // Write out the error message.  
    for(i=0; i<bufferSize; i++)  
    {  
        fout << compileErrors[i];  
    }  
  
    // Close the file.  
    fout.close();  
  
    // Release the error message.  
    errorMessage->Release();  
    errorMessage = 0;  
  
    // Pop a message up on the screen to notify the user to check the text file for compile errors.  
    MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);  
  
    return;  
}

bool RZShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix,const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)  
{  
    HRESULT result;  
    D3D11_MAPPED_SUBRESOURCE mappedResource;  
    MatrixBufferType* dataPtr;  
    unsigned int bufferNumber;  
  
    // Lock the constant buffer so it can be written to.  
    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);  
    if(FAILED(result))  
    {  
        return false;  
    }  
  
    // Get a pointer to the data in the constant buffer.  
    dataPtr = (MatrixBufferType*)mappedResource.pData;  
  
    // Copy the matrices into the constant buffer. Transpose the matrices to prepare them for the shader.
    dataPtr->world = XMMatrixTranspose(worldMatrix);  
    dataPtr->view = XMMatrixTranspose(viewMatrix);  
    dataPtr->projection = XMMatrixTranspose(projectionMatrix);  
  
    // Unlock the constant buffer.  
    deviceContext->Unmap(m_matrixBuffer, 0);  
  
    // Set the position of the constant buffer in the vertex shader.  
    bufferNumber = 0;  
  
    // Finanly set the constant buffer in the vertex shader with the updated values.  
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
  
    return true;  
}

void RZShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, int vertexStart)  
{  
    // Set the vertex input layout.  
    deviceContext->IASetInputLayout(m_layout);  
  
    // Set the vertex and pixel shaders that will be used to render this triangle.  
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);  
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);  
  
    // Render the triangle.  
    deviceContext->DrawIndexed(indexCount, indexStart, 0);  
  
    return;  
}