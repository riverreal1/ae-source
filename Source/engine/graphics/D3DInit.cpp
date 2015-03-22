#include"pch.h"
#include"D3DInit.h"
#include"../system/helper.h"
#include"../shaders/pixelShader.h"
#include"../shaders/vertexShader.h"

using namespace DirectX;

const VertexPosColor D3DInit::m_vertices[8] =
{
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) }, // 0
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 1
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f) }, // 2
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) }, // 3
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }, // 4
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f) }, // 5
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f) }, // 6
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f) } // 7
};

const WORD D3DInit::m_indicies[36] =
{
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	4, 5, 1, 4, 1, 0,
	3, 2, 6, 3, 6, 7,
	1, 5, 6, 1, 6, 2,
	4, 0, 3, 4, 3, 7
};

D3DInit::D3DInit(HWND* hWnd, unsigned int width, unsigned int height) :
m_pD3DDevice(nullptr),
m_pD3DDeviceContext(nullptr),
m_pD3DSwapChain(nullptr),

m_pD3DRenderTargetView(nullptr),
m_pD3DDepthStencilView(nullptr),
m_pD3DDepthStencilBuffer(nullptr),
m_pD3DDepthStencilState(nullptr),
m_pD3DRasterizerState(nullptr),

m_pHWnd(hWnd),
m_width(width),
m_height(height),

m_pD3DVertexBuffer(nullptr),
m_pD3DIndexBuffer(nullptr),
m_pD3DInputLayout(nullptr),
m_pD3DVertexShader(nullptr),
m_pD3DPixelShader(nullptr)
{
	

	
}

D3DInit::~D3DInit()
{

}

bool D3DInit::VInit()
{
	bool result = InitD3D();

	return result;
}

bool D3DInit::CreateDevice()
{
	HRESULT hr;

	//-----------------------------------------SWAPCHAIN DESC--------------------------------------------
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = *m_pHWnd;
	swapChainDesc.SampleDesc.Count = 1; //--------------------------------------MSAA
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;
	//-------------------------------------------------------------------------------------
	UINT createDeviceFlags = 0;
#if _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	D3D_FEATURE_LEVEL featureLevel;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &m_pD3DSwapChain, &m_pD3DDevice, &featureLevel,
		&m_pD3DDeviceContext);
	if (hr == E_INVALIDARG) //if the feature level is invalid, try to get the latest supported level
	{
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1, //<--------------( -1 )
			D3D11_SDK_VERSION, &swapChainDesc, &m_pD3DSwapChain, &m_pD3DDevice, &featureLevel,
			&m_pD3DDeviceContext);
	}

	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create d3d device"), TEXT("Error"), MB_OK);
		//ErrorMSG("Failed to create d3d device.");
		return false;

	}

	return true;
}

bool D3DInit::CreateStates()
{
	//------------------------------------backBuffer and renderTargetView-------------------------------------------------
	LRESULT hr;
	ID3D11Texture2D* backBuffer;
	hr = m_pD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create backbuffer"), TEXT("Error"), MB_OK);
		return false;
	}

	hr = m_pD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pD3DRenderTargetView);

	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create render target view"), TEXT("Error"), MB_OK);
		return false;
	}

	SafeRelease(backBuffer);
	//----------------------------------------depthStencilview---------------------------------------------
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = m_width;
	depthStencilBufferDesc.Height = m_height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_pD3DDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_pD3DDepthStencilBuffer);

	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create depth stencil buffer"), TEXT("Error"), MB_OK);
		return false;
	}

	hr = m_pD3DDevice->CreateDepthStencilView(m_pD3DDepthStencilBuffer, nullptr, &m_pD3DDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create depth stencil view"), TEXT("Error"), MB_OK);
		return false;
	}
	//----------------------------------------depthStencilState---------------------------------------------
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;

	hr = m_pD3DDevice->CreateDepthStencilState(&depthStencilStateDesc, &m_pD3DDepthStencilState);

	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create depth stencil state"), TEXT("Error"), MB_OK);
		return false;
	}
	//------------------------------------------rasterizerState-------------------------------------------
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	hr = m_pD3DDevice->CreateRasterizerState(&rasterizerDesc, &m_pD3DRasterizerState);

	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create rasterizer state"), TEXT("Error"), MB_OK);
		return false;
	}

	//------------------------------------------Binding views-------------------------------------------

	

	return true;
}

void D3DInit::SetViewport()
{
	m_viewport.Width = static_cast<float>(m_width);
	m_viewport.Height = static_cast<float>(m_height);
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	m_pD3DDeviceContext->RSSetViewports(1, &m_viewport);
}

bool D3DInit::InitD3D()
{
	bool result;
	result = CreateDevice();
	if (!result)
	{
		return false;
	}
	result = CreateStates();
	if (!result)
	{
		return false;
	}

	SetViewport();

	result = CreateShaders();
	if (!result)
	{
		return false;
	}

	

	return true;
}

void D3DInit::VShutDown()
{
	SafeRelease(m_pD3DPixelShader);
	SafeRelease(m_pD3DVertexShader);
	SafeRelease(m_pD3DInputLayout);
	SafeRelease(m_pD3DIndexBuffer);
	SafeRelease(m_pD3DVertexBuffer);

	SafeRelease(m_pD3DRasterizerState);
	SafeRelease(m_pD3DDepthStencilState);
	SafeRelease(m_pD3DDepthStencilBuffer);
	SafeRelease(m_pD3DDepthStencilView);
	SafeRelease(m_pD3DRenderTargetView);

	SafeRelease(m_pD3DSwapChain);
	SafeRelease(m_pD3DDeviceContext);
	SafeRelease(m_pD3DDevice);
}

void D3DInit::VStartRendering()
{
	assert(m_pD3DDeviceContext);
	assert(m_pD3DSwapChain);

	Update();

	const float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pD3DDeviceContext->ClearRenderTargetView(m_pD3DRenderTargetView, Colors::CornflowerBlue);
	m_pD3DDeviceContext->ClearDepthStencilView(m_pD3DDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	RenderBox();
}

void D3DInit::VEndRendering()
{
	m_pD3DSwapChain->Present(1, 0);
}

bool D3DInit::CreateShaders()
{
	assert(m_pD3DDevice);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosColor) * _countof(D3DInit::m_vertices);
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem = D3DInit::m_vertices;

	HRESULT hr = m_pD3DDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &m_pD3DVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create vertex buffer"), TEXT("Error"), MB_OK);
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(WORD) * _countof(m_indicies);
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	resourceData.pSysMem = m_indicies;

	hr = m_pD3DDevice->CreateBuffer(&indexBufferDesc, &resourceData, &m_pD3DIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create index buffer"), TEXT("Error"), MB_OK);
		return false;
	}

	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_pD3DDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffers[CB_Application]);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create constant buffer"), TEXT("Error"), MB_OK);
		return false;
	}

	hr = m_pD3DDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffers[CB_Frame]);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create constant buffer"), TEXT("Error"), MB_OK);
		return false;
	}
	hr = m_pD3DDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffers[CB_Object]);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create constant buffer"), TEXT("Error"), MB_OK);
		return false;
	}

	hr = m_pD3DDevice->CreateVertexShader(vs, sizeof(vs), nullptr, &m_pD3DVertexShader);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create vertex shader"), TEXT("Error"), MB_OK);
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, Color), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	hr = m_pD3DDevice->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vs, sizeof(vs), &m_pD3DInputLayout);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create input layout"), TEXT("Error"), MB_OK);
		return false;
	}

	hr = m_pD3DDevice->CreatePixelShader(ps, sizeof(ps), nullptr, &m_pD3DPixelShader);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT("Failed to create pixel shader"), TEXT("Error"), MB_OK);
		return false;
	}

	RECT clientRect;
	GetClientRect(*m_pHWnd, &clientRect);

	float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
	float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

	m_projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), clientWidth / clientHeight, 0.1f, 100.0f);

	m_pD3DDeviceContext->UpdateSubresource(m_pConstantBuffers[CB_Application], 0, nullptr, &m_projMatrix, 0, 0);

	return true;
}

void D3DInit::RenderBox()
{
	assert(m_pD3DDevice);
	assert(m_pD3DDeviceContext);

	const UINT vertexStride = sizeof(VertexPosColor);
	const UINT offset = 0;

	m_pD3DDeviceContext->IASetVertexBuffers(0, 1, &m_pD3DVertexBuffer, &vertexStride, &offset);
	m_pD3DDeviceContext->IASetInputLayout(m_pD3DInputLayout);
	m_pD3DDeviceContext->IASetIndexBuffer(m_pD3DIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pD3DDeviceContext->VSSetShader(m_pD3DVertexShader, nullptr, 0);
	m_pD3DDeviceContext->VSSetConstantBuffers(0, NumCB, m_pConstantBuffers);

	m_pD3DDeviceContext->RSSetState(m_pD3DRasterizerState);

	m_pD3DDeviceContext->PSSetShader(m_pD3DPixelShader, nullptr, 0);

	m_pD3DDeviceContext->OMSetRenderTargets(1, &m_pD3DRenderTargetView, m_pD3DDepthStencilView);
	m_pD3DDeviceContext->OMSetDepthStencilState(m_pD3DDepthStencilState, 1);

	m_pD3DDeviceContext->DrawIndexed(_countof(m_indicies), 0, 0);

}

void D3DInit::Update()
{
	XMVECTOR eyePos = XMVectorSet(0, 0, -10, 1);
	XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	XMVECTOR upDir = XMVectorSet(0, 1, 0, 0);
	m_viewMatrix = XMMatrixLookAtLH(eyePos, focusPoint, upDir);
	m_pD3DDeviceContext->UpdateSubresource(m_pConstantBuffers[CB_Frame], 0, nullptr, &m_viewMatrix, 0, 0);
	
	static float angle = 0.0f;
	angle += 1.5f;
	XMVECTOR rotationAxis = XMVectorSet(0, 1, 1, 0);

	m_worldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
	m_pD3DDeviceContext->UpdateSubresource(m_pConstantBuffers[CB_Object], 0, nullptr, &m_worldMatrix, 0, 0);
}