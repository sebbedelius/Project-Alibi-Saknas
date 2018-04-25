#include "GeometryPass.h"

void GeometryPass::createMatrices()
{

	this->scaling = DirectX::XMMatrixScaling(1, 1, 1);
	this->world = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationY(0), scaling);
	this->viewSpace = DirectX::XMMatrixLookAtLH(this->camera->getCamPos, this->camera->getLookAt, this->camera->getUp);
	this->projection = DirectX::XMMatrixPerspectiveFovLH(this->camera->getFov, this->camera->getAspectRatio, 
		this->camera->getNearPlane, this->camera->getFarPlane);
	DirectX::XMMATRIX tempWorldViewProj = DirectX::XMMatrixMultiply(world, viewSpace);
	this->worldViewProj = DirectX::XMMatrixMultiplyTranspose(tempWorldViewProj, projection);

}

HRESULT GeometryPass::createConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(GS_CONSTANT_BUFFER);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &GsConstData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = 0;
	hr = this->gDevice->CreateBuffer(&cbDesc, &data,
		&this->gConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT GeometryPass::createShaders()
{
	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	HRESULT result = D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	this->gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&this->gVertexShader
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"COLOR", //Ändra namn till TEXCOORD är väl rimligt (glöm inte ändra i shaders)
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT, //Ta bort B32 när vi ska ha texturer för 2 floats istället för 3 (u, v)
			0,
			12,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->gVertexLayout);

	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create geometry shader
	ID3DBlob* pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"GeometryShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,					// effect compile options
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}
	this->gDevice->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&this->gGeometryShader
	);

	pGS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"Fragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	this->gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->gPixelShader);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	return S_OK;
}

HRESULT GeometryPass::createGBuffer()
{
	
}

void GeometryPass::createTriangleData()
{
	struct TriangleVertex
	{
		float x, y, z;
		float r, g, b;
		//float u, v; istället för rgb när det är tex. 
	};

	ObjLoader* tableObj;
	tableObj = new ObjLoader();
	bool loaded = false;

	loaded = tableObj->loadObj("table.obj");

	int sizeOfTriangleVertex = tableObj->getNrOfIndices();
	TriangleVertex * triangleVertices = new TriangleVertex[sizeOfTriangleVertex];
	Vertex * vertex;
	vertex = new Vertex[tableObj->getNrOfIndices()];
	//Texture* texture = new Texture[tableObj->getNrOfIndices()];
	tableObj->getVertices(vertex);
	//tableObj->getTextureCoords(texture);

	for (int i = 0; i < sizeOfTriangleVertex; i++)
	{
		triangleVertices[i].x = vertex[i].pos[0];
		triangleVertices[i].y = vertex[i].pos[1];
		triangleVertices[i].z = vertex[i].pos[2];
		triangleVertices[i].r = 1;
		triangleVertices[i].g = 0;
		triangleVertices[i].b = 0;
		//med texturer blir det
		//triangleVertices[i].u = texture[i].u; och samma för v, istället för rgb.
	}

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(TriangleVertex) * sizeOfTriangleVertex;

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;

	// create a Vertex Buffer
	this->gDevice->CreateBuffer(&bufferDesc, &data, &this->gVertexBuffer);

	delete vertex;
	delete tableObj;
	delete[] triangleVertices;	
}

GeometryPass::GeometryPass(ID3D11Device* inGDevice, ID3D11DeviceContext* inGDeviceContext,
	ID3D11RenderTargetView *inGRTV, ID3D11DepthStencilView *inGDSV, 
	float inDt, Camera *camera)
{
	this->gDevice = inGDevice;
	this->gDeviceContext = inGDeviceContext;
	this->gRenderTargetView = inGRTV;
	this->gDepthStencilView = inGDSV;
	this->camera = camera;
	this->GsConstData = { this->world, this->worldViewProj };	
	this->dt = inDt;
}

GeometryPass::~GeometryPass()
{
	this->gVertexShader->Release();
	this->gGeometryShader->Release();
	this->gPixelShader->Release();
	this->gVertexBuffer->Release();
	this->gVertexLayout->Release();
	this->gConstantBuffer->Release();
}

void GeometryPass::render()
{
	float controlSpeed;
	float constantSpeed = 0.05;	

	// specifying NULL or nullptr we are disabling that stage
	// in the pipeline
	this->gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	this->gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	UINT32 vertexSize = sizeof(float) * 6;
	UINT32 offset = 0;
	// specify which vertex buffer to use next.
	this->gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);

	D3D11_MAPPED_SUBRESOURCE dataPtr;
	this->gDeviceContext->Map(gConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	controlSpeed = this->dt / constantSpeed;

	DirectX::XMMATRIX rotateY = DirectX::XMMatrixRotationY(-0.05 * controlSpeed);

	GsConstData.theWorld = XMMatrixMultiply(GsConstData.theWorld, rotateY);
	GsConstData.theWorldViewProj = XMMatrixMultiply(GsConstData.theWorldViewProj, rotateY);
	// copy memory from CPU to GPU the entire struct	
	memcpy(dataPtr.pData, &GsConstData, sizeof(GS_CONSTANT_BUFFER));
	// UnMap constant buffer so that we can use it again in the GPU
	this->gDeviceContext->Unmap(gConstantBuffer, 0);

	//Set render targets (textures)
	this->gDeviceContext->OMSetRenderTargets(NR_OF_RESOURCE_VIEWS,
		&this->gRenderTargetView,
		this->gDepthStencilView);

	//Bind constant buffer to the geometry shader
	this->gDeviceContext->GSSetConstantBuffers(0, 1, &gConstantBuffer);

	// specify the topology to use when drawing
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// specify the IA Layout (how is data passed)
	this->gDeviceContext->IASetInputLayout(gVertexLayout);

	gDeviceContext->Draw(1000, 0);
}
