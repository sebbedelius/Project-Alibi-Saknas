#include "LightPass.h"

void LightPass::createMatrices()
{

	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(this->camera->getCamPos(), this->camera->getLookAt(), this->camera->getUp());
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(this->camera->getFov(), this->camera->getAspectRatio(),
		this->camera->getNearPlane(), this->camera->getFarPlane());

	this->VSLConstData = { view,
		proj };
}

void LightPass::createLights()
{
	DirectX::XMVECTOR lightPos = DirectX::XMVectorSet(2, 3, -3, 1);
	DirectX::XMVECTOR lightColor = DirectX::XMVectorSet(1, 0, 0, 1);
	DirectX::XMVECTOR lightDirection = DirectX::XMVectorSet(0, 0, 0, 1);
	DirectX::XMVECTOR spotlightAngles = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR lightRange = DirectX::XMVectorSet(5, 5, 5, 5);

	this->PSLConstData = { lightPos, lightColor, lightDirection, spotlightAngles, lightRange };
}

HRESULT LightPass::createConstantBuffer()
{
	HRESULT hr = 0;

	D3D11_BUFFER_DESC vslCbDesc;
	vslCbDesc.ByteWidth = sizeof(VSL_CONSTANT_BUFFER);
	vslCbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vslCbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vslCbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vslCbDesc.MiscFlags = 0;
	vslCbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vslData;
	vslData.pSysMem = &VSLConstData;
	vslData.SysMemPitch = 0;
	vslData.SysMemSlicePitch = 0;

	hr = this->gDevice->CreateBuffer(&vslCbDesc, &vslData,
		&this->gVSLConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_BUFFER_DESC lbDesc;
	lbDesc.ByteWidth = sizeof(PSL_LIGHT_BUFFER);
	lbDesc.Usage = D3D11_USAGE_DYNAMIC;
	lbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lbDesc.MiscFlags = 0;
	lbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA lbData;
	lbData.pSysMem = &PSLConstData;
	lbData.SysMemPitch = 0;
	lbData.SysMemSlicePitch = 0;

	hr = this->gDevice->CreateBuffer(&lbDesc, &lbData,
		&this->gPSLConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT LightPass::createShaders()
{
	ID3DBlob* pVSL = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"VSLight.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VSL_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVSL,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pVSL)
			pVSL->Release();
		return result;
	}

	this->gDevice->CreateVertexShader(
		pVSL->GetBufferPointer(),
		pVSL->GetBufferSize(),
		nullptr,
		&this->gVertexShader
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	D3D11_INPUT_ELEMENT_DESC inputDescLight[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	 result = this->gDevice->CreateInputLayout(inputDescLight, ARRAYSIZE(inputDescLight), pVSL->GetBufferPointer(), pVSL->GetBufferSize(), &this->gVertexLayout);
	 if (FAILED(result))
	 {
		 exit(-1);
	 }
	// we do not need anymore this COM object, so we release it.
	pVSL->Release();


	//create pixel light shader
	ID3DBlob* pPSL = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"PSLight.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PSL_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPSL,			// double pointer to ID3DBlob		
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
		if (pPSL)
			pPSL->Release();
		return result;
	}

	result = this->gDevice->CreatePixelShader(pPSL->GetBufferPointer(), pPSL->GetBufferSize(), nullptr, &this->gPixelShader);
	if (FAILED(result))
	{
		exit(-1);
	}
	// we do not need anymore this COM object, so we release it.
	pPSL->Release();

	return S_OK;
}

void LightPass::createTriangleData()
{
	struct TriangleVertexPos
	{
		float x, y, z, w;
		//float u, v; istället för rgb när det är tex. 
	};

	int screenQuadVertexSize = 6;
	TriangleVertexPos triangleScreenQuadVertices[6] =
	{
		-1.0f, 1.0f, 0.0f, 1.0f,	//v0 pos		

		1.0f, -1.0f, 0.0f, 1.0f,	//v1		

		-1.0f, -1.0f, 0.0f, 1.0f, //v2		

		1.0f, 1.0f, 0.0f, 1.0f, //v3		

		1.0f, -1.0f, 0.0f, 1.0f, //v4		

		-1.0f, 1.0f, 0.0f, 1.0f //v5		
	};

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC lightBufferDesc;
	memset(&lightBufferDesc, 0, sizeof(lightBufferDesc));
	// what type of buffer will this be?
	lightBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	lightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	lightBufferDesc.ByteWidth = sizeof(TriangleVertexPos) * screenQuadVertexSize;
	lightBufferDesc.CPUAccessFlags = 0;
	lightBufferDesc.MiscFlags = 0;

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA lightData;
	lightData.pSysMem = triangleScreenQuadVertices;
	lightData.SysMemPitch = 0;
	lightData.SysMemSlicePitch = 0;

	// create a Vertex Buffer
	this->gDevice->CreateBuffer(&lightBufferDesc, &lightData, &this->gVertexBuffer);		
}

LightPass::LightPass(ID3D11Device* inGDevice, ID3D11DeviceContext* inGDeviceContext,
	ID3D11RenderTargetView* inGRTV, ID3D11DepthStencilView* inGDSV,
	ID3D11ShaderResourceView* inGSRV[], Camera* inCamera)
{
	this->gDevice = inGDevice;
	this->gDeviceContext = inGDeviceContext;
	this->gRenderTargetView = inGRTV;
	this->gDepthStencilView = inGDSV;	
	
	for (int i = 0; i < NR_OF_RESOURCE_VIEWS; i++)
	{
		this->gShaderResourceView[i] = inGSRV[i];
	}
	this->camera = inCamera;	
	
	this->createMatrices();
	this->createLights();
	this->createConstantBuffer();
	this->createShaders();
	this->createTriangleData();
}

LightPass::~LightPass()
{
	this->gVertexShader->Release();
	this->gPixelShader->Release();
	this->gVertexBuffer->Release();
	this->gVSLConstantBuffer->Release();
	this->gPSLConstantBuffer->Release();
	this->gVertexLayout->Release();
}

void LightPass::render()
{

	this->gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->gPixelShader, nullptr, 0);

	UINT32 vertexSizeLight = sizeof(float) * 4;
	UINT32 offsetLight = 0;	
	
	// specify which vertex buffer to use next.
	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBuffer, &vertexSizeLight, &offsetLight);
	
	this->gDeviceContext->OMSetRenderTargets(1,
		&this->gRenderTargetView,
		this->gDepthStencilView);

	//Bind constant buffer to the vertex shader
	this->gDeviceContext->VSSetConstantBuffers(0, 1, &this->gVSLConstantBuffer);
	//Bind constant buffer to the light pixel shader
	this->gDeviceContext->PSSetConstantBuffers(0, 1, &this->gPSLConstantBuffer);
	//Bind textures to light pixel shader
	this->gDeviceContext->PSSetShaderResources(0, NR_OF_RESOURCE_VIEWS,
		this->gShaderResourceView);

	this->gDeviceContext->IASetInputLayout(this->gVertexLayout);
	//issue a draw call of 6 vertices (similar to OpenGL)
	this->gDeviceContext->Draw(6, 0);	
}
