#ifndef _D3DINIT_H_
#define _D3DINIT_H_

#include"../interfaces/IRenderManager.h"
#include<array>

struct VertexPosColor
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

class D3DInit : public IRenderManager
{
public:
	D3DInit(HWND* hWnd, unsigned int width, unsigned int height); //initializes COMs to nullptr
	virtual ~D3DInit();

	virtual bool VInit() override; //abstraction for API specific initiation.
	virtual void VStartRendering();
	virtual void VEndRendering();
	virtual void VShutDown() override; //Uses SafeRelease to release COMs

protected:
	//CreateDevice(), CreateStates(), SetViewport()
	bool InitD3D();
	//creates device, device context and swap chain
	bool CreateDevice();
	//creates backbuffer(renderTargetView), depth/stencil view and sets depth/stencil state and rasterizer state
	bool CreateStates();
	void SetViewport();

	void RenderBox();
	bool CreateShaders();

	void Update();

protected:
	//Used in CreateDevice()
	ID3D11Device* m_pD3DDevice; //released
	ID3D11DeviceContext* m_pD3DDeviceContext; //released
	IDXGISwapChain* m_pD3DSwapChain; //released

	//Used in CreateStates()
	ID3D11RenderTargetView* m_pD3DRenderTargetView; //released
	ID3D11DepthStencilView* m_pD3DDepthStencilView; //released
	ID3D11Texture2D* m_pD3DDepthStencilBuffer; //released
	ID3D11DepthStencilState* m_pD3DDepthStencilState; //released
	ID3D11RasterizerState* m_pD3DRasterizerState; //released

	//Used in SetViewport()
	D3D11_VIEWPORT m_viewport;

	ID3D11Buffer* m_pD3DVertexBuffer;
	ID3D11Buffer* m_pD3DIndexBuffer;

	ID3D11InputLayout* m_pD3DInputLayout;

	ID3D11VertexShader* m_pD3DVertexShader;
	ID3D11PixelShader* m_pD3DPixelShader;

	enum ConstantBuffer
	{
		CB_Application,
		CB_Frame,
		CB_Object,
		NumCB
	};

	ID3D11Buffer* m_pConstantBuffers[NumCB];

	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projMatrix;

	static const VertexPosColor m_vertices[8];

	static const WORD m_indicies[36];

private:
	HWND* m_pHWnd;
	unsigned int m_width;
	unsigned int m_height;
};

#endif