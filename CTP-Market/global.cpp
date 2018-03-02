#include "global.h"

CThostFtdcMdApi* g_pMdUserApi;// 行情指针 
CThostFtdcTraderApi *g_pTradeUserApi;                    // 交易指针  

						   
TThostFtdcBrokerIDType gBrokerID = "9999";                         // 模拟经纪商代码  
TThostFtdcInvestorIDType gInvesterID = "113324";              // 投资者账户名  
TThostFtdcPasswordType gInvesterPassword = "880716";               // 投资者密码  

																		 // 行情参数  
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";               // 模拟行情前置地址  
//char *g_pInstrumentID[] = { "al1901", "al1902", "al1803", "al1804" }; // 行情合约代码列表，中、上、大、郑交易所各选一种  
char *g_pInstrumentID[] = { "cu1811" };
int instrumentNum = 1;                                             // 行情合约订阅数量  
																   //unordered_map<string, TickToKlineHelper> g_KlineHash;              // 不同合约的k线存储表  

																   // 交易参数  

char gTradeFrontAddr[] = "tcp://180.168.146.187:10001";            // 模拟交易前置地址  
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "cu1811";         // 所交易的合约代码  
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Buy;       // 买卖方向  
TThostFtdcPriceType gLimitPrice = 53800;