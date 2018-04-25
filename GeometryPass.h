#ifndef GEOMETRYPASS_H
#define GEOMETRYPASS_H
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Camera.h"
#include "ObjLoader.h"
#include <vector>
const int NR_OF_RESOURCE_VIEWS = 4;

//ConstantBuffer
struct GS_CONSTANT_BUFFER
{
	DirectX::XMMATRIX theWorld;
	DirectX::XMMATRIX theWorldViewProj;
};

class GeometryPass
{
private:

	DirectX::XMMATRIX scaling;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewSpace;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX worldViewProj;

	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDeviceContext;
	ID3D11RenderTargetView* gRenderTargetView;
	ID3D11DepthStencilView* gDepthStencilView;
	ID3D11VertexShader* gVertexShader;
	ID3D11GeometryShader* gGeometryShader;
	ID3D11PixelShader* gPixelShader;
	ID3D11Buffer* gVertexBuffer;
	ID3D11InputLayout* gVertexLayout;
	ID3D11Buffer* gConstantBuffer;
	
	Camera* camera;
	float dt;
	
	void createMatrices();	
	GS_CONSTANT_BUFFER GsConstData;
	
	HRESULT createConstantBuffer();
	HRESULT createShaders();
	HRESULT createGBuffer();
	void createTriangleData();

public:
	GeometryPass(ID3D11Device* inGDevice, ID3D11DeviceContext* inGDeviceContext,
		ID3D11RenderTargetView *inGRTV, ID3D11DepthStencilView *inGDSV,
	  float inDt, Camera *camera);	
	~GeometryPass();
	void render();	
};


#endif
