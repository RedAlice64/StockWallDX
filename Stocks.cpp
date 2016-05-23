#include <iostream>
#include <vector>
#include "TDFAPIStruct.h"
#include "Stocks.h"



SingleStock::SingleStock(TDF_MARKET_DATA data)
{
	code = data.szCode;
	dataByFrame.push_back(data);
	max = data.nOpen + 100;
	min = data.nOpen - 100;
};
void SingleStock::addFrame(TDF_MARKET_DATA data)
{
	
	dataByFrame.push_back(data); 
	if (data.nLow < min)updateLowerBound(data);
	if (data.nHigh>max)updateUpperBound(data);
	int range = max - min;
	int intx = data.nMatch - (range / 2) - min;
	int hh = data.nTime / 10000000-9;
	int mm = data.nTime / 100000 % 100;
	int ss = data.nTime / 1000 % 100;
	int seconds = hh * 3600 + mm * 60 + ss-1500;
	if (hh > 3)seconds -= 5400;
	float y = (float)intx / (range*5), x = (float)seconds / 36750;
	if (frameCount < 10 && vertices.size()>0)
	{
		frameCount++;
		vertices[vertices.size() - 1] = { x + sx, y + sy, 0 };
	}
	else
	{
		vertices.push_back({ x + sx, y + sy , 0 });
		frameCount = 0;
	}

	//calculate change and change rate
	change =  (double)data.nMatch / (double)data.nPreClose -1;
	changeRate =  (data.nMatch-dataByFrame[dataByFrame.size()-2].nMatch) / (double)data.nPreClose;
}
void SingleStock::updateUpperBound(TDF_MARKET_DATA data)
{
	max = data.nHigh + 50;
	int end = vertices.size() - 1;
	for (int i = 0; i < end; i++)
	{
		int range = max - min;
		int intx = dataByFrame[i*10].nMatch - (range / 2) - min;
		float y = (float)intx / (range * 5);
		vertices[i].y =  y+sy;
	}
}
void SingleStock::updateLowerBound(TDF_MARKET_DATA data)
{
	min = data.nLow - 50;
	int end = vertices.size() - 1;
	for (int i = 0; i < end; i++)
	{
		int range = max - min;
		int intx = dataByFrame[i * 10].nMatch - (range / 2) - min;
		float y = (float)intx / (range * 5);
		vertices[i].y = y + sy;
	}
}

void SingleStock::render()
{
	colorVertice backSquare[]=
	{
		{
			DirectX::XMFLOAT3(0, -0.2, 1.0),
			DirectX::XMFLOAT4(0.0f, 0, 0, 1),
		},
		{
			DirectX::XMFLOAT3(0, 0.2, 1.0),
			DirectX::XMFLOAT4(0.0f, 0, 0,1),
		},
		{
			DirectX::XMFLOAT3(0.4, 0.2, 1.0),
			DirectX::XMFLOAT4(0.0f, 0, 0, 1),
		},
		{
			DirectX::XMFLOAT3(0.4, -0.2, 1.0),
			DirectX::XMFLOAT4(0.0f, 0, 0, 1),
		},
		{
			DirectX::XMFLOAT3(0, -0.2, 1.0),
			DirectX::XMFLOAT4(0.0f, 0, 0, 1),
		},
		{
			DirectX::XMFLOAT3(0.4, 0.2, 1.0),
			DirectX::XMFLOAT4(0.0f, 0, 0, 1),
		}
	};

	for (colorVertice &i:backSquare)
	{
		i.pos.x += sx;
		i.pos.y += sy;
		//i.color.z = 1.0f;
		if (change > 0)
		{
			i.color.x = GraphicUtil::changePositiveColor.x*change;
			i.color.y = GraphicUtil::changePositiveColor.y*change;
			i.color.z = GraphicUtil::changePositiveColor.z*change;
		}
		else
		{
			i.color.x = -(GraphicUtil::changeNegativeColor.x*change);
			i.color.y = -(GraphicUtil::changeNegativeColor.y*change);
			i.color.z = -(GraphicUtil::changeNegativeColor.z*change);
		}
		if (changeRate > 0)
		{
			if (change > 0)
			{
				i.color.x += GraphicUtil::changeRatePositiveColor.x*changeRate+0.3f;
				i.color.y += GraphicUtil::changeRatePositiveColor.y*changeRate;
				i.color.z += GraphicUtil::changeRatePositiveColor.z*changeRate;
			}
			else
			{
				i.color.x += GraphicUtil::changeNegativeRatePositiveColor.x*changeRate;
				i.color.y += GraphicUtil::changeNegativeRatePositiveColor.y*changeRate;
				i.color.y -= 0.3;
				i.color.z += GraphicUtil::changeNegativeRatePositiveColor.z*changeRate;
			}
		}
		else if (changeRate<0)
		{
			i.color.x += -(GraphicUtil::changeRateNegativeColor.x*changeRate);
			i.color.y += -(GraphicUtil::changeRateNegativeColor.y*changeRate);
			i.color.z += -(GraphicUtil::changeRateNegativeColor.z*changeRate)+1.0f;
		}
	}
	//create background buffer
	D3D11_BUFFER_DESC bd1;
	ZeroMemory(&bd1, sizeof(bd1));
	bd1.Usage = D3D11_USAGE_DEFAULT;
	bd1.ByteWidth = sizeof(colorVertice)*6;
	bd1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd1.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData1;
	ZeroMemory(&InitData1, sizeof(InitData1));
	InitData1.pSysMem = &backSquare;
	GraphicUtil::device->CreateBuffer(&bd1, &InitData1, &GraphicUtil::vertexBackBuffer);

	//create now line buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DirectX::XMFLOAT3)*vertices.size() - 1;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &(vertices[0]);
	GraphicUtil::device->CreateBuffer(&bd, &InitData, &GraphicUtil::vertexBuffer);


	//switch to backsqare shader and draw
	GraphicUtil::swichPaint(DYNAMIC_PAINT);

	// Set vertex buffer
	UINT stride1 = sizeof(colorVertice);
	UINT offset1 = 0;
	GraphicUtil::deviceContext->IASetVertexBuffers(0, 1, &GraphicUtil::vertexBackBuffer, &stride1, &offset1);

	// Set primitive topology
	GraphicUtil::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render a triangle
	GraphicUtil::deviceContext->Draw(6, 0);

	///////////////////////////////////////////////////////////////////
	//switch to nowprice shader and draw
	GraphicUtil::swichPaint(YELLOW_PAINT);

	// Set vertex buffer
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	GraphicUtil::deviceContext->IASetVertexBuffers(0, 1, &GraphicUtil::vertexBuffer, &stride, &offset);

	// Set primitive topology
	GraphicUtil::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// Render lines
	GraphicUtil::deviceContext->Draw(vertices.size() - 1, 0);

	

}

void SingleStock::setCoord(float _x, float _y)
{
	sx = _x;
	sy = _y;
}

TDF_DATA_LITE::TDF_DATA_LITE(TDF_MARKET_DATA _data)
{
	nMatch = _data.nMatch;
	nNumTrades = _data.nNumTrades;
	iVolume = _data.iVolume;
	nWeightedAvgAskPrice = _data.nWeightedAvgAskPrice;
	nWeightedAvgBidPrice = _data.nWeightedAvgBidPrice;
}
