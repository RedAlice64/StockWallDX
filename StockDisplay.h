#include "Stocks.h"
#include "TDFAPI.h"
#include "string"
#include "map"
#include <d3d11_1.h>
#include <d3dcompiler.h>



class StockDisplay{
protected:
	
	static std::vector<TDF_INDEX_DATA> index;
	static int updated;
	static int poolCount;
	

	static void systemMsgHandler(THANDLE, TDF_MSG*);
	static void dataMsgHandler(THANDLE, TDF_MSG*);
	HRESULT StockDisplay::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

public:
	virtual void render();
	virtual std::vector<DirectX::XMFLOAT3> vertices()=0;
	void updateStart();
	void initPtr(ID3D11Device*, ID3D11DeviceContext*, IDXGISwapChain*, ID3D11RenderTargetView*, ID3D11Buffer*,ID3D11PixelShader*,ID3D11VertexShader*);
	int isUpdated();
	static std::map<std::string, SingleStock> stocks;

	static ID3D11Device* device;
	static ID3D11DeviceContext* deviceContext;
	static IDXGISwapChain* swapChain;
	static ID3D11RenderTargetView* renderTargetView;
	static ID3D11Buffer* vertexBuffer;
	static ID3D11PixelShader* pixelShader;
	static ID3D11VertexShader* vertexShader;

	static ID3D11PixelShader* pixelShaderWhite;
	static ID3D11VertexShader* vertexShaderColor;
	static ID3D11PixelShader* pixelShaderColor;
};

class StockPools :public StockDisplay{

public:
	void render();
	std::vector<DirectX::XMFLOAT3>vertices(){ return stocks[0].vertices; }
};

class Plates :public StockDisplay{
public:
	void render();
};

