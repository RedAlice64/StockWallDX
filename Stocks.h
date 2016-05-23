#include <iostream>
#include <vector>
#include "TDFAPIStruct.h"
#include <d3d11_1.h>
#include "DirectXMath.h"
#include <algorithm> 
#include "GraphicUtil.h"



//������������ ѹ��������
typedef struct TDF_DATA_LITE
{
	unsigned int nMatch;				//���¼�
	unsigned int nNumTrades;			//�ɽ�����
	__int64		 iVolume;				//�ɽ�����
	unsigned int nWeightedAvgBidPrice;	//��Ȩƽ��ί��۸�
	unsigned int nWeightedAvgAskPrice;  //��Ȩƽ��ί���۸�
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
