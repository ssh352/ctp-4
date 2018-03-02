#include "CustomMdSpi.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;


extern CThostFtdcMdApi* g_pMdUserApi;// 行情指针

extern TThostFtdcBrokerIDType gBrokerID;                         // 模拟经纪商代码  
extern TThostFtdcInvestorIDType gInvesterID;                      // 投资者账户名  
extern TThostFtdcPasswordType gInvesterPassword;                 // 投资者密码  

																		 // 行情参数  
extern char gMdFrontAddr[];            // 模拟行情前置地址  
extern char *g_pInstrumentID[]; // 行情合约代码列表，中、上、大、郑交易所各选一种  
extern int instrumentNum;                                     // 行情合约订阅数量  
																   //unordered_map<string, TickToKlineHelper> g_KlineHash;              // 不同合约的k线存储表  

																   // 交易参数  
extern CThostFtdcTraderApi *g_pTradeUserApi;             // 交易指针  
extern char gTradeFrontAddr[] ;            // 模拟交易前置地址  
extern TThostFtdcInstrumentIDType g_pTradeInstrumentID;         // 所交易的合约代码  
																   //TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // 买卖方向  
extern TThostFtdcPriceType gLimitPrice ;





void CustomMdSpi::OnFrontConnected()
{
	printf("BMB:已经成功连接交易系统行情接口!\n");

	
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy(loginReq.BrokerID, gBrokerID);
	strcpy(loginReq.UserID, gInvesterID);
	strcpy(loginReq.Password, gInvesterPassword);
	static int requestID = 0; // 请求编号  
	int rt = g_pMdUserApi->ReqUserLogin(&loginReq, requestID);
	if (!rt)
		std::cout << ">>>>>>发送登录请求成功" << std::endl;
	else
		std::cerr << "--->>>发送登录请求失败" << std::endl;
		
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
		std::cout << "=====账户登录成功=====" << std::endl;
		std::cout << "交易日： " << pRspUserLogin->TradingDay << std::endl;
		std::cout << "登录时间： " << pRspUserLogin->LoginTime << std::endl;
		std::cout << "经纪商： " << pRspUserLogin->BrokerID << std::endl;
		std::cout << "帐户名： " << pRspUserLogin->UserID << std::endl;
		// 开始订阅行情  
		int rt = g_pMdUserApi->SubscribeMarketData(g_pInstrumentID, instrumentNum);
		if (!rt)
			std::cout << ">>>>>>发送订阅行情请求成功" << std::endl;
		else
			std::cerr << "--->>>发送订阅行情请求失败" << std::endl;
	}
	else
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
		
}


void CustomMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		/*
		std::cout << "=====订阅行情成功=====" << std::endl;
		std::cout << "合约代码： " << pSpecificInstrument->InstrumentID << std::endl;*/
		// 如果需要存入文件或者数据库，在这里创建表头,不同的合约单独存储  
		char filePath[100] = { '\0' };
		sprintf(filePath, "%s_market_data.csv", pSpecificInstrument->InstrumentID);
		ofstream outFile;
		outFile.open(filePath, std::ios::out); // 新开文件  
		outFile << "合约代码" << ","
			<< "更新时间" << ","
			<< "最新价" << ","
			<< "今开盘" << ","
			<< "最高价" << ","
			<< "成交量" << ","
			<< "本次结算价" << ","
			<< "买价一" << ","
			<< "买量一" << ","
			<< "买价2" << ","
			<< "买量2" << ","
			<< "卖价一" << ","
			<< "卖量一" << ","
			<< "持仓量" << ","
			<< "换手率"
			<< std::endl;
		outFile.close();
	}
	else
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}


void CustomMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	time_t t;
	struct tm * lt;
	time(&t);//获取Unix时间戳。
	lt = localtime(&t);//转为时间结构。
	//printf("%d/%d/%d %d:%d:%d\n", lt->tm_year + 1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);//输出结果

 
	//std::cout << "=====获得深度行情=====" << std::endl;
	/*
	std::cout << "交易日： " << pDepthMarketData->TradingDay << std::endl;
	std::cout << "更新时间： " << pDepthMarketData->UpdateTime << std::endl;
	std::cout << "交易所代码： " << pDepthMarketData->ExchangeID << std::endl;
	std::cout << "合约代码： " << pDepthMarketData->InstrumentID << std::endl;
	std::cout << "合约在交易所的代码： " << pDepthMarketData->ExchangeInstID << std::endl;
	std::cout << "最新价： " << pDepthMarketData->LastPrice << std::endl;
	std::cout << "成交量： " << pDepthMarketData->Volume << std::endl;
	std::cout << "上次结算价(博弈云->昨结)： " << pDepthMarketData->PreSettlementPrice << std::endl;
	std::cout << "最高价(博弈云->最高)： " << pDepthMarketData->HighestPrice << std::endl;
	std::cout << "最低价(博弈云->最低)： " << pDepthMarketData->LowestPrice << std::endl;
	//std::cout << "本次结算价(博弈云->最低)： " << pDepthMarketData->SettlementPrice << std::endl;
	std::cout << "昨虚实度： " << pDepthMarketData->PreDelta << std::endl;
	std::cout << "今虚实度： " << pDepthMarketData->CurrDelta << std::endl;
	std::cout << "成交金额： " << pDepthMarketData->Turnover << std::endl;
	std::cout << "涨停板价： " << pDepthMarketData->UpperLimitPrice << std::endl;
	std::cout << "跌停板价： " << pDepthMarketData->LowerLimitPrice << std::endl;
	std::cout << "博弈云->涨跌(最新价-昨结)： " << pDepthMarketData->LastPrice - pDepthMarketData->PreSettlementPrice << std::endl;
	std::cout << "博弈云->涨幅%： " << ((pDepthMarketData->LastPrice - pDepthMarketData->PreSettlementPrice)/ pDepthMarketData->PreSettlementPrice)*100 << std::endl;
	std::cout << "博弈云->仓差： " << pDepthMarketData->OpenInterest - pDepthMarketData->PreOpenInterest << std::endl;
	*/
	
	// 如果只获取某一个合约行情，可以逐tick地存入文件或数据库  
	char filePath[100] = { '\0' };
	sprintf(filePath, "%s_market_data.csv", pDepthMarketData->InstrumentID);
	std::ofstream outFile;
	outFile.open(filePath, std::ios::app); // 文件追加写入   
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

	// 计算实时k线  
	/*std::string instrumentKey = std::string(pDepthMarketData->InstrumentID);
	if (g_KlineHash.find(instrumentKey) == g_KlineHash.end())
		g_KlineHash[instrumentKey] = TickToKlineHelper();
	g_KlineHash[instrumentKey].KLineFromRealtimeData(pDepthMarketData);*/


	// 取消订阅行情  
	//int rt = g_pMdUserApi->UnSubscribeMarketData(g_pInstrumentID, instrumentNum);  
	//if (!rt)  
	//  std::cout << ">>>>>>发送取消订阅行情请求成功" << std::endl;  
	//else  
	//  std::cerr << "--->>>发送取消订阅行情请求失败" << std::endl;  
}