#include "global.h"

CThostFtdcMdApi* g_pMdUserApi;// ����ָ�� 
CThostFtdcTraderApi *g_pTradeUserApi;                    // ����ָ��  

						   
TThostFtdcBrokerIDType gBrokerID = "9999";                         // ģ�⾭���̴���  
TThostFtdcInvestorIDType gInvesterID = "113324";              // Ͷ�����˻���  
TThostFtdcPasswordType gInvesterPassword = "880716";               // Ͷ��������  

																		 // �������  
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";               // ģ������ǰ�õ�ַ  
//char *g_pInstrumentID[] = { "al1901", "al1902", "al1803", "al1804" }; // �����Լ�����б��С��ϡ���֣��������ѡһ��  
char *g_pInstrumentID[] = { "cu1811" };
int instrumentNum = 1;                                             // �����Լ��������  
																   //unordered_map<string, TickToKlineHelper> g_KlineHash;              // ��ͬ��Լ��k�ߴ洢��  

																   // ���ײ���  

char gTradeFrontAddr[] = "tcp://180.168.146.187:10001";            // ģ�⽻��ǰ�õ�ַ  
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "cu1811";         // �����׵ĺ�Լ����  
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Buy;       // ��������  
TThostFtdcPriceType gLimitPrice = 53800;