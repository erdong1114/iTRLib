#ifndef _TCPLITEPROTOCOL_H
#define _TCPLITEPROTOCOL_H

#include ".\..\Basic\Basic.h"
#include ".\..\DataStruct\DataStruct.h"

//#define ZTCPDataLengthMax 64
#define TCPLiteQueueLength 8
#define TCPLitePortMax 4

typedef enum
{
	Waiting = 0, Sended = 1, ReSended = 2, Done = 3, Fail = 4
} TCPLiteStateEnum;

typedef struct
{
	U8 Port;
	U8 ID;
	U8* Data;
	U8 DataLength;
	U8 DataCapacity;
} TCPLiteDataStruct;

typedef struct
{
	TCPLiteDataStruct TCPLiteData;
	bool Done, Fail;
	S32 Tick, SendTime;
} TCPLiteStateStruct;

typedef void (*TCPLiteFunctionData)(TCPLiteDataStruct* TCPLiteData);
typedef void (*TCPLiteFunction)(void* TCPLite);

typedef struct
{
	U8 TCPDataLengthMax;
	S32 TimeOut;
	S32 RetryTimes;
	S32 ResponseTimes;

	GeneralSend TCPLiteDataSendFun;
	TCPLiteFunctionData TCPLitePorts[TCPLitePortMax];
	TCPLiteFunctionData TCPLiteOnReceive;
	TCPLiteFunction TCPLiteOnConnectionStateChange;

	BL_QueueU8Struct ToSendQueue;
	TCPLiteStateStruct TCPLiteStates[TCPLiteQueueLength];
	U8* SendBuffer;

	S32 MainID;
	S32 Tick;
	bool NeedResponse;
	bool IsConnected;
} TCPLiteStruct;

//��ʼ��TCPLiteDataStruct�ṹ�岢����ָ���ռ��Data����
//bool TCPLiteDataInit(TCPLiteDataStruct* TCPLiteData, U8 DataCapacity);
//��TCPLiteData���������
//bool TCPLiteDataFill(TCPLiteDataStruct* TCPLiteData,U8 Port,U8* Data,U8 Length);
//ʹ�û��������TCPLiteData
bool TCPLiteRAWDataFill(TCPLiteDataStruct* TCPLiteData, U8* Buffer, U8 Length);
//TCPLiteState״̬����
//void TCPLiteStateClear(TCPLiteStateStruct* TCPLiteState);
//TCPLiteState״̬��ʼ��,��Ϊ���е�TCPLiteData����ռ�
//bool TCPLiteStateInit(TCPLiteStateStruct* TCPLiteState,U8 DataCapacity);
//�������״̬
TCPLiteStateEnum TCPLiteStateEnumGet(TCPLiteStateStruct* TCPLiteState);
//��ʼ��TCPLite��Ϊ�����жӷ���ռ�
bool TCPLiteInit(TCPLiteStruct* TCPLite, U8 ZTCPDataLengthMax, GeneralSend TCPLiteDataSendFun);
//ע��˿���Ӧ
bool TCPLitePortReg(TCPLiteStruct* TCPLite, U8 Port, TCPLiteFunctionData Fun);
//��鷢���ж��Ƿ���ʣ��ռ�
bool TCPLiteToSendAvailable(TCPLiteStruct* TCPLite);
//��鷢���ж�ʣ��ռ�
S32 TCPLiteToSendAvailableNum(TCPLiteStruct* TCPLite);
//���һ�����еķ��Ϳռ�
//U8 TCPLiteGetToSendAvailable(TCPLiteStruct* TCPLite);
//���涨��ʽ��������
//bool TCPLiteSendRAW(TCPLiteStruct* TCPLite,U8 ID,U8 Port,U8* Data,U8 Length);
//��������
bool TCPLiteSend(TCPLiteStruct* TCPLite, U8 Port, U8* Data, U8 Length);
//S32 Add1(S32 ID);
//S32 Sub1(S32 ID);
//��������״̬
//void ChangeConnectionState(TCPLiteStruct* TCPLite,bool NewState);
//��̨���ݴ�����
void TCPLiteSendWorkFunation(TCPLiteStruct* TCPLite);
//��շ����жӲ����ü�����
//void TCPLiteClear(TCPLiteStruct* TCPLite)
//���ݽ��մ���
void TCPLiteReceiveProcess(TCPLiteStruct* TCPLite, TCPLiteDataStruct* TCPLiteData);

#endif

