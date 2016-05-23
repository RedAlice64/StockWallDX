#include "StockDisplay.h"
#include <directxcolors.h>



HRESULT StockDisplay::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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


std::map<std::string, SingleStock> StockDisplay::stocks;
std::vector<TDF_INDEX_DATA> StockDisplay::index;
int StockDisplay::updated;
int StockDisplay::poolCount;

ID3D11Device* StockDisplay::device;
ID3D11DeviceContext* StockDisplay::deviceContext;
IDXGISwapChain* StockDisplay::swapChain;
ID3D11RenderTargetView* StockDisplay::renderTargetView;
ID3D11Buffer* StockDisplay::vertexBuffer;
ID3D11PixelShader* StockDisplay::pixelShader;
ID3D11VertexShader* StockDisplay::vertexShader;

ID3D11PixelShader* StockDisplay::pixelShaderWhite;
ID3D11VertexShader* StockDisplay::vertexShaderColor;
ID3D11PixelShader* StockDisplay::pixelShaderColor;

void StockDisplay::systemMsgHandler(THANDLE hTdf, TDF_MSG* pMsgHead){}
void StockDisplay::dataMsgHandler(THANDLE hTdf, TDF_MSG* pMsgHead){
	TDF_MARKET_DATA marketData = *(TDF_MARKET_DATA*)pMsgHead->pData;
	TDF_INDEX_DATA indexData = *(TDF_INDEX_DATA*)pMsgHead->pData;

	switch (pMsgHead->nDataType){

	case MSG_DATA_FUTURE:
		break;
	case MSG_DATA_MARKET:


		if (marketData.nMatch == 0)return;
		if ((stocks.find(marketData.szCode)) != stocks.end())
			stocks[marketData.szCode].addFrame(marketData);
		else 
		{
			stocks[marketData.szCode] = *(new SingleStock(marketData));
			int sx=poolCount%5, sy=5-poolCount/5;
			stocks[marketData.szCode].setCoord(sx*0.4 - 1.0,sy*0.4 -1.2);
			poolCount++;
		}
		break;
	case MSG_DATA_INDEX:
		index.push_back(indexData);
		
		break;
	
	}
	updated++;
}
void StockDisplay::updateStart(){
	updated = 0;
	poolCount = 0;

		THANDLE tHandle;
		TDF_ERR tErr;
		TDF_OPEN_SETTING tSetting = {
			"114.80.154.34",
			"6221",
			"TD3432099003",
			"48295524",
			dataMsgHandler,
			systemMsgHandler,
			"",
			"601601.SH;601988.SH;601766.SH;601398.SH;600103.SH;600979.SH;600496.SH;600871.SH;600006.SH;600649.SH;600130.SH;600493.SH;600743.SH;600328.SH;601718.SH;600329.SH;600737.SH;600961.SH;600076.SH;600773.SH;600749.SH;600262.SH;600162.SH;600639.SH;600133",
			0x1,
			0xffffffff,
			0
		};
		tHandle = TDF_Open(&tSetting, &tErr);
	}
void StockDisplay::render(){
}

int StockDisplay::isUpdated()
{
	return updated;
}

void StockPools::render()
{
	GraphicUtil::clear();
	//deviceContext->VSSetShader(vertexShader, nullptr, 0);
	//deviceContext->PSSetShader(pixelShader, nullptr, 0);
	for (auto &i : stocks)
	{
		i.second.render();
	}
	GraphicUtil::swapChain->Present(0, 0);
}

void Plates::render(){
}

void StockDisplay::initPtr(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, IDXGISwapChain* _swapChain, ID3D11RenderTargetView* _renderTargetView, ID3D11Buffer* _vertexBuffer,ID3D11PixelShader* _pixelShader,ID3D11VertexShader* _vertexShader)
{
	device = _device;
	deviceContext = _deviceContext;
	swapChain = _swapChain;
	renderTargetView = _renderTargetView;
	vertexBuffer = _vertexBuffer;
	pixelShader = _pixelShader;
	vertexShader = _vertexShader;

	ID3DBlob* pVSColorBlob = nullptr;
	CompileShaderFromFile(L"Tutorial03.fx", "VS_COLOR", "vs_4_0", &pVSColorBlob);
	device->CreateVertexShader(pVSColorBlob->GetBufferPointer(), pVSColorBlob->GetBufferSize(), nullptr,&vertexShaderColor);

	ID3DBlob* pPSColorBlob = nullptr;
	CompileShaderFromFile(L"Tutorial03.fx","PS_COLOR", "ps_4_0", &pPSColorBlob);
	device->CreatePixelShader(pPSColorBlob->GetBufferPointer(), pPSColorBlob->GetBufferSize(), nullptr, &pixelShaderColor);

	ID3DBlob* pPSWhiteBlob = nullptr;
	CompileShaderFromFile(L"Tutorial03.fx", "PS_WHITE", "ps_4_0", &pPSWhiteBlob);
	device->CreatePixelShader(pPSWhiteBlob->GetBufferPointer(), pPSWhiteBlob->GetBufferSize(), nullptr, &pixelShaderWhite);
}



