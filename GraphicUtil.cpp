#include "DXUT.h"

#include "SpriteFont.h"
#include "GraphicUtil.h"

class trial
{
public:
	static DirectX::SpriteFont* font;
	trial(ID3D11Device* device, WCHAR str[250])
	{
		font = new DirectX::SpriteFont(device, str);
	}
};

ID3D11Device* GraphicUtil::device;
ID3D11DeviceContext* GraphicUtil::deviceContext;
IDXGISwapChain* GraphicUtil::swapChain;
ID3D11RenderTargetView* GraphicUtil::renderTargetView;
ID3D11Buffer* GraphicUtil::vertexBuffer;
ID3D11Buffer* GraphicUtil::vertexBackBuffer;

ID3D11PixelShader* GraphicUtil::pixelShader;
ID3D11VertexShader* GraphicUtil::vertexShader;

ID3D11PixelShader* GraphicUtil::pixelShaderWhite;
ID3D11VertexShader* GraphicUtil::vertexShaderColor;
ID3D11PixelShader* GraphicUtil::pixelShaderColor;

ID3D11InputLayout* GraphicUtil::colorLayout;
ID3D11InputLayout* GraphicUtil::defaultLayout;

DirectX::SpriteBatch* font;




DirectX::XMFLOAT4 GraphicUtil::changePositiveColor = DirectX::XMFLOAT4(3, 0, 0, 0);
DirectX::XMFLOAT4 GraphicUtil::changeNegativeColor = DirectX::XMFLOAT4(0, 3.0, 0, 0);
DirectX::XMFLOAT4 GraphicUtil::changeRatePositiveColor = DirectX::XMFLOAT4(0.4, 0.8, 0, 0);
DirectX::XMFLOAT4 GraphicUtil::changeNegativeRatePositiveColor = DirectX::XMFLOAT4(0.2, -0.2, 0, 0);
DirectX::XMFLOAT4 GraphicUtil::changeRateNegativeColor = DirectX::XMFLOAT4(0, 0.5, 8, 0);

void GraphicUtil::initPtr(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, IDXGISwapChain* _swapChain, ID3D11RenderTargetView* _renderTargetView, ID3D11Buffer* _vertexBuffer, ID3D11PixelShader* _pixelShader, ID3D11VertexShader* _vertexShader)
{
	device = _device;
	deviceContext = _deviceContext;
	swapChain = _swapChain;
	renderTargetView = _renderTargetView;
	vertexBuffer = _vertexBuffer;
	pixelShader = _pixelShader;
	vertexShader = _vertexShader;

	//font = new DirectX::SpriteFont(device, L"assets\\italic.spritefont");

	ID3DBlob* pVSColorBlob = nullptr;
	CompileShaderFromFile(L"Tutorial03.fx", "VS_COLOR", "vs_4_0", &pVSColorBlob);
	device->CreateVertexShader(pVSColorBlob->GetBufferPointer(), pVSColorBlob->GetBufferSize(), nullptr, &vertexShaderColor);

	// Define the input layout
	deviceContext->IAGetInputLayout(&defaultLayout);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC layoutDefault[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	UINT numElementsDefault = ARRAYSIZE(layoutDefault);

	// Create the input layout
	device->CreateInputLayout(layout, numElements, pVSColorBlob->GetBufferPointer(),
		pVSColorBlob->GetBufferSize(), &colorLayout);

	device->CreateInputLayout(layout, numElements, pVSColorBlob->GetBufferPointer(),
		pVSColorBlob->GetBufferSize(), &defaultLayout);

	ID3DBlob* pPSColorBlob = nullptr;
	CompileShaderFromFile(L"Tutorial03.fx", "PS_COLOR", "ps_4_0", &pPSColorBlob);
	device->CreatePixelShader(pPSColorBlob->GetBufferPointer(), pPSColorBlob->GetBufferSize(), nullptr, &pixelShaderColor);


	ID3DBlob* pPSWhiteBlob = nullptr;
	CompileShaderFromFile(L"Tutorial03.fx", "PS_WHITE", "ps_4_0", &pPSWhiteBlob);
	device->CreatePixelShader(pPSWhiteBlob->GetBufferPointer(), pPSWhiteBlob->GetBufferSize(), nullptr, &pixelShaderWhite);
}

HRESULT GraphicUtil::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void GraphicUtil::swichPaint(BrushType type)
{
	switch (type)
	{
	case YELLOW_PAINT:
		//deviceContext->IASetInputLayout(defaultLayout);
		deviceContext->VSSetShader(vertexShader, nullptr, 0);
		deviceContext->PSSetShader(pixelShader, nullptr, 0);
		break;
	case WHITE_PAINT:
		//deviceContext->IASetInputLayout(defaultLayout);
		deviceContext->VSSetShader(vertexShader, nullptr, 0);
		deviceContext->PSSetShader(pixelShaderWhite, nullptr, 0);
		break;
	case DYNAMIC_PAINT:
		deviceContext->IASetInputLayout(colorLayout);
		deviceContext->VSSetShader(vertexShaderColor, nullptr, 0);
		deviceContext->PSSetShader(pixelShaderColor, nullptr, 0);
	}
}

void GraphicUtil::clear()
{
	deviceContext->ClearRenderTargetView(renderTargetView, DirectX::Colors::Black);
}