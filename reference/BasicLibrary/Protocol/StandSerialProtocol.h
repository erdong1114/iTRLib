#ifndef STANDSERIALPROTOCOL
#define STANDSERIALPROTOCOL

#include ".\..\Basic\Basic.h"
#include ".\..\DataStruct\DataStruct.h"

//*****Protocol Level 0*****

#define StandSerialProtocolMaxLength 255
#define StandSerialProtocolHeadLength 2
#define StandSerialProtocolMateLength 2
#define StandSerialProtocolCRCLength 2
#define StandSerialProtocolPlusLength StandSerialProtocolHeadLength+StandSerialProtocolMateLength+StandSerialProtocolCRCLength
#define StandSerialProtocolKey1Length 16
#define StandSerialProtocolKey2Length 4

struct _StandFrameStruct
{
	U8 S0, S1;
	U8 Length;
	U8 Property;
	U8 Package;
	//U16 CRC;
};
typedef struct _StandFrameStruct StandFrameStruct;

struct _StandFrameMapStruct
{
	U8* BaseAddress;
	U8 PackageLength;
};
typedef struct _StandFrameMapStruct StandFrameMapStruct;

//For StandFrameStruct
#define SFSAddress(SFMS) ((StandFrameStruct*)((SFMS)->BaseAddress))
#define SFSLength(SFMS) ((SFSAddress(SFMS))->Length)
#define SFSProperty(SFMS) ((SFSAddress(SFMS))->Property)
#define SFSProtocolID(SFMS) ((SFSProperty(SFMS))&0x0F)
#define SFSIsEncrypt(SFMS) (((SFSProperty(SFMS))&0x10)==0x10)
#define SFSPackageAddress(SFMS) (&((SFSAddress(SFMS))->Package))
#define SFSPackageLength(SFMS) ((SFMS)->PackageLength)
#define SFSCRC(SFMS) (*((U16*)((SFSPackageAddress(SFMS))+(SFSPackageLength(SFMS)))))

typedef void (*SSPReceive)(void* SSPTObj, StandFrameMapStruct* SFMS);

struct _StandSerialProtocolStruct
{
	U8 DefaultS0, DefaultS1;
	bool AutoDecrypt, AutoEncrypt;
	U8 Key1[StandSerialProtocolKey1Length];
	U8 Key2[StandSerialProtocolKey2Length];
	bool IsHaveNewFrame;
	StandFrameMapStruct SFMS;
	SSPReceive ProcessFunction[16];
	GeneralSend DataSendFun;
	S32 receiveBufferState;
	U8 receiveBuffer[StandSerialProtocolMaxLength];
	U8 sendBuffer[StandSerialProtocolMaxLength];
};
typedef struct _StandSerialProtocolStruct StandSerialProtocolStruct;

//��ʼ��Э��ջ
//StandSerialProtocolObj=�ڲ�����ʼ��Э���ջ����
//DefaultS0=Э��֡������0
//DefaultS1=Э��֡������1
//SendQueue=���ݷ��ͽӿڶ���
void SSPStructInit(StandSerialProtocolStruct* SSPTObj, U8 DefaultS0, U8 DefaultS1, GeneralSend DataSendFun);
//������յ��Ĵ������ݣ����н���Э��֡��
//StandSerialProtocolObj=Э���ջ����
//Buffer=���ݽ��սӿڶ���
S32 SSPProcessRAWByte(StandSerialProtocolStruct* SSPTObj, U8* Buffer, S32 Length);
//����֡CRC16���
//SFMS=�����֡
//bool SSPFormatCheck(StandFrameMapStruct *SFMS);
//void SSPEncrypt(StandFrameMapStruct *SFMS, U8 *Key1, U8 *Key2);
//void SSPDecrypt(StandFrameMapStruct *SFMS, U8 *Key1, U8 *Key2);
//��ʼ��֡����
//SFMS=�ڲ�����ʼ��֡����
//BaseAddress=���ݻ���ַ(�˵�ַ�Ŀռ�Ӧ��Ϊ�ѷ���)
//PackageLength=���ݰ�����
//void SSPInitSFMS(StandFrameMapStruct *SFMS, U8 *BaseAddress, U8 PackageLength);
//���֡����(��������ȷ��)
//StandSerialProtocolObj=Э���ջ����
//SFMS=�����֡
//ProtocolID=Э��ID
//Package=���ݰ�����
//S32 SSPFillSFMS(StandSerialProtocolStruct *SSPTObj, StandFrameMapStruct *SFMS, U8 ProtocolID, U8 *Package);
//����֡
//StandSerialProtocolObj=Э���ջ����
//SFMS=������֡
//void SSPSendSFMSRAW(StandSerialProtocolStruct *SSPTObj, StandFrameMapStruct *SFMS);
//����֡(ʹ�÷��ͻ�����)
//StandSerialProtocolObj=Э���ջ����
//ProtocolID=Э��ID
//Package=���ݰ�����
//PackageLength=���ݰ�����
void SSPSendPackage(StandSerialProtocolStruct* SSPTObj, U8 ProtocolID, U8* Package, U8 PackageLength);

#endif

