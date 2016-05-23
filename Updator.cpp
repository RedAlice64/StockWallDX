//Singleton

#include "TDFAPI.h"
#include "afxwin.h"




class Updator{
private:
	
	static void systemMsgHandler(THANDLE hTdf, TDF_MSG* pMsgHead){

	}
	static void dataMsgHandler(THANDLE hTdf, TDF_MSG* pMsgHead){
		//printf("%s", (char*)pMsgHead);
		TDF_MARKET_DATA* marketData = (TDF_MARKET_DATA*)pMsgHead->pData;
		switch (pMsgHead->nDataType){

		case MSG_DATA_FUTURE:
			break;
		case MSG_DATA_MARKET:

			printf("%d %d\n", marketData->nTime, marketData->nMatch);
			break;
		case MSG_DATA_INDEX:
			TDF_INDEX_DATA* indexData = (TDF_INDEX_DATA*)pMsgHead->pData;
			printf("%d %d\n", indexData->nTime, indexData->nLastIndex);
			break;
		}
	}
	void Update(){
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
			"601601.SH",
			0x1,
			0,
			0
		};
		tHandle = TDF_Open(&tSetting, &tErr);
		while (1);
	}

public:
	void init(){
		CWinThread thread;
	}
};