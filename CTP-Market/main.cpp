#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "global.h"
#include "CustomMdSpi.h"
#include "CustomTrader.h"

using namespace std;


extern CThostFtdcMdApi* g_pMdUserApi;// 行情指针
extern char gMdFrontAddr[];
extern TThostFtdcBrokerIDType gBrokerID;                         // 模拟经纪商代码  
extern TThostFtdcInvestorIDType gInvesterID;                      // 投资者账户名  
extern TThostFtdcPasswordType gInvesterPassword;                 // 投资者密码  


extern CThostFtdcTraderApi *g_pTradeUserApi; //交易指针
extern char gTradeFrontAddr[]; //模拟交易前置地址

int main()
{
	printf("CTP账号为:%s,密码为%s\n", &gInvesterID, &gInvesterPassword);

	// 初始化行情线程  
	cout << "CTP初始化行情..." << endl;

	g_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();   // 创建行情实例  
	CThostFtdcMdSpi *pMdUserSpi = new CustomMdSpi();       // 创建行情回调实例 

	g_pMdUserApi->RegisterSpi(pMdUserSpi);               // 注册事件类  
	g_pMdUserApi->RegisterFront(gMdFrontAddr);           // 设置行情前置地址  
	g_pMdUserApi->Init();
	


	// 初始化交易线程  
	cout << "初始化交易..." << endl;
	g_pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(); // 创建交易实例  
																  //CThostFtdcTraderSpi *pTradeSpi = new CustomTradeSpi;  
	CustomTradeSpi *pTradeSpi = new CustomTradeSpi();               // 创建交易回调实例  
	g_pTradeUserApi->RegisterSpi(pTradeSpi);                      // 注册事件类  
	g_pTradeUserApi->SubscribePublicTopic(THOST_TERT_RESTART);    // 订阅公共流  
	g_pTradeUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // 订阅私有流  
	g_pTradeUserApi->RegisterFront(gTradeFrontAddr);              // 设置交易前置地址  
	g_pTradeUserApi->Init();                                      // 连接运行  


																  // 等到线程退出  
	g_pMdUserApi->Join();
	delete pMdUserSpi;
	g_pMdUserApi->Release();

	g_pTradeUserApi->Join();
	delete pTradeSpi;
	g_pTradeUserApi->Release();

	
	getchar();
	return 0;
}