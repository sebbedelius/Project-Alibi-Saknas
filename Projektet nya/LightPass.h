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

struct PSL_CAMERA_BUFFER
{
	DirectX::XMVECTOR camPos;
};

class LightPass
{
private:	

	VSL_CONSTANT_BUFFER VSLConstData;
	PSL_LIGHT_BUFFER PSLConstData;
	PSL_CAMERA_BUFFER PSLConstCamData;
	ID3D11Device * gDevice;
	ID3D11DeviceContext* gDeviceContext;
	ID3D11RenderTargetView* gRenderTargetView;
	ID3D11DepthStencilView* gDepthStencilView;
	ID3D11VertexShader* gVertexShader;
	ID3D11PixelShader* gPixelShader;
	ID3D11Buffer* gVertexBuffer;	
	ID3D11Buffer* gVSLConstantBuffer;
	ID3D11Buffer* gPSLConstantBuffer;
	ID3D11InputLayout* gVertexLayout;
	ID3D11ShaderResourceView* gShaderResourceView[NR_OF_RESOURCE_VIEWS];	

	Camera* camera;	

	void createMatrices();
	void createLights();
	HRESULT createConstantBuffer();
	HRESULT createShaders();
	void createTriangleData();
	
public:
	LightPass(ID3D11Device* inGDevice, ID3D11DeviceContext* inGDeviceContext, 
		ID3D11RenderTargetView* inGRTV, ID3D11DepthStencilView* inGDSV,
		ID3D11ShaderResourceView* inGSRV[], Camera* inCamera);
	~LightPass();
	
	void update();
	void render();
};


#endif
