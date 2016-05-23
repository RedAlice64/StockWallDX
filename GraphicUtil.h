#include "DXUT.h"
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include "DirectXMath.h"
#include <directxcolors.h>

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"


enum BrushType
{
	YELLOW_PAINT = 0,
	WHITE_PAINT = 1,
	RED_PAINT = 2,
	DYNAMIC_PAINT = 3
};


class GraphicUtil
{
public:
	static ID3D11Device* device;
	static ID3D11DeviceContext* deviceContext;
	static IDXGISwapChain* swapChain;
	static ID3D11RenderTargetView* renderTargetView;
	static ID3D11Buffer* vertexBuffer;
	static ID3D11Buffer* vertexBackBuffer;

	static ID3D11PixelShader* pixelShader;
	static ID3D11VertexShader* vertexShader;

	static ID3D11PixelShader* pixelShaderWhite;
	static ID3D11VertexShader* vertexShaderColor;
	static ID3D11PixelShader* pixelShaderColor;
	static ID3D11InputLayout* colorLayout;
	static ID3D11InputLayout* defaultLayout;

	static DirectX::XMFLOAT4 changePositiveColor;
	static DirectX::XMFLOAT4 changeNegativeColor;
	static DirectX::XMFLOAT4 changeRatePositiveColor;
	static DirectX::XMFLOAT4 changeNegativeRatePositiveColor;
	static DirectX::XMFLOAT4 changeRateNegativeColor;

	static DirectX::SpriteFont* font;

	static void clear();

	void initPtr(ID3D11Device*, ID3D11DeviceContext*, IDXGISwapChain*, ID3D11RenderTargetView*, ID3D11Buffer*, ID3D11PixelShader*, ID3D11VertexShader*);
	static void swichPaint(BrushType);

private:
	
	HRESULT GraphicUtil::CompileShaderFromFile(WCHAR*, LPCSTR, LPCSTR, ID3DBlob**);
};



typedef struct colorVertice
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};