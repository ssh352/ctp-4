#include "CustomTrader.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <thread>
#include <iomanip>
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
extern char gTradeFrontAddr[];            // ģ�⽻��ǰ�õ�ַ  
extern TThostFtdcInstrumentIDType g_pTradeInstrumentID;         // �����׵ĺ�Լ����  
																//TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // ��������  
extern TThostFtdcPriceType gLimitPrice;
extern TThostFtdcDirectionType gTradeDirection;

int trade_front_id;
int session_id;
char order_ref[13];


void CustomTradeSpi::OnFrontConnected()
{
	printf("BMB:�Ѿ��ɹ�����CTP���׽ӿ�!\n");

	
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy(loginReq.BrokerID, gBrokerID);
	strcpy(loginReq.UserID, gInvesterID);
	strcpy(loginReq.Password, gInvesterPassword);
	static int requestID = 0; // ������  
	int rt = g_pTradeUserApi->ReqUserLogin(&loginReq, requestID);
	if (!rt)
		std::cout << ">>>>>>���͵�¼����ɹ�" << std::endl;
	else
		std::cerr << "--->>>���͵�¼����ʧ��" << std::endl;
		
}


void CustomTradeSpi::reqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField settlementConfirmReq;
	memset(&settlementConfirmReq, 0, sizeof(settlementConfirmReq));
	strcpy(settlementConfirmReq.BrokerID, gBrokerID);
	strcpy(settlementConfirmReq.InvestorID, gInvesterID);
	static int requestID = 0; // ������
	int rt = g_pTradeUserApi->ReqSettlementInfoConfirm(&settlementConfirmReq, requestID);
	if (!rt)
		std::cout << ">>>>>>����Ͷ���߽�����ȷ������ɹ�" << std::endl;
	else
		std::cerr << "--->>>����Ͷ���߽�����ȷ������ʧ��" << std::endl;
}


void CustomTradeSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====�˻���¼�ɹ�->���׽ӿ�=====" << std::endl;
		loginFlag = true;
		std::cout << "�����գ� " << pRspUserLogin->TradingDay << std::endl;
		std::cout << "��¼ʱ�䣺 " << pRspUserLogin->LoginTime << std::endl;
		std::cout << "�����̣� " << pRspUserLogin->BrokerID << std::endl;
		std::cout << "�ʻ����� " << pRspUserLogin->UserID << std::endl;
		// ����Ự����  
		trade_front_id = pRspUserLogin->FrontID;
		session_id = pRspUserLogin->SessionID;
		strcpy(order_ref, pRspUserLogin->MaxOrderRef);

		// Ͷ���߽�����ȷ��  
		reqSettlementInfoConfirm();
		
	}
}


///Ͷ���߽�����ȷ����Ӧ
void CustomTradeSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====Ͷ���߽�����ȷ�ϳɹ�=====" << std::endl;
		std::cout << "ȷ�����ڣ� " << pSettlementInfoConfirm->ConfirmDate << std::endl;
		std::cout << "ȷ��ʱ�䣺 " << pSettlementInfoConfirm->ConfirmTime << std::endl;
		// �����ѯ��Լ
		reqQueryInstrument();
	}
}


void CustomTradeSpi::reqQueryInstrument()
{
	CThostFtdcQryInstrumentField instrumentReq;
	memset(&instrumentReq, 0, sizeof(instrumentReq));
	strcpy(instrumentReq.InstrumentID, g_pTradeInstrumentID);
	static int requestID = 0; // ������
	int rt = g_pTradeUserApi->ReqQryInstrument(&instrumentReq, requestID);
	if (!rt)
		std::cout << ">>>>>>���ͺ�Լ��ѯ����ɹ�" << std::endl;
	else
		std::cerr << "--->>>���ͺ�Լ��ѯ����ʧ��" << std::endl;
}


void CustomTradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====��ѯ��Լ����ɹ�=====" << std::endl;
		std::cout << "���������룺 " << pInstrument->ExchangeID << std::endl;
		std::cout << "��Լ���룺 " << pInstrument->InstrumentID << std::endl;
		std::cout << "��Լ�ڽ������Ĵ��룺 " << pInstrument->ExchangeInstID << std::endl;
		std::cout << "ִ�мۣ� " << pInstrument->StrikePrice << std::endl;
		std::cout << "�����գ� " << pInstrument->EndDelivDate << std::endl;
		std::cout << "��ǰ����״̬�� " << pInstrument->IsTrading << std::endl;
		// �����ѯͶ�����ʽ��˻�
		reqQueryTradingAccount();
	}
}

void CustomTradeSpi::reqQueryTradingAccount()
{
	CThostFtdcQryTradingAccountField tradingAccountReq;
	memset(&tradingAccountReq, 0, sizeof(tradingAccountReq));
	strcpy(tradingAccountReq.BrokerID, gBrokerID);
	strcpy(tradingAccountReq.InvestorID, gInvesterID);
	static int requestID = 0; // ������
	std::this_thread::sleep_for(std::chrono::milliseconds(700)); // ��ʱ����Ҫͣ��һ����ܲ�ѯ�ɹ�
	int rt = g_pTradeUserApi->ReqQryTradingAccount(&tradingAccountReq, requestID);
	//int rt = g_pTradeUserApi->ReqQrySecAgentTradingAccount(&tradingAccountReq, requestID);
	if (!rt)
		std::cout << ">>>>>>����Ͷ�����ʽ��˻���ѯ����ɹ�" << std::endl;
	else
		std::cerr << "--->>>����Ͷ�����ʽ��˻���ѯ����ʧ��" << "return:"+rt<< std::endl;
}


void CustomTradeSpi::OnRspQryTradingAccount(
	CThostFtdcTradingAccountField *pTradingAccount,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====��ѯͶ�����ʽ��˻��ɹ�=====" << std::endl;
		std::cout << "Ͷ�����˺ţ� " << pTradingAccount->AccountID << std::endl;
		std::cout << "�����ʽ� " << setiosflags(ios::fixed) << pTradingAccount->Available << std::endl;
		std::cout << "��ȡ�ʽ� " << pTradingAccount->WithdrawQuota << std::endl;
		std::cout << "��ǰ��֤��: " << pTradingAccount->CurrMargin << std::endl;
		std::cout << "ƽ��ӯ���� " << pTradingAccount->CloseProfit << std::endl;
		// �����ѯͶ���ֲ߳�
		reqQueryInvestorPosition();
	}
}


void CustomTradeSpi::reqQueryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField postionReq;
	memset(&postionReq, 0, sizeof(postionReq));
	strcpy(postionReq.BrokerID, gBrokerID);
	strcpy(postionReq.InvestorID, gInvesterID);
	strcpy(postionReq.InstrumentID, g_pTradeInstrumentID);
	static int requestID = 0; // ������
	std::this_thread::sleep_for(std::chrono::milliseconds(700)); // ��ʱ����Ҫͣ��һ����ܲ�ѯ�ɹ�
	int rt = g_pTradeUserApi->ReqQryInvestorPosition(&postionReq, requestID);
	if (!rt)
		std::cout << ">>>>>>����Ͷ���ֲֲ߳�ѯ����ɹ�" << std::endl;
	else
		std::cerr << "--->>>����Ͷ���ֲֲ߳�ѯ����ʧ��" << std::endl;
}


//Ͷ���ֲֲ߳�ѯӦ��
void CustomTradeSpi::OnRspQryInvestorPosition(
	CThostFtdcInvestorPositionField *pInvestorPosition,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====��ѯͶ���ֲֳ߳ɹ�=====" << std::endl;
		if (pInvestorPosition)
		{
			std::cout << "��Լ���룺 " << pInvestorPosition->InstrumentID << std::endl;
			std::cout << "���ּ۸� " << pInvestorPosition->OpenAmount << std::endl;
			std::cout << "�������� " << pInvestorPosition->OpenVolume << std::endl;
			std::cout << "���ַ��� " << pInvestorPosition->PosiDirection << std::endl;
			std::cout << "ռ�ñ�֤��" << pInvestorPosition->UseMargin << std::endl;
		}
		else
			std::cout << "----->�ú�Լδ�ֲ�" << std::endl;

		// ����¼������������һ���ӿڣ��˴��ǰ�˳��ִ�У�
		/*if (loginFlag)
		reqOrderInsert();*/
		//if (loginFlag)
		//	reqOrderInsertWithParams(g_pTradeInstrumentID, gLimitPrice, 1, gTradeDirection); // �Զ���һ�ʽ���
		if (loginFlag)
			reqOrderInsert(g_pTradeInstrumentID, gLimitPrice, 1, gTradeDirection); // �Զ���һ�ʽ���  

		// ���Խ���
		/*std::cout << "=====��ʼ������Խ���=====" << std::endl;
		while (loginFlag)
			StrategyCheckAndTrade(g_pTradeInstrumentID, this);*/
	}
}


void CustomTradeSpi::reqOrderInsert(
	TThostFtdcInstrumentIDType instrumentID,
	TThostFtdcPriceType price,
	TThostFtdcVolumeType volume,
	TThostFtdcDirectionType direction)
{
	CThostFtdcInputOrderField orderInsertReq;
	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
	///���͹�˾����
	strcpy(orderInsertReq.BrokerID, gBrokerID);
	///Ͷ���ߴ���
	strcpy(orderInsertReq.InvestorID, gInvesterID);
	///��Լ����
	strcpy(orderInsertReq.InstrumentID, instrumentID);
	///��������
	strcpy(orderInsertReq.OrderRef, order_ref);
	///�����۸�����: �޼�
	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///��������: 
	orderInsertReq.Direction = direction;
	///��Ͽ�ƽ��־: ����
	orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///���Ͷ���ױ���־
	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	orderInsertReq.LimitPrice = price;
	///������1
	orderInsertReq.VolumeTotalOriginal = volume;
	///��Ч������: ������Ч
	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
	///�ɽ�������: �κ�����
	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	orderInsertReq.MinVolume = 1;
	///��������: ����
	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ǿƽԭ��: ��ǿƽ
	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	orderInsertReq.IsAutoSuspend = 0;
	///�û�ǿ����־: ��
	orderInsertReq.UserForceClose = 0;

	static int requestID = 0; // ������
	int rt = g_pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++requestID);
	if (!rt)
		std::cout << ">>>>>>���ͱ���¼������ɹ�" << std::endl;
	else
		std::cerr << "--->>>���ͱ���¼������ʧ��" << std::endl;
}

void CustomTradeSpi::reqOrderInsert1()
{
	CThostFtdcInputOrderField orderInsertReq;
	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
	///���͹�˾����
	strcpy(orderInsertReq.BrokerID, gBrokerID);
	///Ͷ���ߴ���
	strcpy(orderInsertReq.InvestorID, gInvesterID);
	///��Լ����
	strcpy(orderInsertReq.InstrumentID, g_pTradeInstrumentID);
	///��������
	strcpy(orderInsertReq.OrderRef, order_ref);
	///�����۸�����: �޼�
	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	///��������: 
	orderInsertReq.Direction = gTradeDirection;
	///��Ͽ�ƽ��־: ����
	orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///���Ͷ���ױ���־
	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	orderInsertReq.LimitPrice = 0;
	///������1
	orderInsertReq.VolumeTotalOriginal = 1;
	///��Ч������: ������Ч
	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
	///�ɽ�������: �κ�����
	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	orderInsertReq.MinVolume = 1;
	///��������: ����
	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ǿƽԭ��: ��ǿƽ
	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	orderInsertReq.IsAutoSuspend = 0;
	///�û�ǿ����־: ��
	orderInsertReq.UserForceClose = 0;

	static int requestID = 0; // ������
	int rt = g_pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++requestID);
	if (!rt)
		std::cout << ">>>>>>���ͱ���¼������ɹ�" << std::endl;
	else
		std::cerr << "--->>>���ͱ���¼������ʧ��" << std::endl;
}


void CustomTradeSpi::OnRspOrderInsert(
	CThostFtdcInputOrderField *pInputOrder,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====����¼��ɹ�=====" << std::endl;
		std::cout << "��Լ���룺 " << pInputOrder->InstrumentID << std::endl;
		std::cout << "�۸� " << pInputOrder->LimitPrice << std::endl;
		std::cout << "������ " << pInputOrder->VolumeTotalOriginal << std::endl;
		std::cout << "���ַ��� " << pInputOrder->Direction << std::endl;
	}
}

void CustomTradeSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	char str[10];
	sprintf(str, "%d", pOrder->OrderSubmitStatus);
	int orderState = atoi(str) - 48;    //����״̬0=�Ѿ��ύ��3=�Ѿ�����  

	std::cout << "=====�յ�����Ӧ��=====" << std::endl;

	if (isMyOrder(pOrder))
	{
		if (isTradingOrder(pOrder))
		{
			std::cout << "--->>> �ȴ��ɽ��У�" << std::endl;
			//reqOrderAction(pOrder); // ������Գ���  
			//reqUserLogout(); // �ǳ�����  
		}
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			std::cout << "--->>> �����ɹ���" << std::endl;
	}
}

void CustomTradeSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	std::cout << "=====�����ɹ��ɽ�=====" << std::endl;
	std::cout << "�ɽ�ʱ�䣺 " << pTrade->TradeTime << std::endl;
	std::cout << "��Լ���룺 " << pTrade->InstrumentID << std::endl;
	std::cout << "�ɽ��۸� " << pTrade->Price << std::endl;
	std::cout << "�ɽ����� " << pTrade->Volume << std::endl;
	std::cout << "��ƽ�ַ��� " << pTrade->Direction << std::endl;
}


void CustomTradeSpi::OnRspQrySecAgentTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====��ѯͶ�����ʽ��˻��ɹ�=====" << std::endl;
		std::cout << "Ͷ�����˺ţ� " << pTradingAccount->AccountID << std::endl;
		std::cout << "�����ʽ� " << pTradingAccount->Available << std::endl;
		std::cout << "��ȡ�ʽ� " << pTradingAccount->WithdrawQuota << std::endl;
		std::cout << "��ǰ��֤��: " << pTradingAccount->CurrMargin << std::endl;
		std::cout << "ƽ��ӯ���� " << pTradingAccount->CloseProfit << std::endl;
		// �����ѯͶ���ֲ߳�
		//reqQueryInvestorPosition();
	}
}


bool CustomTradeSpi::isErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult)
	{
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
	return bResult;
}


bool CustomTradeSpi::isMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == trade_front_id) &&
		(pOrder->SessionID == session_id) &&
		(strcmp(pOrder->OrderRef, order_ref) == 0));
}

bool CustomTradeSpi::isTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}