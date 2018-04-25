#ifndef LIGHT_PASS
#define LIGHT_PASS
#include<d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Camera.h"
const int NR_OF_RESOURCE_VIEWS = 4;

struct VSL_CONSTANT_BUFFER
{
	DirectX::XMMATRIX theView;
	DirectX::XMMATRIX theProj;
}; 

struct PSL_LIGHT_BUFFER
{
	DirectX::XMVECTOR theLightPos;
	DirectX::XMVECTOR theLightColor;
	DirectX::XMVECTOR theLightDirection;
	DirectX::XMVECTOR theSpotlightAngles;
	DirectX::XMVECTOR theLightRange;
};


class LightPass
{
private:	

	VSL_CONSTANT_BUFFER VSLConstData;
	PSL_LIGHT_BUFFER PSLConstData;
	ID3D11Device * gDevice;
	ID3D11DeviceContext* gDeviceContext;
	ID3D11RenderTargetView* gRenderTargetView;
	ID3D11DepthStencilView* gDepthStencilView;
	ID3D11VertexShader* gVertexShader;
	ID3D11PixelShader* gPixelShader;
	ID3D11Buffer* gVertexBuffer;
	ID3D11Buffer* gLightBuffer;
	ID3D11Buffer* gVSLConstantBuffer;
	ID3D11Buffer* gPSLConstantBuffer;
	ID3D11InputLayout* gVertexLayout;
	ID3D11ShaderResourceView* gShaderResourceView;
	ID3D11RenderTargetView* gRenderTargetView;

	Camera* camera;

	void createMatrices();
	void createLights();
	HRESULT createConstantBuffer();
	HRESULT createShaders();
	void createTriangleData();
	
public:
	LightPass();
	~LightPass();
	
	void render();
};


#endif
