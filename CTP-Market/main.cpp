#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "global.h"
#include "CustomMdSpi.h"
#include "CustomTrader.h"

using namespace std;


extern CThostFtdcMdApi* g_pMdUserApi;// ����ָ��
extern char gMdFrontAddr[];
extern TThostFtdcBrokerIDType gBrokerID;                         // ģ�⾭���̴���  
extern TThostFtdcInvestorIDType gInvesterID;                      // Ͷ�����˻���  
extern TThostFtdcPasswordType gInvesterPassword;                 // Ͷ��������  


extern CThostFtdcTraderApi *g_pTradeUserApi; //����ָ��
extern char gTradeFrontAddr[]; //ģ�⽻��ǰ�õ�ַ

int main()
{
	printf("CTP�˺�Ϊ:%s,����Ϊ%s\n", &gInvesterID, &gInvesterPassword);

	// ��ʼ�������߳�  
	cout << "CTP��ʼ������..." << endl;

	g_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();   // ��������ʵ��  
	CThostFtdcMdSpi *pMdUserSpi = new CustomMdSpi();       // ��������ص�ʵ�� 

	g_pMdUserApi->RegisterSpi(pMdUserSpi);               // ע���¼���  
	g_pMdUserApi->RegisterFront(gMdFrontAddr);           // ��������ǰ�õ�ַ  
	g_pMdUserApi->Init();
	


	// ��ʼ�������߳�  
	cout << "��ʼ������..." << endl;
	g_pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(); // ��������ʵ��  
																  //CThostFtdcTraderSpi *pTradeSpi = new CustomTradeSpi;  
	CustomTradeSpi *pTradeSpi = new CustomTradeSpi();               // �������׻ص�ʵ��  
	g_pTradeUserApi->RegisterSpi(pTradeSpi);                      // ע���¼���  
	g_pTradeUserApi->SubscribePublicTopic(THOST_TERT_RESTART);    // ���Ĺ�����  
	g_pTradeUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // ����˽����  
	g_pTradeUserApi->RegisterFront(gTradeFrontAddr);              // ���ý���ǰ�õ�ַ  
	g_pTradeUserApi->Init();                                      // ��������  


																  // �ȵ��߳��˳�  
	g_pMdUserApi->Join();
	delete pMdUserSpi;
	g_pMdUserApi->Release();

	g_pTradeUserApi->Join();
	delete pTradeSpi;
	g_pTradeUserApi->Release();

	
	getchar();
	return 0;
}