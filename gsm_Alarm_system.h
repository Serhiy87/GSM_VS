/* ~~~~~~~~~~~~~
	����������� ������ ��� LISTENER 2
	������������ SIM800 � SIM900r.
	��� ������ ��������� webserver_listener2 

 	Modem: SIM800.
	AT+CGMR = Revision:1308B08SIM800M32

 	Modem: SIM900R
	AT+CGMR = Revision:1137B01SIM900R64_ST_ENHANCE_EAT - �� ������! ��������� ������!

	9600 bps,N,1

	TCP/IP Server Port 80 ��� webserver.h �������� �����. ����� �� 80 ����!
	
	�������� ��������� � HTTP Server webserver.h

	����� ���� - ������������� ��������
	����� ���� - ����� ������ �������� AT ������� � �������
	����� ����� - ������������� �������� 9600
	
	����� 
	������ ���������� UDP-������� �� 
	 ���� GPRS_FlgSz_Out!=0
	������ �� ���������� �� IP=0.0.0.0 ��� ���� ����������� ������� IP_SendMask!

	����� �������� SMS. SMS �� ������������ �� ����� 00000000000000!


	��� ����� ��������� ������: xxxxxxxxxxxxxxxx<CR><LF>
	'\r' CR - carridge return 0x0D
	'\n' LF - line feed 0x0A
	'\0' LF - ����� ������ 0x00
	������: ATE0<CR><LF>

	��� ��������� - �.�.����� ��������� �� ���� �� ��, ��� ��� ������ �� UART RX, ������ �����������

	A.����� ���������� �������� ������: 9600 baud.
	AT+IPR=9600
	AT&W (������� ������������ � ���������������� �����)
	�� ������������ ����� 115200bps �� ����� ������������� ���������� �������� (autobaud).
	
	B.SIM-�����
	³������� ����� PIN-����

	Brief AT-command description. For detail see SIM300_ATC_V2.03.pdf
	ATE0 - No echo
	AT+CMGF=1 - Set SMS system into text mode, as opposed to PDU mode
	AT+CSMP - Set SMS text mode parameters
	AT+CMGS - Send SMS
	AT+CIPMUX - to enable multi connection mode
	AT+CGATT=1 - Attach to GPRS network
	AT+CIPCSGP - Set CSD or GPRS connection mode. +CIPCSGP:1-GPRS, APN, USER NAME, PASSWORD
	AT+CSTT - START task and Set APN..USER ID..PASSWORD
	AT+CIICR - Bring up wireless connection with GPRS or CSD
			AT+CIICR only activate moving scene at the status of IP START..after
			operate this command, the state changed to IP CONFIG. If module
			accept the activate operation, the state changed to IP IND; after module
			accept the activate operation, if activate successfully, the state changed
			to IP GPRSACT, response OK, otherwise response ERROR
	AT+CIFSR - Get local IP address
	AT+CLPORT - Set local port
	AT+CIPSERVER - Configure as a server
	AT+CIPCCON - choose connection. Note that there may exist two connections at one time: one connection is as
				client connecting with remote server, the other connection is as server connecting with remote client.
				Using this command to choose through which connection data is sent.
	AT+CIPSTART - Start up TCP or UDP connection
	AT+CIPSEND - Send data through TCP or UDP connection
	AT+CIPCLOSE - Close TCP or UDP Connection
	AT+CIPSHUT - Disconnect wireless connection

	��������� ��������:
	���� �� RX ��� ������(�������). �������, ���� ������������� ������� ������ ����� GSM_RxStr, �� ��� ������ � ���� ������ ������������
	� ����������� �������� �������	RxBufOverFlow. �.�. ��� ������� �������� ������� ������, ��� ������� scan cycle, �������� 
	������������� ��������.

	�������� ��������� ������ ����������� AT-������ ������� ���-��'\n' == 2
	�������� �� ��������� ������ �� AT+CIICR,AT+CIPSERVER, ������ ���������� ����������� ������� ������
		ERROR,OK �.�. ���� 'R' - ������ ����� - ������ �� AT+CIPSHUT
	
	20 ��� �������� ������������ � ���� AT+CGATT=1. �������� �.�.� ������� ���� ����� ������� �� ������ ���� ���������.

	24.12.2012	- ������� ����� 3 ��� ����� ���������� �� UDP(�.�. �������� �� IP2)

	25.01.2013	- ��������������� GPRS_TerminalProcess �� ����� 4x20 � 8x40

	15.10.2013	- ������� ��� TCP-���������� ��� ��������� ���������� TCP-��������,
				- ���������� APN(2-� ������), USER_NAME, PASSWORD
				- ������ ���� AT-������ ��� define GSM_DEBUG

	04.11.2013	- ������� ��� GSM_WAIT_CALL_READY �������� �� 20 ���

	04.02.2014	- ������� ��� GSM_SEND_CIICR �������� �� 120 ��� (��� APN test.umc.ua ����� ���������� �� 43 ���)

	13.02.2014	- ��� ���������� IP �������: ���� 255.255.255.255 - ������������� ����� ��� ������

	04.03.2014	- ��������� (1��� � 655,35���) ��������������� Server (������ ������ ����� Server �� �������)
					����� ���������� ���������� ����� ������������ TCP �������� �������� (TCP_CONNECT_timeout)
	
	15.07.2014 - ��������� (1��� � 15���) ������������ ������� GSM-�������
					���� �������� � ������� GSM_WAIT_Inv,GSM_WAIT_CIPSEND_SEND_OK
					�� ����������� (GSM_ReStart1), � ��������� � GSM_ServerIdle
	
	19.08.2014 - ��� ������ uint8_t sscanf(str,"%hhu") 1-����. ������ sscanf(str,"%hu"): �.�. 2 ����� ����� � ������

	07.04.2015 - ����� �������� �� ����� ��������� IP GSM_WAIT_CIFSR_OK. ��������� if(Timer16Stopp(TD_GSM)) GSM_State = GSM_ReStart1;
				- ������ ����� ������ 24 ���� TD_GSM_Reset. (������ ������ ����� ����� ����������, ��� ������� ��������� 
				�����������, �� ������ ���� ������������ � TCP/IP �������! )
***********	http

	06.05.2015 - ��������� TCP/IP-���������� � ���-��������
				��� ������� �������� �������� GSM_RXSTR_SIZE = 100 (����� 50)

	14.09.2015 - ���������� ���������� �������: CTS/RTS #define GSM_HW_FLOW_CONTROL - �� ��������! (��.webserver.h)

	18.09.2015 - � ��������� GSM_WAIT_CIPCLOSE_OK ��� ��� CLOSE OK ��� CLOSED

	22,09,2015 - ������������� ��������� ������ GSM_RxStr ��������� <CR><LF> uint8_t ForceEndStringFromFIFO(void)
	
	25,09,2015 - ����������� ��������� �� ��������� (������ ��������� ����) uint8_t AppProtocol � ������ GSM_ProtocolMode
				GSM_MultiCon

	29,09,2015 - ������ �� ��������� � GSM_DataMode ��� if(AppProtocol != _HTTP)

	19.10.2015 - ��� ������� ��������� � GSM_ProtocolMode. ������ �������� _HTTP
				��������� �������� �� ��������� ������ GSM_GotoNextVega
	
	23.10.2015 - �� ����� WebSession - �� ����������� ������� �������� � ������ GSM_ServerIdle
***********	������������ ��� SIM800
	
	13.05.2016 - ��� config �� ��������� AT+IPR=9600 -> ������� AT&W
				- ���������� ������ ������ AT+CGMM
				- SIM800 TCP-������ ��������(��� �����) - ����� ����: AT+CGATT=0; �����  AT+CGATT=1;

	07.07.2016 - ������ ��� Listener2, ������������ � WebserverListener2.
~~~~~~~~~~~~~*/

void WebClose(void);
uint8_t IsWebSession(void);

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "ip.h"	// ������ � IP-��������

uint8_t GSM_Modem;
	#define NOT_RECOGNIZED	0
	#define SIMCOM_SIM900R	1
	#define SIMCOM_SIM800	2

prog_char GSM_SIMCOM_SIM900R[]	= "SIMCOM_SIM900R";
prog_char GSM_SIMCOM_SIM800[]	= "SIMCOM_SIM800";
prog_char GSM_SIMCOM_SIM800C[]	= "SIMCOM_SIM800C";

//#define GSM_HW_FLOW_CONTROL

//#define GSM_DEBUG	//���������� �������, 
#define GSM_DEBUG_DELAY 1	//���������  �  ��� ������� 

#if LCDXSz==16
	#ifndef GPRS_APN1
		#define GPRS_APN1		"vpni.kyivstar.n"
	#endif
	#ifndef GPRS_APN2
		#define GPRS_APN2 		"et             "
	#endif
	#ifndef GPRS_USER_NAME
		#define GPRS_USER_NAME 	"               "
	#endif
	#ifndef GPRS_PSW
		#define GPRS_PSW 		"               "
	#endif
#elif LCDXSz==20
	#ifndef GPRS_APN1
		#define GPRS_APN1		"vpni.kyivstar.net  "
	#endif
#ifdef LISTENER2
	#ifndef GPRS_LISTENER2INITSTRING
		#error Ne zadana stroka inicializacii!
	#endif
#endif
	#ifndef GPRS_CHECKBALLANCEREQEST
		#define GPRS_CHECKBALLANCEREQEST		"*101#              "
	#endif
	#ifndef GPRS_CURRENCY
		#define GPRS_CURRENCY					" grn               "
	#endif
	#ifndef GPRS_APN2
		#define GPRS_APN2 		"                   "
	#endif
	#ifndef GPRS_USER_NAME
		#define GPRS_USER_NAME 	"                   "
	#endif
	#ifndef GPRS_PSW
		#define GPRS_PSW 		"                   "
	#endif
#elif LCDXSz==40
	#ifndef GPRS_APN
		#define GPRS_APN		"vpni.kyivstar.net            "	//29��������
	#endif
	#ifndef GPRS_USER_NAME
		#define GPRS_USER_NAME 	"                   "	//19��������
	#endif
	#ifndef GPRS_PSW
		#define GPRS_PSW 		"                   "	//19��������
	#endif
#endif

#ifndef VEGA_SN
	#define VEGA_SN 0
#endif
uint16_t Vega_SN EEMEM = VEGA_SN;

#if LCDXSz==40
	uint8_t GPRS_apn[30] EEMEM 				= GPRS_APN;
	uint8_t GPRS_apn_user_name[20] EEMEM	= GPRS_USER_NAME;
	uint8_t GPRS_apn_psw[20] EEMEM 			= GPRS_PSW;
#else
	uint8_t GPRS_apn1[LCDXSz] EEMEM 				= GPRS_APN1;
	uint8_t GPRS_apn2[LCDXSz] EEMEM 				= GPRS_APN2;
	//uint8_t GPRS_listener2initString[LCDXSz] EEMEM 	= GPRS_LISTENER2INITSTRING;
	uint8_t GPRS_checkballancerequest[LCDXSz] EEMEM = GPRS_CHECKBALLANCEREQEST;
	uint8_t GPRS_currency[LCDXSz] EEMEM 			= GPRS_CURRENCY;
	uint8_t GPRS_apn_user_name[LCDXSz] EEMEM		= GPRS_USER_NAME;
	uint8_t GPRS_apn_psw[LCDXSz] EEMEM 				= GPRS_PSW;
#endif

//URC
prog_char URC_RDY[]	= "RDY";
prog_char URC_CFUN[] = "+CFUN: 1";
prog_char URC_CPIN_READY[] = "+CPIN: READY";
prog_char URC_CALL_READY[] = "Call Ready";
prog_char URC_SMS_READY[] = "SMS Ready";
prog_char URC_REMOTE_IP[] = "REMOTE IP";
prog_char URC_CONNECT[] = "CONNECT";
prog_char URC_RECEIVE[] = "+RECEIVE,";
prog_char URC_CLOSED[] = ", CLOSED";
prog_char URC_PDPDEACT[] = "+PDP DEACT";
prog_char URC_NO_CARRIER[] = "NO CARRIER";

// AT-commands RESPONSE
prog_char RESP_OK[] = "OK";
prog_char RESP_CONNECT[]="CONNECT";
prog_char RESP_CONNECT_OK[] = "x, CONNECT OK";
prog_char RESP_SERVER_OK[]	= "SERVER OK";
prog_char RESP_SERVER_CLOSE[] = "SERVER CLOSE";
prog_char RESP_Invitation[] = ">";
prog_char RESP_SEND_OK[] = "x, SEND OK";
prog_char RESP_CLOSE_OK[] = "CLOSE OK";
prog_char RESP_CLOSE_OK_FAST[] = "x, CLOSE OK";
prog_char RESP_CSQ[] = "+CSQ:";
prog_char RESP_SHUT_OK[] = "SHUT OK";

prog_char ESC_SEQ[] = "+++";

prog_char IDENTIFICATION[] = "IDENTIFICATION";
// ~~~~~~~~~~~
// AT-commands
// ~~~~~~~~~~~

// Config
prog_char AT_AT[]			= "AT";
prog_char AT_IPR9600[]		= "AT+IPR=9600";
prog_char AT_W[]			= "AT&W";



// Total
prog_char AT_E0[]			= "ATE0";
prog_char AT_CLCC1[]		= "AT+CLCC=1";
prog_char AT_DDET101[]		= "AT+DDET=1,0,1";
prog_char AT_CGMM[]			= "AT+CGMM";
prog_char AT_CCID[]			= "AT+CCID";
prog_char AT_IFC[]			= "AT+IFC=2,2";
prog_char ATD101[]			= "ATD*101#";

prog_char AT_CUSD101[]		= "AT+CUSD=1,\"";
// SMS
prog_char AT_CMGF[]			= "AT+CMGF=1";
prog_char AT_CSMP[]			= "AT+CSMP=17,167,0,0";

prog_char AT_CMGS[]			= "AT+CMGS=\"+";

// GPRS
prog_char AT_CIPMODE_0[]	= "AT+CIPMODE=0";
prog_char AT_CIPMODE_1[]	= "AT+CIPMODE=1";
prog_char AT_CIPMUX_0[]		= "AT+CIPMUX=0";
prog_char AT_CIPMUX_1[]		= "AT+CIPMUX=1";

prog_char AT_CGATT[]		= "AT+CGATT=1";
//prog_char AT_CIPCSGP[]		= "AT+CIPCSGP=1,\""GPRS_APN"\"\r";
prog_char AT_CIPCSGP[]		= "AT+CIPCSGP=1,\"";
prog_char AT_CSTT[]			= "AT+CSTT";
prog_char AT_CIICR[]		= "AT+CIICR";
prog_char AT_CIFSR[]		= "AT+CIFSR";
prog_char AT_CIPTKA[]       = "AT+CIPTKA=1,30,30,1";

//prog_char AT_CIPSTART[]		= "AT+CIPSTART=0,\"UDP\",";
prog_char AT_CIPSTART[]		= "AT+CIPSTART=\"UDP\",";

prog_char AT_CIPSTART_TCP_CLIENT[]="AT+CIPSTART=\"TCP\",";
prog_char AT_CIPSERVER[]	= "AT+CIPSERVER=1,";
prog_char AT_SERVERCLOSE[]	= "AT+CIPSERVER=0";

prog_char AT_CIPSEND[]		= "AT+CIPSEND=";

prog_char AT_CIPCLOSE_1[]	= "AT+CIPCLOSE=x,1";

prog_char AT_CIPCLOSE[]		= "AT+CIPCLOSE=1";	//������������ ������ ������� ����� �������� TCP/IP �����������
prog_char AT_CIPSHUT[]		= "AT+CIPSHUT";
prog_char AT_CGATT_0[]		= "AT+CGATT=0";

prog_char AT_CSQ[]			= "AT+CSQ";
prog_char ATO[]				= "ATO";

extern uint8_t WebSession;
uint8_t CommandModeDestination,GSM_ReStartN;
int32_t Ballance;
uint8_t numOfCheckBallanceRetry=0;
// Auto
enum {
	GSM_PowerOn,//0
	GSM_WAIT_RDY,//1
	GSM_SEND_CGMM,//2
 	GSM_WAIT_CGMM_READ,//3
	GSM_WAIT_CFUN,//4
	GSM_WAIT_CPIN_READY,//5
	GSM_WAIT_CALL_READY,//6
	GSM_WAIT_SMS_READY,//7

	GSM_SEND_E0, 		GSM_WAIT_E0_OK,		//8,9
	GSM_SEND_CLCC1,		GSM_WAIT_CLCC_OK,	//10,11
	GSM_SEND_DDET101,	GSM_WAIT_DDET101_OK,//12,13

	GSM_IDLE,//14

	GSM_SEND_CMGF,		GSM_WAIT_CMGF_OK,
	GSM_SEND_CSMP,		GSM_WAIT_CSMP_OK,
	GSM_SEND_CIPMUX, 	GSM_WAIT_CIPMUX_OK,
	GSM_SEND_CIPMODE, 	GSM_WAIT_CIPMODE_OK,

	GSM_WAIT_1,

	GSM_SEND_CGATT,		GSM_WAIT_CGATT_OK,
	GSM_SEND_CIPCSGP,	GSM_WAIT_CIPCSGP_OK,
		GSM_SEND_CIPTKA,	GSM_WAIT_CIPTKA_OK,
	GSM_SEND_CSTT,		GSM_WAIT_CSTT_OK,
	GSM_SEND_CIICR,		GSM_WAIT_CIICR_OK,

	GSM_SEND_CIFSR,		GSM_WAIT_CIFSR_OK,


	GSM_SEND_CIPSTART_TCP_CLIENT,		GSM_WAIT_CIPSTART_TCP_CLIENT_OK,	GSM_WAIT_CIPSTART_TCP_CLIENT_CONNECT,//32,33,34

	GSM_WAIT_IDENTIFICATION,			GSM_AnalyzeURC,//35,36


	GSM_SEND_CMGS,			GSM_WAIT_SMS_Inv,			GSM_SEND_SMS_DATA, 			GSM_WAIT_CMGS_OK,//37,38,39,40



	GSM_CIPSHUT_, GSM_ShutWait_, GSM_SEND_CGATT0, GSM_WAIT_CGATT0_OK,

	GSM_CMGS, GSM_SMS,

	GSM_ReStart1, GSM_ReStart2,

	GSM_SEND_ATD_Check_Ballance, GSM_WAIT_ATD_Check_Ballance_OK, GSM_WAIT_Ballance,

	GSM_SEND_CSQ, 	GSM_WAIT_CSQ, GSM_WAIT_CSQ_OK,
	
	GSM_ProtocolMode, GSM_DataMode,	GSM_Swtch2CommandMode,	GSM_Swtch2CommandModePlus,	GSM_WAIT_D2C_SWITCH_OK,	

	GSM_Return2dataMode, GSM_WAIT_R2D_OK, GSM_WAIT_R2D_CONNECT

};

// Config
enum {
	GSM_CFG_START,
	GSM_CFG_SEND_AT, 		GSM_CFG_WAIT_AT_OK,
	GSM_CFG_SEND_IPR,		GSM_CFG_WAIT_IPR_OK,
	GSM_CFG_SEND_ATW,		GSM_CFG_WAIT_ATW_OK,
	GSM_SAVE_IPR,			GSM_FINISH 
};

volatile uint8_t GSM_State;
uint8_t CallerStatus=0;

volatile uint8_t GSM_Control;	// ����� ���������� �������
	#define GSM_CNTL_AUTO	0	// ������� (�� ���������)
	#define GSM_CNTL_MANUAL	1	// ������� (����� �������)
	#define GSM_CNTL_CONFIG	2	// ��������� ������������ (��������� ������� UART ������)
		
volatile uint8_t GSM_Flag = 0b00000001;
	#define flg_TxCStr	0	//�������� ������ ���������


volatile uint8_t RxBufOverFlow,dataSendFlg=0;

#if defined (__AVR_ATmega16__) ||  defined (__AVR_ATmega32__)
	#define GPRS_Out_MaxSz	(10)
	#define GPRS_In_MaxSz	(10)
#elif defined (__AVR_ATmega128__) || defined (__AVR_ATmega2561__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)  || (__AVR_ATxmega128A1__)
	#define GPRS_Out_MaxSz	(0xFF)
	#define GPRS_In_MaxSz	(50)
#endif

static uint8_t TD_GSM, TD_TCP_Connect,  TD_GSM_Reset, TD_5min_timer, TD_1hour_resetGPRS_timer, TD_CheckModem, TD_CleanFIFO, TD_data_start_delay, TD_timer_after_page_transfer;

#define GSM_RXSTR_SIZE 100	// ������������ ����� ������ (�� ������!) �� ������
char GSM_RxStr[GSM_RXSTR_SIZE];
volatile uint8_t GSM_RxCharN, GSM_TxCharN;

char GSM_TxStr[0xFF];

uint8_t GPRS_Data_Out[GPRS_Out_MaxSz], GPRS_Data_In[GPRS_In_MaxSz];
char SMS_Data_Out[100];
uint8_t GPRS_FlgSz_Out, GPRS_FlgSz_In, SMS_FlgSz_Out;
volatile uint8_t GSMTxSz;
volatile uint8_t GSM_Tx;
uint8_t GPRS_Protocol_Out;
	#define UDP 0
	#define TCP 1
uint8_t IP_SendMask;	// ��������� ��������� ���� ��� ���������� : 0bit-UDP IP1, 1bit-UDP IP2, 2bit-UDP IP3, 3bit-UDP IP4, 
uint8_t TempNum;
uint8_t GSM_Temp;
uint8_t GSM_RSSI, GSM_BER;	//RSSI - recieved signal strengh indicator BER - bit error rate
uint8_t GSM_MultiCon;

// ~~~~~~~~~~~
//FIFO
#define GSM_RX_FIFO_SIZE 100		// ��� �������� �� 5�� ScanCycle � GPRS_In_MaxSz =< 50
uint8_t GSM_RX_FIFO[GSM_RX_FIFO_SIZE];
volatile uint8_t GSM_RX_FIFO_Begin;	//������ ���������� ��������� �����
uint8_t GSM_RX_FIFO_End;	//������ ���������� ����������� �����
volatile uint8_t GSM_RX_FIFOOverFlow;
volatile uint8_t GSM_RX_FIFOMax;

// ~~~~~~~~~~~
IP_Addr GSM_MyIP;
IP_Addr GSM_ClientIP;
#define NO_CONNECTION 255
uint8_t GSM_ActiveConnection = NO_CONNECTION;


#ifndef UDP_ServerIP_Init
	#define UDP_ServerIP_Init { \
	 {194,176,97,118},	\
	{0,0,0,0},	\
	{0,0,0,0},	\
	{0,0,0,0}}
#endif

#ifdef ERGOMERA
	#define TCP_ServerIP_Init { \
		 {94,153,208,46},	\
		{0,0,0,0},	\
		{0,0,0,0},	\
		{0,0,0,0}}
	#define TCP_ServerPort_Init	{555,0,0,0}
#endif
#ifndef TCP_ServerIP_Init
	#define TCP_ServerIP_Init { \
	 {194,176,97,118},	\
	{0,0,0,0},	\
	{0,0,0,0},	\
	{0,0,0,0}}
#endif
#ifndef TCP_ServerPort_Init
	#define TCP_ServerPort_Init	{2030,0,0,0}
#endif
IP_Addr UDP_ServerIP[4] EEMEM= UDP_ServerIP_Init;

IP_Addr TCP_ServerIP[4] EEMEM= TCP_ServerIP_Init;

#ifndef UDP_ServerPort_Init
	#define UDP_ServerPort_Init	{2021,0,0,0}
#endif

//#define TCP_ServerPort_Init	{555,0,0,0}

#ifndef TCP_ServerPort_Init
	#define TCP_ServerPort_Init	{555,0,0,0}
#endif
uint16_t UDP_ServerPort[4] EEMEM = UDP_ServerPort_Init;
uint16_t TCP_ServerPort[4] EEMEM = TCP_ServerPort_Init;

#ifndef TCP_ListenPort_Init
	#define TCP_ListenPort_Init	2020
#endif
uint16_t TCP_ListenPort EEMEM = TCP_ListenPort_Init;

#ifndef TCP_ClientIP_Init
	#define TCP_ClientIP_Init { \
	{194,176,97,118},	\
	{176,36,14,205},	\
	{0,0,0,0},	\
	{0,0,0,0}}
#endif
IP_Addr TCP_ClientIP[4] EEMEM = TCP_ClientIP_Init;

uint16_t TCP_CONNECT_timeout EEMEM = 7500;

//---SMS
#ifndef MaxTelephN
	#define MaxTelephN 13	//+1 for \0 end of string
#endif
#ifndef MaxTelephDirSz
	#define MaxTelephDirSz 3
#endif

#ifndef SMS_NUMBER_INIT
	#define SMS_NUMBER_INIT {"000000000000", "000000000000", "000000000000"}
	#define CALL_NUMBER_INIT {"380633232300", "000000000000", "000000000000"}
#endif
char SMS_Number[MaxTelephDirSz][MaxTelephN] EEMEM = SMS_NUMBER_INIT;
char CALL_Number[MaxTelephDirSz][MaxTelephN] EEMEM = CALL_NUMBER_INIT;


uint8_t SMS_On EEMEM = 0;

uint8_t AppProtocol;
	#define _HTTP 1
	#define _VFCP 2

void GSM_SendFirstChar(void);

// ~~~~~~~~~~~
#ifdef GSM_DEBUG
	uint8_t GPRS_DebugStr[LCDXSz];
	uint8_t GPRS_RxTerminalPtr;
	char GPRS_RxTerminalStr[20][LCDXSz];

	void ShiftDown(void){
		for (uint8_t j=19; j>0; j--)
			for (uint8_t i=0; i<LCDXSz; i++)
				GPRS_RxTerminalStr[j][i] = GPRS_RxTerminalStr[j-1][i];
		for (uint8_t i=0; i<LCDXSz; i++)
			GPRS_RxTerminalStr[0][i] = ' ';	
	}

	void GPRS_TerminalProcess(char Char){
		switch(Char){
			case 0x0D://CR - carridge return 0x0D
				GPRS_RxTerminalPtr = 0;
				break;
			case 0x0A://LF - line feed 0x0A
				ShiftDown();
				break;
			default:
				if(GPRS_RxTerminalPtr>=LCDXSz){
					ShiftDown();
					GPRS_RxTerminalPtr = 0;	
				}
				if(GPRS_RxTerminalPtr<LCDXSz)
					if ( isprint(Char) ||  ( (0xC0<=Char) && (Char<=0xFF) ) ||
						Char==0xB2 || Char==0xB3 || Char==0xA8 || Char==0xB8 || Char==0xAA || Char==0xBA || Char==0xAF || Char==0xBF)
							GPRS_RxTerminalStr[0][GPRS_RxTerminalPtr++] = Char;
		}
	}
	
	void GPRS_RunDebugStr(void){
		if(GSM_Flag & (1<<flg_TxCStr)){		
			uint8_t i=0;
			while ( (i<LCDXSz-1) && (GPRS_DebugStr[i]!=' ')){
				GSM_TxStr[i] = GPRS_DebugStr[i];
				i++;
			}
			GSM_TxStr[i] = '\r';
			GSMTxSz = i+1;
			GSM_SendFirstChar();
			memset(GPRS_DebugStr,' ',sizeof(GPRS_DebugStr)-1);
		}
	}
#endif
// ~~~~~~~~~~~
void InitFIFO(void){
	GSM_RX_FIFO_Begin = 255;
	GSM_RX_FIFO_End = 255;
	GSM_RxCharN = 0;
	GSM_RX_FIFOOverFlow = 0;
	GSM_RX_FIFOMax = 0;
}
// ~~~~~~~~~~~
void GetByteFromFIFO(uint8_t *ReadBuf, uint8_t *Index, uint8_t ReadBuf_Sz){

	//Reset *Index if read buffer overflow
	if(*Index >= ReadBuf_Sz){
		*Index = 0;
		//TODO ���������� ����
	}	

	if(GSM_RX_FIFO_Begin > GSM_RX_FIFO_End){
		if((GSM_RX_FIFO_End != 255)&&(GSM_RX_FIFO_End != 255))			
			if(GSM_RX_FIFOMax < (GSM_RX_FIFO_Begin - GSM_RX_FIFO_End))
				GSM_RX_FIFOMax =  GSM_RX_FIFO_Begin - GSM_RX_FIFO_End;
		GSM_RX_FIFO_End++;
		ReadBuf[*Index] = GSM_RX_FIFO[GSM_RX_FIFO_End];
		*Index=*Index+1;
	}
	if(GSM_RX_FIFO_End > GSM_RX_FIFO_Begin){
		if((GSM_RX_FIFO_End != 255)&&(GSM_RX_FIFO_End != 255))
			if(GSM_RX_FIFOMax < (GSM_RX_FIFO_SIZE - GSM_RX_FIFO_End  + GSM_RX_FIFO_Begin))
				GSM_RX_FIFOMax = GSM_RX_FIFO_SIZE - GSM_RX_FIFO_End  + GSM_RX_FIFO_Begin;
		GSM_RX_FIFO_End++;
		if(GSM_RX_FIFO_End >= GSM_RX_FIFO_SIZE) GSM_RX_FIFO_End = 0;
		ReadBuf[*Index] = GSM_RX_FIFO[GSM_RX_FIFO_End];
		*Index=*Index+1;
	}
}
// ~~~~~~~~~~~
// ���������� ����� ������ � ������ ������� ���������� ������
uint8_t GetStringFromFIFO(void){

	uint8_t StrLength = 0;

	while(GSM_RX_FIFO_Begin != GSM_RX_FIFO_End){
		
		GetByteFromFIFO((uint8_t *)GSM_RxStr, (uint8_t *)&GSM_RxCharN, GSM_RXSTR_SIZE);

		//��� ����� ��������� ������: xxxxxxxxxxxxxxxx<CR><LF>
		//	'\r' CR - carridge return 0x0D
		//	'\n' LF - line feed 0x0A
		//����������� � ������ xxxxxxxxxxxxxxxx<\0>
		if( (GSM_RxCharN >= 2) && (GSM_RxStr[GSM_RxCharN-2]== '\r') && (GSM_RxStr[GSM_RxCharN-1]== '\n') ){
			GSM_RxStr[GSM_RxCharN-2] = '\0';
			StrLength = GSM_RxCharN-1;
			GSM_RxCharN = 0;
			// Ignore GSM_RxStr like (0x0D 0x0A)
			return StrLength;
		}

	}//while
	
	return StrLength;
}

uint8_t GetStringFromFIFOwithOverflowDetect(void){

	uint8_t StrLength = 0;

	while(GSM_RX_FIFO_Begin != GSM_RX_FIFO_End){
		
		GetByteFromFIFO((uint8_t *)GSM_RxStr, (uint8_t *)&GSM_RxCharN, GSM_RXSTR_SIZE);

		//��� ����� ��������� ������: xxxxxxxxxxxxxxxx<CR><LF>
		//	'\r' CR - carridge return 0x0D
		//	'\n' LF - line feed 0x0A
		//����������� � ������ xxxxxxxxxxxxxxxx<\0>
		if( ((GSM_RxCharN >= 2) && (GSM_RxStr[GSM_RxCharN-2]== '\r') && (GSM_RxStr[GSM_RxCharN-1]== '\n'))||(GSM_RxCharN==GSM_RXSTR_SIZE-1) ){
			GSM_RxStr[GSM_RxCharN-2] = '\0';
			StrLength = GSM_RxCharN-1;
			GSM_RxCharN = 0;
			// Ignore GSM_RxStr like (0x0D 0x0A)
			return StrLength;
		}

	}//while
	
	return StrLength;
}
// ~~~~~~~~~~~
// ������������� ��������� ������ GSM_RxStr ��������� <CR><LF>
uint8_t ForceEndStringFromFIFO(void){
	uint8_t StrLength;
	if(GSM_RxCharN > GSM_RXSTR_SIZE-1) GSM_RxCharN = GSM_RXSTR_SIZE-1;
	StrLength = GSM_RxCharN;
	GSM_RxStr[GSM_RxCharN]='\0';
	GSM_RxCharN = 0;
	return StrLength;
}

// ~~~~~~~~~~~
uint8_t GetDataFromFIFO(const uint8_t Amount){

	while(GSM_RX_FIFO_Begin != GSM_RX_FIFO_End){
		GetByteFromFIFO((uint8_t *)GSM_RxStr, (uint8_t *)&GSM_RxCharN, GSM_RXSTR_SIZE);
		if(GSM_RxCharN >= Amount){
			GSM_RxCharN = 0;
			return Amount;
		}
	}//while
	return GSM_RxCharN;
}
// ~~~~~~~~~~~
// �������� �� ������ EEPROM � ������ RAM �� ������� ������� ��� '\0' ��� ������ ������ EEPROM
uint8_t strcpy_EE(char* str_RAM, uint8_t* str_EE){
	uint8_t i=0;
	while( (erb(&str_EE[i])!='\0') && (erb(&str_EE[i])!=' ') ){
		str_RAM[i] = (char)erb(&str_EE[i]);
		i++;
	}
	return i;	
}
// ~~~~~~~~~~~
void
GSM_Init(void)
{	
	InitFIFO();
	#ifdef GSM_DEBUG
		memset(GPRS_RxTerminalStr,' ',sizeof(GPRS_RxTerminalStr)-1);
		memset(GPRS_DebugStr,' ',sizeof(GPRS_DebugStr)-1);
	#endif

	#if defined (__AVR_ATxmega128A1__)
		// USARTE0
		PORTE.OUT |=(1<<3);
		PORTE.DIR |=(1<<3);
		USART_GSM.BAUDCTRLA = 0x67;	// 9600
		USART_GSM.BAUDCTRLB = 0x00 + USART_BSCALE0_bm;
		USART_GSM.CTRLA = USART_RXCINTLVL_HI_gc;
		USART_GSM.CTRLB = (1<<USART_RXEN_bp) | (1<<USART_TXEN_bp) | (0<<USART_CLK2X_bp) | (0 << USART_MPCM_bp) | (0 << USART_TXB8_bp);
		USART_GSM.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | (0<< USART_SBMODE_bp) | USART_CHSIZE_8BIT_gc;
	#else
		UCSR_GSM_A = ~(1<<U2X_GSM) &~(1<<MPCM_GSM);
		UCSR_GSM_B =  (1<<RXCIE_GSM) | (0<<TXCIE_GSM) | (0<<UDRIE_GSM) | (1<<RXEN_GSM) | (0<<UCSZ_GSM_2) | (1<<TXEN_GSM);
		#if defined (__AVR_ATmega2561__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)			
			UCSR_GSM_C = (0<<UMSEL_GSM_1) | (0<<UMSEL_GSM_0) | (0<<UPM_GSM_1) | (0<<UPM_GSM_0) | (0<<USBS_GSM) | (1<<UCSZ_GSM_1) | (1<<UCSZ_GSM_0) | (0<<UCPOL_GSM);
		#else
			UCSR_GSM_C = ((~(1<<UMSEL_GSM) &~(1<<UPM_GSM_1) &~(1<<UPM_GSM_0) &~(1<<USBS_GSM)) |(1<<UCSZ_GSM_1) |(1<<UCSZ_GSM_0)) &~(1<<UCPOL_GSM);
		#endif
		UBRR_GSM_H = 0x00;
		UBRR_GSM_L = 0x67;	// 9600
	#endif

	GSM_PWRCNTRL_Config();
	GSM_PWRCNTRL_ON();
	#ifdef GSM_HW_FLOW_CONTROL
		GSM_CTS_RTS_Config();
	#endif

	TD_GSM = Timer16SysAlloc(1);
	TD_TCP_Connect = Timer16SysAlloc(1);
	TD_GSM_Reset = Timer32SysAlloc(1);
	TD_5min_timer = Timer16SysAlloc(1);
	TD_1hour_resetGPRS_timer = Timer32SysAlloc(1);
	TD_CheckModem = Timer16SysAlloc(1);
	TD_CleanFIFO = Timer16SysAlloc(1);
	TD_data_start_delay = Timer16SysAlloc(1);
	TD_timer_after_page_transfer = Timer16SysAlloc(1);

/*	for (uint8_t i=0; i<4; i++)
		GPRS_ServerAddr[i] = erb(GPRS_ServerAddr_EE+i);
*/
	GSM_Control = GSM_CNTL_AUTO;
	GSM_State = GSM_PowerOn;



}
// ~~~~~~~~~
void
GSM_RX(void)
{
	char Char = UDR_GSM;

	//---FIFO
	GSM_RX_FIFO_Begin++;
	if(GSM_RX_FIFO_Begin >= GSM_RX_FIFO_SIZE) GSM_RX_FIFO_Begin = 0;
	GSM_RX_FIFO[GSM_RX_FIFO_Begin] = Char;
	if(GSM_RX_FIFO_Begin == GSM_RX_FIFO_End){
		GSM_RX_FIFOOverFlow = 1;
		GSM_RX_FIFOMax = GSM_RX_FIFO_SIZE;
	}
	#ifdef GSM_DEBUG
		GPRS_TerminalProcess(Char);
	#endif

}
// ~~~~~~~~~
//�������� ������ ����� GetStringFromFIFO()!
uint8_t GSM_Wait_Response(char *Str, uint8_t RestoreCMD){
//	if(Timer16Stopp(TD_GSM) && RestoreCMD){
//		GSM_State = RestoreCMD;
//		return 0;
//	}
	if(strcmp(GSM_RxStr, Str))return 0;
	else return 1;
}
// ~~~~~~~~~
uint8_t GSM_Wait_Response_P(prog_char *Str_P, uint8_t RestoreCMD){
	
	if(Timer16Stopp(TD_GSM) && RestoreCMD){
		GSM_State = RestoreCMD;
		return 0;
	}
	//������ �������� ������
	if(GetStringFromFIFO()){
		if(strcmp_P(GSM_RxStr, Str_P)) return 0;
		else return 1;
	}
	else return 0;
}
// ~~~~~~~~~
uint8_t GSM_Wait_Char(char Char, uint8_t RestoreCMD){
	if(Timer16Stopp(TD_GSM)){
		GSM_State = RestoreCMD;
		return 0;
	}
	//������ ������� ��������� �����
	if(GetDataFromFIFO(1)){
		if(GSM_RxStr[0] == Char)	return 1;
		else return 0;
	}
	else return 0;
}
// ~~~~~~~~~
void GSM_SendFirstChar(void){

	UDR_GSM = GSM_TxStr[0];
	#ifdef GSM_DEBUG
		GPRS_TerminalProcess(GSM_TxStr[0]);
	#endif

	GSM_TxCharN = 1;
	cli();
	GSM_Flag &=~(1<<flg_TxCStr);	
	sei();
	cli();
	#if defined (__AVR_ATxmega128A1__)
		USART_GSM.CTRLA = USART_RXCINTLVL_HI_gc | USART_DREINTLVL_HI_gc;
	#else
		UCSR_GSM_B =  (1<<RXCIE_GSM) | (0<<TXCIE_GSM) | (1<<UDRIE_GSM) | (1<<RXEN_GSM) | (0<<UCSZ_GSM_2) | (1<<TXEN_GSM);
	#endif
	
	sei();	
}
// ~~~~~~~~~
//���������� �������� ������ + CR
void GSM_SendCR(prog_char *Str_P){
	GSMTxSz = strlen_P(Str_P);
	sprintf_P(GSM_TxStr, Str_P);
	sprintf(GSM_TxStr + GSMTxSz, "\r");
	GSMTxSz++;
	GSM_SendFirstChar();
}
// ~~~~~~~~~
//���������� �������� ������ + CntrZ
void GSM_AddHeaderSendData(uint8_t *Data_Out, uint8_t Size){
	GSMTxSz = Size;
	memcpy(GSM_TxStr, Data_Out, GSMTxSz);
	GSM_SendFirstChar();
}
// ~~~~~~~~~
void GSM_SendData(uint8_t *Data_Out, uint8_t Size){
	GSMTxSz = Size;
	memcpy(GSM_TxStr, Data_Out, GSMTxSz);
	GSM_SendFirstChar();
}
// ~~~~~~~~~
void GSM_Execute_Command(prog_char *Str_P, uint16_t Timeout){
	if(GSM_Flag & (1<<flg_TxCStr)){
		GSM_SendCR(Str_P);
		StartTimer16(TD_GSM, Timeout);
	}
}
// ~~~~~~~~~
void GSM_DRE(void){
	if(GSM_TxCharN < GSMTxSz){
		UDR_GSM = GSM_TxStr[GSM_TxCharN];
		#ifdef GSM_DEBUG
			GPRS_TerminalProcess(GSM_TxStr[GSM_TxCharN]);
		#endif
		GSM_TxCharN++;
	}
	else{
		#if defined (__AVR_ATxmega128A1__)
			USART_GSM.CTRLA = USART_RXCINTLVL_HI_gc | USART_TXCINTLVL_HI_gc;
		#else
			UCSR_GSM_B =  (1<<RXCIE_GSM) | (1<<TXCIE_GSM) | (0<<UDRIE_GSM) | (1<<RXEN_GSM) | (0<<UCSZ_GSM_2) | (1<<TXEN_GSM);
		#endif
	}
}
// ~~~~~~~~~
void
GSM_TX(void)
{
	GSM_Flag |=(1<<flg_TxCStr);
	if(dataSendFlg){
	dataSendFlg=0;
	GPRS_FlgSz_Out=0;
	}
	#if defined (__AVR_ATxmega128A1__)
		USART_GSM.CTRLA = USART_RXCINTLVL_HI_gc;
	#else
		UCSR_GSM_B =  (1<<RXCIE_GSM) | (0<<TXCIE_GSM) | (0<<UDRIE_GSM) | (1<<RXEN_GSM) | (0<<UCSZ_GSM_2) | (1<<TXEN_GSM);
	#endif
}

// ~~~~~~~~~~~~
void
GSM_Reset(void)
{
/*	IntOff();
	GSM_State = GSM_Restart;
	IntOn();*/
}

uint8_t GSM_GotoNextVega(void){
 return 0;
}
uint8_t counter=0;

// ~~~~~~~~~~~~~~~~~~~~~~~~
inline static void GSM_Auto(){

	switch(GSM_State){

		case GSM_PowerOn:
			StartTimer16(TD_GSM,1000*GSM_DEBUG_DELAY);
			StartTimer32(TD_GSM_Reset, 24UL*60UL*6000);	// ���������� ������ ������ 24� ���
			StartTimer32(TD_1hour_resetGPRS_timer, 360000); //��� � ��� ������������ � ����������
			StartTimer16(TD_CheckModem,6000);// �������� ������ ������� ��� � 5 �����	
			GSM_PWRCNTRL_ON();
			InitFIFO();
			counter=0;
			GSM_Modem = NOT_RECOGNIZED;
			GSM_State = GSM_WAIT_RDY;
			break;

		case GSM_WAIT_RDY:
			if(GSM_Wait_Response_P(URC_RDY, GSM_ReStart1)) GSM_State++;
			break;
		case GSM_SEND_CGMM:
			GSM_Execute_Command(AT_CGMM,5000*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_CGMM_READ:
			if(Timer16Stopp(TD_GSM)) GSM_State = GSM_ReStart1;
			if(GetStringFromFIFO()){
				if(!strcmp_P(GSM_RxStr, GSM_SIMCOM_SIM900R)) GSM_Modem = SIMCOM_SIM900R;
				if(!strcmp_P(GSM_RxStr, GSM_SIMCOM_SIM800)) GSM_Modem = SIMCOM_SIM800;
				if(!strcmp_P(GSM_RxStr, GSM_SIMCOM_SIM800C)) GSM_Modem = SIMCOM_SIM800;
				//Add here new model GSM-modem
				if(GSM_Modem != NOT_RECOGNIZED) GSM_State = GSM_WAIT_CALL_READY;
			}
		case GSM_WAIT_CFUN:
			if(GSM_Wait_Response_P(URC_CFUN, GSM_ReStart1)) GSM_State++;
			break;
		case GSM_WAIT_CPIN_READY:
			if(GSM_Wait_Response_P(URC_CPIN_READY, GSM_ReStart1)) GSM_State++;
			break;

		case GSM_WAIT_CALL_READY:
			if(GSM_Wait_Response_P(URC_CALL_READY, GSM_ReStart1)){
				if(GSM_Modem == SIMCOM_SIM800) GSM_State = GSM_WAIT_SMS_READY;
				else GSM_State = GSM_SEND_E0;
			}
			break;
		case GSM_WAIT_SMS_READY:
			if(GSM_Wait_Response_P(URC_SMS_READY, GSM_ReStart1))GSM_State++;
			break;
		//------------------------
		case GSM_SEND_E0:
			GSM_Execute_Command(AT_E0,100*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_E0_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State++;
			break;
		case GSM_SEND_CLCC1:
			GSM_Execute_Command(AT_CLCC1,100*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_CLCC_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State++;
			break;
		case GSM_SEND_DDET101:
			GSM_Execute_Command(AT_DDET101,100*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_DDET101_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State++;
			break;
		case GSM_IDLE:
			break;


		case GSM_SEND_CMGF:
			GSM_Execute_Command(AT_CMGF,100*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_CMGF_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State++;
			break;
		case GSM_SEND_CSMP:
			GSM_Execute_Command(AT_CSMP, 100*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_CSMP_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State=GSM_SEND_ATD_Check_Ballance;
			break;
		//------------------------
		case GSM_SEND_CIPMUX:
			if(GSM_MultiCon) GSM_Execute_Command(AT_CIPMUX_1, 100*GSM_DEBUG_DELAY);
			else GSM_Execute_Command(AT_CIPMUX_0, 100*GSM_DEBUG_DELAY);
			GSM_State++; 
			break;
		case GSM_WAIT_CIPMUX_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)){
				if(GSM_MultiCon){
					GSM_State = GSM_WAIT_1;
					// between "Call Ready" and "+CMGS"; 8 sec min needs; for kyivstar vpni only
					StartTimer16(TD_GSM, 1000);
				}
				else GSM_State++;
			}
			break;
		case GSM_SEND_CIPMODE:
			if(GSM_MultiCon) GSM_Execute_Command(AT_CIPMODE_0, 100*GSM_DEBUG_DELAY);
			else GSM_Execute_Command(AT_CIPMODE_1, 100*GSM_DEBUG_DELAY);
			GSM_State++;
			break;
		case GSM_WAIT_CIPMODE_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)){
				GSM_State++;
				// between "Call Ready" and "+CMGS"; 8 sec min needs; for kyivstar vpni only
				StartTimer16(TD_GSM, 1000);
			}
			break;

		//------------------------
		case GSM_WAIT_1:
			GetStringFromFIFO();		// ����� �� ���� ������������ FIFO
			if(Timer16Stopp(TD_GSM)){
				GSM_State++;
				GSM_Temp = 0;
			}
			break;
		case GSM_SEND_CGATT:
			GSM_Execute_Command(AT_CGATT, 500*GSM_DEBUG_DELAY); GSM_Temp++; GSM_State++;
			break;
		case GSM_WAIT_CGATT_OK:
			if(GSM_Temp>20) GSM_State = GSM_ReStart1;
			if(GSM_Wait_Response_P(RESP_OK, GSM_SEND_CGATT)) GSM_State++;
			break;

		case GSM_SEND_CIPCSGP:
			//GSM_Execute_Command(AT_CIPCSGP, 300*GSM_DEBUG_DELAY); GSM_State++;
			if(GSM_Flag & (1<<flg_TxCStr)){
				sprintf_P(GSM_TxStr, AT_CIPCSGP);	GSMTxSz = strlen_P(AT_CIPCSGP);
				#if LCDXSz==40
					GSMTxSz += strcpy_EE(&GSM_TxStr[GSMTxSz],GPRS_apn);
				#else
					GSMTxSz += strcpy_EE(&GSM_TxStr[GSMTxSz],GPRS_apn1);
					GSMTxSz += strcpy_EE(&GSM_TxStr[GSMTxSz],GPRS_apn2);
				#endif
				sprintf(GSM_TxStr + GSMTxSz, "\",\"");	GSMTxSz = GSMTxSz+3;				
				GSMTxSz += strcpy_EE(&GSM_TxStr[GSMTxSz],GPRS_apn_user_name);
				sprintf(GSM_TxStr + GSMTxSz, "\",\"");	GSMTxSz = GSMTxSz+3;
				GSMTxSz += strcpy_EE(&GSM_TxStr[GSMTxSz],GPRS_apn_psw);
				sprintf(GSM_TxStr + GSMTxSz, "\"\r");	GSMTxSz = GSMTxSz+2;
				GSM_SendFirstChar();
				StartTimer16(TD_GSM, 300*GSM_DEBUG_DELAY);
			}
			GSM_State++;
			break;
		case GSM_WAIT_CIPCSGP_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)){
			 if(GSM_Modem == SIMCOM_SIM800)GSM_State++;
			 else GSM_State=GSM_SEND_CSTT;
			 }
			break;

		case GSM_SEND_CIPTKA:
			GSM_Execute_Command(AT_CIPTKA, 1000*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_CIPTKA_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State++;
			break;

		case GSM_SEND_CSTT:
			GSM_Execute_Command(AT_CSTT, 300*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_CSTT_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State++;
			break;

		case GSM_SEND_CIICR:
			GSM_Execute_Command(AT_CIICR, 12000*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_CIICR_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State++;
			break;

	

		case GSM_SEND_CIFSR:
			GSM_Execute_Command(AT_CIFSR, 1000*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_CIFSR_OK:
			if(Timer16Stopp(TD_GSM)) GSM_State = GSM_ReStart1;
			if(GetStringFromFIFO()){
				SetIP(&GSM_MyIP,0,0,0,0);
				for(uint8_t i=0; i<strlen(GSM_RxStr); i++)
					if(!isdigit(GSM_RxStr[i])) GSM_RxStr[i] = ' ';
				sscanf_P(GSM_RxStr,PSTR("%hhu %hhu %hhu %hhu"), (unsigned char*)&GSM_MyIP.IP1, (unsigned char*)&GSM_MyIP.IP2, (unsigned char*)&GSM_MyIP.IP3, (unsigned char*)&GSM_MyIP.IP4);
				if(!IP_compare_Const(&GSM_MyIP,0,0,0,0)){
				  GSM_State = GSM_SEND_CIPSTART_TCP_CLIENT;
				}
				TempNum=0;	//
			}			
			break;
		//------------------------
		case GSM_SEND_CIPSTART_TCP_CLIENT:
	
				if(GSM_Flag & (1<<flg_TxCStr)){
					GSMTxSz = strlen_P(AT_CIPSTART_TCP_CLIENT);
					sprintf_P(GSM_TxStr, AT_CIPSTART_TCP_CLIENT);
					//GSM_TxStr[12] = 0x30+TempNum;
					sprintf(GSM_TxStr + GSMTxSz, "\"%u.%u.%u.%u\",\"%u\"\r",(unsigned int)erb(&TCP_ServerIP[0].IP1), (unsigned int)erb(&TCP_ServerIP[0].IP2), 
						(unsigned int)erb(&TCP_ServerIP[0].IP3), (unsigned int)erb(&TCP_ServerIP[0].IP4), (unsigned int)erw(&TCP_ServerPort[0]));
					GSMTxSz = strlen(GSM_TxStr);
					GSM_SendFirstChar();
					StartTimer16(TD_GSM, 500*GSM_DEBUG_DELAY);
					GSM_State++;
				}
		
			break;

		case GSM_WAIT_CIPSTART_TCP_CLIENT_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State++;
			break;
		case GSM_WAIT_CIPSTART_TCP_CLIENT_CONNECT:
			if(GSM_Wait_Response_P(RESP_CONNECT, GSM_ReStart1)){
				 GSM_State = GSM_WAIT_IDENTIFICATION;
				 StartTimer16(TD_GSM, 1000*GSM_DEBUG_DELAY);
				 }
			break;

		//------------------------

		case GSM_WAIT_IDENTIFICATION:
				
		//---� ��� ��������� �������� ����� ��������������
			#ifdef ERGOMERA
			if(counter==0){

				GSMTxSz = strcpy_EE(GSM_TxStr,GPRS_listener2initString);
				GSM_SendFirstChar();
				counter=1;

			}
			#endif	

			if(GetStringFromFIFO()){
				GSM_State = GSM_AnalyzeURC;
				TempNum = 0;
				break;
			}
		/*	if(!IsWebSession()){
				if(Timer16Stopp(TD_GSM)){
					GSM_State = GSM_ReStart1;
					break;
				}
			}*/
		
		
			break;

		case GSM_AnalyzeURC:			//������ �������� ������
			
			//���������� ��������
			if( (strstr_P(GSM_RxStr, URC_CLOSED+2) != NULL) ){
				GSM_State =GSM_ReStart1;
				break;
			}
/*
			if( (strstr_P(GSM_RxStr, IDENTIFICATION) != NULL) ){
				//GSM_SendCR(VMD);
				GSMTxSz = strcpy_EE(GSM_TxStr,GPRS_listener2initString);
				GSM_SendFirstChar();
				StartTimer16(TD_data_start_delay,100);
				GSM_State = GSM_ProtocolMode;
				InitFIFO();
				break;
			}
*/			
			if( (strstr_P(GSM_RxStr, PSTR("OK")) != NULL) ){
				StartTimer16(TD_data_start_delay,100);
				GSM_State = GSM_ProtocolMode;
				InitFIFO();
				break;
			}


			// ������������� ������
			if( (strstr_P(GSM_RxStr, URC_CONNECT) != NULL) ){

				GSM_State = GSM_ProtocolMode;
				for(int i=0;i<GSM_RXSTR_SIZE;i++)
				{
					GSM_RxStr[i]=0;	
				}
				InitFIFO();

				break;
			}

			// ������������ ������
			if(strstr_P(GSM_RxStr, URC_CLOSED) != NULL){
				// ������ ��������
				sscanf_P(GSM_RxStr,PSTR("%hhu, CLOSED"),(unsigned char*)&TempNum);
				if(GSM_ActiveConnection == TempNum){
					GSM_ActiveConnection = NO_CONNECTION;
					SetIP(&GSM_ClientIP,0,0,0,0);
				}

				GSM_State = GSM_ReStart1;
				break;
			}

			// +PDP DEACT
			if( (strstr_P(GSM_RxStr, URC_PDPDEACT) != NULL) ){
				GSM_State = GSM_SEND_E0;
				break;
			}



			// �������� ����� �� ������ �� 
			if(strstr_P(GSM_RxStr, RESP_CSQ) != NULL ){
				GSM_State = GSM_WAIT_CSQ;
				break;
			}

			// �����
			GSM_State = GSM_WAIT_IDENTIFICATION;
			break;
		//------------------------

	
		//------------------------


		//------------------------
		case GSM_CIPSHUT_:
			GSM_Execute_Command(AT_CIPSHUT, 1000*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_ShutWait_:
			if(GSM_Wait_Response_P(RESP_SHUT_OK, GSM_ReStart1)) GSM_State++;
			break;
		case GSM_SEND_CGATT0:
			GSM_Execute_Command(AT_CGATT_0, 1000*GSM_DEBUG_DELAY); GSM_State++;
			break;
		case GSM_WAIT_CGATT0_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)){
				StartTimer16(TD_GSM, 1000);
			 	GSM_State = GSM_SEND_ATD_Check_Ballance;
			}
			break;

		//------------------------
		case GSM_SEND_CMGS:
			if(GSM_Flag & (1<<flg_TxCStr)){			
				erbl(GSM_TxStr, SMS_Number+TempNum, MaxTelephN);
				GSMTxSz = strlen(GSM_TxStr);
				uint8_t j=0;
				for(uint8_t i=0; i<GSMTxSz; i++)
					if(GSM_TxStr[i] != '0') j++;
				if(j){
					GSMTxSz = strlen_P(AT_CMGS);
					sprintf_P(GSM_TxStr, AT_CMGS);
					erbl(GSM_TxStr+GSMTxSz, SMS_Number+TempNum, MaxTelephN);
					GSMTxSz = strlen(GSM_TxStr);
					sprintf(GSM_TxStr + GSMTxSz, "\"\r");
					GSMTxSz = strlen(GSM_TxStr);
					GSM_SendFirstChar();
					StartTimer16(TD_GSM, 500*GSM_DEBUG_DELAY);
					GSM_State++;
				}
				else{
					TempNum++;
					if(TempNum>=MaxTelephDirSz){
						//StartTimer16(TD_GSM,150);

						GSM_State = GSM_Return2dataMode;
				
						SMS_FlgSz_Out = 0;
					}					
				}
			}
			break;
		case GSM_WAIT_SMS_Inv:
			if(GSM_Wait_Char('>', GSM_ReStart1)) GSM_State++;
			break;
		case GSM_SEND_SMS_DATA:
			SMS_Data_Out[SMS_FlgSz_Out] = 0x1A;	// SMS Data end - char Ctrl-Z
			GSM_SendData((uint8_t *)SMS_Data_Out, SMS_FlgSz_Out+1);
			GSM_State++;
			break;		
		case GSM_WAIT_CMGS_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)){
				if(++TempNum>=MaxTelephDirSz){
					GSM_State = GSM_Return2dataMode;
					SMS_FlgSz_Out = 0;
					//StartTimer16(TD_GSM,150);
				}
				else GSM_State = GSM_SEND_CMGS;
			}			
			break;

		//----������� � data mode
		case GSM_Return2dataMode:

			CommandModeDestination=0;
			InitFIFO();
			for(int i=0;i<GSM_RXSTR_SIZE;i++)
			{
				GSM_RxStr[i]=0;	
			}
			GSM_Execute_Command(ATO, 600*GSM_DEBUG_DELAY); //GSM_State++;
			GSM_State=GSM_WAIT_R2D_CONNECT;
		
			break;

		case GSM_WAIT_R2D_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) GSM_State++;
			break;

		case GSM_WAIT_R2D_CONNECT:
			if(GSM_Wait_Response_P(RESP_CONNECT, GSM_ReStart1)){ 				
				GSM_State=GSM_ProtocolMode;
				StartTimer16(TD_timer_after_page_transfer,100);
				InitFIFO();
			}
			break;
		//------------------------		

		//------------------------

		
		//------------------------
		case GSM_ReStart1:
			GSM_ReStartN++;
			GSM_PWRCNTRL_OFF();
			StartTimer16(TD_GSM,1000);
			RxBufOverFlow = 0;
		//	WebSession=0;
			InitFIFO();
			for(int i=0;i<GSM_RXSTR_SIZE;i++)
			{
				GSM_RxStr[i]=0;	
			}
			SMS_FlgSz_Out = 0;	// �.�. ���� ������������ ���� �� ���� ����� ����� �������� ���������� - ����� �����
			GSM_ActiveConnection = NO_CONNECTION;
			SetIP(&GSM_ClientIP,0,0,0,0);
			GSM_Flag |=(1<<flg_TxCStr);
			GSM_State++;
			break;
		case GSM_ReStart2:
			if(Timer16Stopp(TD_GSM)){
				GSM_State = GSM_PowerOn;
			}
			break;
		//------------------------ �������� ����� �� �����


		case GSM_SEND_ATD_Check_Ballance:	
			if(GSM_Flag & (1<<flg_TxCStr)){
				sprintf_P(GSM_TxStr, AT_CUSD101);	
				GSMTxSz = strlen_P(AT_CUSD101);
				GSMTxSz += strcpy_EE(GSM_TxStr+GSMTxSz,GPRS_checkballancerequest);
				GSM_TxStr[GSMTxSz]='"';
				GSM_TxStr[GSMTxSz+1]='\r';
				GSM_TxStr[GSMTxSz+2]='\n';
				GSMTxSz+=3;
				GSM_SendFirstChar();
				StartTimer16(TD_GSM, 1000);
			}
			GSM_State++;	
			GSM_Temp = 0;
			break;
		case GSM_WAIT_ATD_Check_Ballance_OK:
			if(GSM_Wait_Response_P(RESP_OK,GSM_SEND_CSQ)) GSM_State++;
			break;
		case GSM_WAIT_Ballance:
			if(Timer16Stopp(TD_GSM)) GSM_State = GSM_SEND_CSQ;
			GSM_RxStr[GSM_RXSTR_SIZE-1]='\0';
			while(GSM_RX_FIFO_Begin != GSM_RX_FIFO_End){
				
		
				GetByteFromFIFO((uint8_t *)GSM_RxStr, (uint8_t *)&GSM_RxCharN, GSM_RXSTR_SIZE);

				if(GSM_RxCharN >= 90){

					GSM_State = GSM_SEND_CSQ;

				}

			}//while
			char buf[20];
			uint8_t i=0;
			while( (erb(&GPRS_currency[i])!='\0') && ((erb(&GPRS_currency[i])!=' ')||i==0) ){
				buf[i] = (char)erb(&GPRS_currency[i]);
				i++;
			}
			buf[i]='\0';
			char *istr=strstr(GSM_RxStr, buf);
			if(istr)
			{
					Ballance=*(istr-1)-0x30;
					Ballance+=(*(istr-2)-0x30)*10;
					uint8_t charnum=4;
					uint32_t razr=100;
					while((*(istr-charnum)>=0x30)&&(*(istr-charnum)<=0x39)){
						Ballance+=(*(istr-charnum)-0x30)*razr;
						razr*=10;
						charnum++;	
					}
					if(*(istr-charnum)==0x2D)
					{
						Ballance*=-1;
					}

			}	
			break;

		//------------------------ �������� ������ GSM-�������
		case GSM_SEND_CSQ:
				GSM_Execute_Command(AT_CSQ, 500*GSM_DEBUG_DELAY); 
				InitFIFO();
				GSM_State++;
			break;
		case GSM_WAIT_CSQ:
				if(Timer16Stopp(TD_GSM)){
						GSM_State = GSM_ReStart1;
				}
				else{				
					if(GetStringFromFIFO()){
						GSM_RSSI = 255; GSM_BER = 255;
						if(strstr_P(GSM_RxStr, RESP_CSQ) != NULL ){
							sscanf_P(GSM_RxStr,PSTR("+CSQ: %hhu,%hhu"), (unsigned char*)&GSM_RSSI, (unsigned char*)&GSM_BER);
									
								GSM_State = GSM_WAIT_CSQ_OK;
						
						}
					}
				}
			break;
		case GSM_WAIT_CSQ_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) {
				if(CommandModeDestination==5){
					GSM_State  = GSM_Return2dataMode;
				}
				else{
					GSM_State=GSM_SEND_CIPMUX;
				}
			}
			break;
		//------------------------

		//-----------

		case GSM_ProtocolMode:
			if( (strstr_P(GSM_RxStr, URC_CLOSED+2) != NULL) ){
					GSM_State =GSM_ReStart1;
					break;
			}



			while(GSM_RX_FIFO_Begin != GSM_RX_FIFO_End){	
					GetByteFromFIFO((uint8_t *)GSM_RxStr, (uint8_t *)&GSM_RxCharN, GSM_RXSTR_SIZE);
					if(GSM_RxCharN==2)
						{
							if(!strncmp_P(GSM_RxStr, PSTR("\r\n"), 2)) {GSM_RxCharN=0;}
						}
					if(GSM_RxCharN >= 4){
						AppProtocol = 0;
						if(!strncmp_P(GSM_RxStr, PSTR("GET "), 4)) {AppProtocol = _HTTP;GSM_State = GSM_DataMode;break;}//GET
						if(!strncmp_P(GSM_RxStr, PSTR("POST"), 4)) {AppProtocol = _HTTP;GSM_State = GSM_DataMode;break;}//POST
						if(!strncmp_P(GSM_RxStr, PSTR("CLOS"), 4)) {AppProtocol = _HTTP;GSM_State = GSM_ReStart1;break;}//��������� ����������
						if(!strncmp_P(GSM_RxStr, PSTR("+PDP"), 4)) {AppProtocol = _HTTP;GSM_State = GSM_ReStart1;break;}//GPRS ��������
						if( (GSM_RxStr[0]==0) && (GSM_RxStr[1]==0)){AppProtocol = _HTTP;GSM_State = GSM_ReStart1;break;}//������� ������ �������
						//GSM_State = GSM_DataMode;
						
						if(AppProtocol == 0){
							InitFIFO();//���� ����� "�����", �������
							break;
						}
						StartTimer16(TD_CleanFIFO,500);
						//break;
					}
				}//while
				//GSM_RxCharN=0;
				if(Timer16Stopp(TD_CleanFIFO)&&(GSM_RX_FIFO_Begin == GSM_RX_FIFO_End)){
					StartTimer16(TD_CleanFIFO,500);
					InitFIFO();
				}

				if(Timer16Stopp(TD_data_start_delay)){
					if(GPRS_FlgSz_Out && (GPRS_FlgSz_Out <= GPRS_Out_MaxSz)&&(GSM_Flag&(1<<flg_TxCStr))&&Timer16Stopp(TD_timer_after_page_transfer)&&(!dataSendFlg)){			
							dataSendFlg=1;
							GSM_AddHeaderSendData(GPRS_Data_Out, GPRS_FlgSz_Out);
							break;
					}
/*					if(!IsWebSession()){					
							if(SMS_FlgSz_Out && erb(&SMS_On)&&(GSM_Flag&(1<<flg_TxCStr))&&(!dataSendFlg)){
								//---------------SMS
								GSM_State = GSM_Swtch2CommandMode;
								CommandModeDestination=1;
								TempNum = 0;
								break;
							}
							else{
								//--------------���������� GPRS ��� � ���
								if(Timer32Stopp(TD_1hour_resetGPRS_timer)&&(GSM_Flag&(1<<flg_TxCStr))&&(!dataSendFlg)){
									GSM_State = GSM_Swtch2CommandMode;
									CommandModeDestination=2;
									StartTimer32(TD_1hour_resetGPRS_timer,360000);
									break;
								}
								else{//--------�������� ������ �������
									if(Timer16Stopp(TD_CheckModem)&&(GSM_Flag&(1<<flg_TxCStr))&&Timer16Stopp(TD_timer_after_page_transfer)&&(!dataSendFlg)){
										CommandModeDestination=5;
										GSM_State = GSM_Swtch2CommandMode;
										if((Ballance==0)&&(numOfCheckBallanceRetry<1)){
											CommandModeDestination=2;
											numOfCheckBallanceRetry++;
											}
										StartTimer16(TD_CheckModem,30000);
										break;
									}
								}
							
							}
							if(Timer32Stopp(TD_GSM_Reset)){//------ ����� ������ ��� � �����
								GSM_State = GSM_ReStart1;
								break;
							}
					
					}*/
			}
			break;

		case GSM_DataMode:
			// ----- �� ������ � ��������
			if(GSM_RX_FIFO_Begin != GSM_RX_FIFO_End){	// if FIFO not empty
				StartTimer16(TD_TCP_Connect, erw(&TCP_CONNECT_timeout));	// ��������� ������ ��������
				break;
			}
			// ----- 
		/*	if(Timer16Stopp(TD_TCP_Connect)){	// ���� ��� ���������� �� ������ ��������� � CommandMode
				GSM_State = GSM_Swtch2CommandMode;
				break;
			}*/
			//StartTimer16(TD_CheckModem,1000);
			if( (strstr_P(GSM_RxStr, URC_CLOSED+2) != NULL) ){
				GSM_State =GSM_ReStart1;
				break;
			}			
			break;
		case GSM_Swtch2CommandMode:
			StartTimer16(TD_GSM,100);	// min 1000ms before +++
//IF WEB			WebClose();
			GSM_State++;

			break;
		case GSM_Swtch2CommandModePlus:
			if(Timer16Stopp(TD_GSM)){
				for(int i=0;i<GSM_RXSTR_SIZE;i++)
				{
					GSM_RxStr[i]=0;	
				}
				sprintf_P(GSM_TxStr, ESC_SEQ);
				GSMTxSz = strlen(GSM_TxStr);
				GSM_SendFirstChar();
				StartTimer16(TD_GSM,1000);	// min 500ms after +++
				GSM_State++;
			}
			break;
		case GSM_WAIT_D2C_SWITCH_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_ReStart1)) {

				switch(CommandModeDestination){
					case 1:	GSM_State = GSM_SEND_CMGS;
						break;
					case 2:	
						GSM_State = GSM_CIPSHUT_;
						counter=0;	
						break;						
					case 4:	
						GSM_State = GSM_SEND_ATD_Check_Ballance;
						counter=0;	
						break;
					case 5:	
						GSM_State = GSM_SEND_CSQ;
						counter=0;	
						break;
					default:GSM_State = GSM_Return2dataMode;
						break;
				}
			}
			break;

	}//switch

}
// ~~~~~~~~~~~~~~~~~~~~~~~~
inline static void GSM_Manual(){}
// ~~~~~~~~~~~~~~~~~~~~~~~~
inline static void GSM_Config(){
	switch(GSM_State){
		case GSM_CFG_START:
			StartTimer16(TD_GSM,200);
			GSM_PWRCNTRL_ON();
			GSM_State++;
			break;
		case GSM_CFG_SEND_AT:
			if(Timer16Stopp(TD_GSM)) GSM_Execute_Command(AT_AT, 100); GSM_State++;
			break;
		case GSM_CFG_WAIT_AT_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_CFG_SEND_AT)) GSM_State++;
			break;
		case GSM_CFG_SEND_IPR:
			GSM_Execute_Command(AT_IPR9600, 100); GSM_State++;
			break;
		case GSM_CFG_WAIT_IPR_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_CFG_SEND_AT)){
				StartTimer16(TD_GSM,200);
				GSM_State++;
			}
			break;
		case GSM_CFG_SEND_ATW:
			GSM_Execute_Command(AT_W, 100); GSM_State++;
			break;
		case GSM_CFG_WAIT_ATW_OK:
			if(GSM_Wait_Response_P(RESP_OK, GSM_CFG_SEND_AT)){
				StartTimer16(TD_GSM,200);
				GSM_State++;
			}
			break;
		case GSM_SAVE_IPR:
			if(Timer16Stopp(TD_GSM)) GSM_State++;
			break;
		case GSM_FINISH:			
			break;
		default:
			GSM_State = GSM_CFG_START;
	}//switch
}
// ~~~~~~~~~~~~~~~~~~~~~~~~
void
GSM_Cycle(void)
{
	switch(GSM_Control){
		case GSM_CNTL_AUTO:
			GSM_Auto();
			break;
		case GSM_CNTL_MANUAL:
			GSM_Manual();
			break;	
		case GSM_CNTL_CONFIG:
			GSM_Config();
			break;								
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~
void
GPRS_ServerAddr_Save(void)
{

}
// ~~~~~~~~~~~~~~~~~~~~~~~~
