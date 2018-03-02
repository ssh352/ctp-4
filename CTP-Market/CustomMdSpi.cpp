#include "CustomMdSpi.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;


extern CThostFtdcMdApi* g_pMdUserApi;// ����ָ��

extern TThostFtdcBrokerIDType gBrokerID;                         // ģ�⾭���̴���  
extern TThostFtdcInvestorIDType gInvesterID;                      // Ͷ�����˻���  
extern TThostFtdcPasswordType gInvesterPassword;                 // Ͷ��������  

																		 // �������  
extern char gMdFrontAddr[];            // ģ������ǰ�õ�ַ  
extern char *g_pInstrumentID[]; // �����Լ�����б��С��ϡ���֣��������ѡһ��  
extern int instrumentNum;                                     // �����Լ��������  
																   //unordered_map<string, TickToKlineHelper> g_KlineHash;              // ��ͬ��Լ��k�ߴ洢��  

																   // ���ײ���  
extern CThostFtdcTraderApi *g_pTradeUserApi;             // ����ָ��  
extern char gTradeFrontAddr[] ;            // ģ�⽻��ǰ�õ�ַ  
extern TThostFtdcInstrumentIDType g_pTradeInstrumentID;         // �����׵ĺ�Լ����  
																   //TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // ��������  
extern TThostFtdcPriceType gLimitPrice ;





void CustomMdSpi::OnFrontConnected()
{
	printf("BMB:�Ѿ��ɹ����ӽ���ϵͳ����ӿ�!\n");

	
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy(loginReq.BrokerID, gBrokerID);
	strcpy(loginReq.UserID, gInvesterID);
	strcpy(loginReq.Password, gInvesterPassword);
	static int requestID = 0; // ������  
	int rt = g_pMdUserApi->ReqUserLogin(&loginReq, requestID);
	if (!rt)
		std::cout << ">>>>>>���͵�¼����ɹ�" << std::endl;
	else
		std::cerr << "--->>>���͵�¼����ʧ��" << std::endl;
		
};


void CustomMdSpi::OnFrontDisconnected(int nReason)
{
	printf("connect failed!");
}

void CustomMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "=====�˻���¼�ɹ�=====" << std::endl;
		std::cout << "�����գ� " << pRspUserLogin->TradingDay << std::endl;
		std::cout << "��¼ʱ�䣺 " << pRspUserLogin->LoginTime << std::endl;
		std::cout << "�����̣� " << pRspUserLogin->BrokerID << std::endl;
		std::cout << "�ʻ����� " << pRspUserLogin->UserID << std::endl;
		// ��ʼ��������  
		int rt = g_pMdUserApi->SubscribeMarketData(g_pInstrumentID, instrumentNum);
		if (!rt)
			std::cout << ">>>>>>���Ͷ�����������ɹ�" << std::endl;
		else
			std::cerr << "--->>>���Ͷ�����������ʧ��" << std::endl;
	}
	else
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
		
}


void CustomMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		/*
		std::cout << "=====��������ɹ�=====" << std::endl;
		std::cout << "��Լ���룺 " << pSpecificInstrument->InstrumentID << std::endl;*/
		// �����Ҫ�����ļ��������ݿ⣬�����ﴴ����ͷ,��ͬ�ĺ�Լ�����洢  
		char filePath[100] = { '\0' };
		sprintf(filePath, "%s_market_data.csv", pSpecificInstrument->InstrumentID);
		ofstream outFile;
		outFile.open(filePath, std::ios::out); // �¿��ļ�  
		outFile << "��Լ����" << ","
			<< "����ʱ��" << ","
			<< "���¼�" << ","
			<< "����" << ","
			<< "��߼�" << ","
			<< "�ɽ���" << ","
			<< "���ν����" << ","
			<< "���һ" << ","
			<< "����һ" << ","
			<< "���2" << ","
			<< "����2" << ","
			<< "����һ" << ","
			<< "����һ" << ","
			<< "�ֲ���" << ","
			<< "������"
			<< std::endl;
		outFile.close();
	}
	else
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}


void CustomMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	time_t t;
	struct tm * lt;
	time(&t);//��ȡUnixʱ�����
	lt = localtime(&t);//תΪʱ��ṹ��
	//printf("%d/%d/%d %d:%d:%d\n", lt->tm_year + 1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);//������

 
	//std::cout << "=====����������=====" << std::endl;
	/*
	std::cout << "�����գ� " << pDepthMarketData->TradingDay << std::endl;
	std::cout << "����ʱ�䣺 " << pDepthMarketData->UpdateTime << std::endl;
	std::cout << "���������룺 " << pDepthMarketData->ExchangeID << std::endl;
	std::cout << "��Լ���룺 " << pDepthMarketData->InstrumentID << std::endl;
	std::cout << "��Լ�ڽ������Ĵ��룺 " << pDepthMarketData->ExchangeInstID << std::endl;
	std::cout << "���¼ۣ� " << pDepthMarketData->LastPrice << std::endl;
	std::cout << "�ɽ����� " << pDepthMarketData->Volume << std::endl;
	std::cout << "�ϴν����(������->���)�� " << pDepthMarketData->PreSettlementPrice << std::endl;
	std::cout << "��߼�(������->���)�� " << pDepthMarketData->HighestPrice << std::endl;
	std::cout << "��ͼ�(������->���)�� " << pDepthMarketData->LowestPrice << std::endl;
	//std::cout << "���ν����(������->���)�� " << pDepthMarketData->SettlementPrice << std::endl;
	std::cout << "����ʵ�ȣ� " << pDepthMarketData->PreDelta << std::endl;
	std::cout << "����ʵ�ȣ� " << pDepthMarketData->CurrDelta << std::endl;
	std::cout << "�ɽ��� " << pDepthMarketData->Turnover << std::endl;
	std::cout << "��ͣ��ۣ� " << pDepthMarketData->UpperLimitPrice << std::endl;
	std::cout << "��ͣ��ۣ� " << pDepthMarketData->LowerLimitPrice << std::endl;
	std::cout << "������->�ǵ�(���¼�-���)�� " << pDepthMarketData->LastPrice - pDepthMarketData->PreSettlementPrice << std::endl;
	std::cout << "������->�Ƿ�%�� " << ((pDepthMarketData->LastPrice - pDepthMarketData->PreSettlementPrice)/ pDepthMarketData->PreSettlementPrice)*100 << std::endl;
	std::cout << "������->�ֲ " << pDepthMarketData->OpenInterest - pDepthMarketData->PreOpenInterest << std::endl;
	*/
	
	// ���ֻ��ȡĳһ����Լ���飬������tick�ش����ļ������ݿ�  
	char filePath[100] = { '\0' };
	sprintf(filePath, "%s_market_data.csv", pDepthMarketData->InstrumentID);
	std::ofstream outFile;
	outFile.open(filePath, std::ios::app); // �ļ�׷��д��   
	outFile << pDepthMarketData->InstrumentID << ","
		<< pDepthMarketData->UpdateTime << "." << pDepthMarketData->UpdateMillisec << ","
		<< pDepthMarketData->LastPrice << ","
		<< pDepthMarketData->OpenPrice << ","
		<< pDepthMarketData->HighestPrice << ","
		<< pDepthMarketData->Volume << ","
		<< pDepthMarketData->SettlementPrice << ","
		<< pDepthMarketData->BidPrice1 << ","
		<< pDepthMarketData->BidVolume1 << ","
		<< pDepthMarketData->BidPrice2 << ","
		<< pDepthMarketData->BidVolume2 << ","
		<< pDepthMarketData->AskPrice1 << ","
		<< pDepthMarketData->AskVolume1 << ","
		<< pDepthMarketData->OpenInterest << ","
		<< pDepthMarketData->Turnover << std::endl;
	outFile.close();

	// ����ʵʱk��  
	/*std::string instrumentKey = std::string(pDepthMarketData->InstrumentID);
	if (g_KlineHash.find(instrumentKey) == g_KlineHash.end())
		g_KlineHash[instrumentKey] = TickToKlineHelper();
	g_KlineHash[instrumentKey].KLineFromRealtimeData(pDepthMarketData);*/


	// ȡ����������  
	//int rt = g_pMdUserApi->UnSubscribeMarketData(g_pInstrumentID, instrumentNum);  
	//if (!rt)  
	//  std::cout << ">>>>>>����ȡ��������������ɹ�" << std::endl;  
	//else  
	//  std::cerr << "--->>>����ȡ��������������ʧ��" << std::endl;  
}