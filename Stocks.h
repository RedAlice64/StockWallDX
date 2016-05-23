#include <iostream>
#include <vector>
#include "TDFAPIStruct.h"
#include <d3d11_1.h>
#include "DirectXMath.h"
#include <algorithm> 
#include "GraphicUtil.h"



//轻量化的数据 压缩数据量
typedef struct TDF_DATA_LITE
{
	unsigned int nMatch;				//最新价
	unsigned int nNumTrades;			//成交笔数
	__int64		 iVolume;				//成交总量
	unsigned int nWeightedAvgBidPrice;	//加权平均委买价格
	unsigned int nWeightedAvgAskPrice;  //加权平均委卖价格
	TDF_DATA_LITE();
	TDF_DATA_LITE(TDF_MARKET_DATA);
};

class SingleStock{
private:
	std::vector<TDF_DATA_LITE> dataByFrame;
	std::string code;
	int max, min;
	int frameCount=0;
	float sx, sy;
	float change;
	float changeRate;
	void updateLowerBound(TDF_MARKET_DATA);
	void updateUpperBound(TDF_MARKET_DATA);
	


	
public:
	std::vector<DirectX::XMFLOAT3> vertices;
	void render();
	void addFrame(TDF_MARKET_DATA);
	SingleStock(TDF_MARKET_DATA);
	SingleStock(){};
	void setCoord(float, float);
};
