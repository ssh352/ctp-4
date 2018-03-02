#pragma once

#include "ThostFtdcTraderApi.h"
#ifndef CTP_MARKET_CUSTOMTRADER
#define CTP_MARKET_CUSTOMTRADER

class CustomTradeSpi : public CThostFtdcTraderSpi
{
	// ---- ctp_api���ֻص��ӿ� ---- //  
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	
	
	
	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	
	
	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQrySecAgentTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	virtual void OnRspQryTradingAccount(
		CThostFtdcTradingAccountField *pTradingAccount,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast);

	//Ͷ���ֲֲ߳�ѯӦ��
	virtual void OnRspQryInvestorPosition(
		CThostFtdcInvestorPositionField *pInvestorPosition,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast);

	virtual void OnRspOrderInsert(
		CThostFtdcInputOrderField *pInputOrder,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast);

	///����֪ͨ  
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///�ɽ�֪ͨ  
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	/*
	///��¼������Ӧ  
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��  
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������  
	void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�  
	void OnHeartBeatWarning(int nTimeLapse);

	///�ǳ�������Ӧ  
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߽�����ȷ����Ӧ  
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	

	

	///�����ѯͶ���ֲ߳���Ӧ  
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��������Ӧ  
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������������Ӧ  
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	

	
	*/

	// ---- �Զ��庯�� ---- //  
public:
	bool loginFlag; // ��½�ɹ��ı�ʶ  
	void reqOrderInsert(
		TThostFtdcInstrumentIDType instrumentID,
		TThostFtdcPriceType price,
		TThostFtdcVolumeType volume,
		TThostFtdcDirectionType direction); // ���Ի�����¼�룬�ⲿ����  
private:
	void reqUserLogin(); // ��¼����  
	void reqUserLogout(); // �ǳ�����  
	void reqSettlementInfoConfirm(); // Ͷ���߽��ȷ��  
	void reqQueryInstrument(); // �����ѯ��Լ  
	void reqQueryTradingAccount(); // �����ѯ�ʽ��ʻ�  
	void reqQueryInvestorPosition(); // �����ѯͶ���ֲ߳�  
	void reqOrderInsert1(); // ���󱨵�¼��  

	void reqOrderAction(CThostFtdcOrderField *pOrder); // ���󱨵�����  
	bool isErrorRspInfo(CThostFtdcRspInfoField *pRspInfo); // �Ƿ��յ�������Ϣ  
	bool isMyOrder(CThostFtdcOrderField *pOrder); // �Ƿ��ҵı����ر�  
	bool isTradingOrder(CThostFtdcOrderField *pOrder); // �Ƿ����ڽ��׵ı���  
};

#endif