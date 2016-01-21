#include "RZCurve.h";

using namespace RZ;

RZCurveKnot::RZCurveKnot()
{
}

RZCurveKnot::RZCurveKnot(float px, float py, float pz, float cx, float cy, float cz, float u, float v)
{
	position.x = px;
	position.y = py;
	position.z = pz;
	color.x = cx;
	color.y = cy;
	color.z = cz;
	uv.x = u;
	uv.y = v;
}

RZCurveKnot::RZCurveKnot(const RZCurveKnot& other)
{
	position.x = other.position.x;
	position.y = other.position.y;
	position.z = other.position.z;
	color.x = other.color.x;
	color.y = other.color.y;
	color.z = other.color.z;
	uv.x = other.uv.x;
	uv.y = other.uv.y;
}

RZCurveKnot& RZCurveKnot::operator = (const RZCurveKnot& other)
{
	return RZCurveKnot(other);
}


RZCurveKnot::~RZCurveKnot()
{
}





RZCurve::RZCurve()
{
}

RZCurve::RZCurve(const RZCurve& other)
{
}

RZCurve::~RZCurve()
{
}

bool RZCurve::Initialize(ID3D11Device* pd3dDevice, HWND hwnd)
{
	HRESULT result;

	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* geometryShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;


	// Initialize the pointers this function will use to null.  
	errorMessage = 0;
	vertexShaderBuffer = 0;
	geometryShaderBuffer = 0;
	pixelShaderBuffer = 0;

	m_device = pd3dDevice;


	result = D3DX11CompileFromFile(lineVS, NULL, NULL, "LineVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, lineVS);
		}
		else
		{
			MessageBox(hwnd, lineVS, "Missing Shader File", MB_OK);
		}

		return false;
	}

	result = pd3dDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_lineVS);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CompileFromFile(lineGS, NULL, NULL, "LineGS", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &geometryShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, lineGS);
		}
		else
		{
			MessageBox(hwnd, lineGS, "Missing Shader File", MB_OK);
		}

		return false;
	}

	result = pd3dDevice->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &m_lineGS);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CompileFromFile(linePS, NULL, NULL, "LinePS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, linePS);
		}
		else
		{
			MessageBox(hwnd, linePS, "Missing Shader File", MB_OK);
		}

		return false;
	}

	result = pd3dDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_linePS);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.  
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(CurveCBT);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = pd3dDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	UINT numElements;
	// Now setup the layout of the data that goes into the shader.  
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.  
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "UV";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.  
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.  
	result = pd3dDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	//---------------------Tessellation---------------------------

	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;

	hullShaderBuffer = 0;
	domainShaderBuffer = 0;


	//Create Hull Shader 
	result = D3DX11CompileFromFile(lineHS, NULL, NULL, "LineHS", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &hullShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, lineHS);
		}
		else
		{
			MessageBox(hwnd, lineHS, "Missing Shader File", MB_OK);
		}

		return false;
	}

	result = pd3dDevice->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &m_lineHS);
	if (FAILED(result))
	{
		return false;
	}


	//Create Domain Shader
	result = D3DX11CompileFromFile(lineDS, NULL, NULL, "LineDS", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &domainShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, lineHS);
		}
		else
		{
			MessageBox(hwnd, lineDS, "Missing Shader File", MB_OK);
		}

		return false;
	}

	result = pd3dDevice->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &m_lineDS);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


bool RZCurve::CreateLine(UINT length, RZCurveKnot *data)
{
	m_length = length;
	for (unsigned i = 0; i != m_length;++i)
	{
		m_data.push_back(data[i]);
	}
	if (!CreateVertexBuffer())
	{
		return false;
	}

	return true;
}

void RZCurve::Release()
{
	m_data.clear();

	RZRELEASE(m_vertexBuffer);
	RZRELEASE(m_matrixBuffer);
	RZRELEASE(m_lineVS);
	RZRELEASE(m_lineGS);
	RZRELEASE(m_linePS);
	RZRELEASE(m_lineDS);
	RZRELEASE(m_lineHS);
	RZRELEASE(m_layout);
}

bool RZCurve::CreateVertexBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(RZCurveKnot) * m_length;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_data.data();

	// Do the creation of the actual vertex buffer
	hr = m_device->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool RZCurve::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CurveCBT* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.  
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.  
	dataPtr = (CurveCBT*)mappedResource.pData;

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
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

void RZCurve::RenderLine(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	//pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	UINT stride, offset;
	stride = sizeof(RZCurveKnot);
	offset = 0;
	pd3dDeviceContext->IASetInputLayout(m_layout);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	pd3dDeviceContext->VSSetShader(m_lineVS, NULL, 0);
	pd3dDeviceContext->GSSetShader(m_lineGS, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_linePS, NULL, 0);

	// Render the triangle.  

	SetShaderParameters(pd3dDeviceContext, worldMatrix, viewMatrix, projectionMatrix);
	pd3dDeviceContext->Draw(m_length, 0);


	pd3dDeviceContext->GSSetShader(0, NULL, 0);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
}


void RZCurve::RenderTessellatedLine(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	//pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST);

	UINT stride, offset;
	stride = sizeof(RZCurveKnot);
	offset = 0;
	pd3dDeviceContext->IASetInputLayout(m_layout);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	pd3dDeviceContext->VSSetShader(m_lineVS, NULL, 0);
	pd3dDeviceContext->HSSetShader(m_lineHS, NULL, 0);
	pd3dDeviceContext->DSSetShader(m_lineDS, NULL, 0);
	pd3dDeviceContext->GSSetShader(m_lineGS, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_linePS, NULL, 0);

	// Render the triangle.  

	SetShaderParameters(pd3dDeviceContext, worldMatrix, viewMatrix, projectionMatrix);
	pd3dDeviceContext->Draw(m_length, 0);


	pd3dDeviceContext->GSSetShader(0, NULL, 0);
	pd3dDeviceContext->HSSetShader(0, NULL, 0);
	pd3dDeviceContext->DSSetShader(0, NULL, 0);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void RZCurve::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const CHAR* shaderFilename)
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
	for (i = 0; i<bufferSize; i++)
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