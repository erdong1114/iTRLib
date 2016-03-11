#ifndef _GIMBALCTRL_H
#define _GIMBALCTRL_H

#include ".\..\Basic\Basic.h"

#define GimbalServoNum 3
#define GimbalModeDisable 0
#define GimbalModeBodyFix 1
#define GimbalModeAttSteady 2
#define GimbalModePosLock 3

typedef struct _GimbalCtrlStruct
{	
	//0=Pitch+Roll��ʾ��Type=0ʱ��̨����Ϊ�ȸ����ٹ�ת���ӻ��忪ʼ���㣩���Դ����ơ�
	U8 Type;//0=Pitch+Roll,1=Roll+Pitch,2=Pitch+Yaw,3=Yaw+Pitch,4=Roll+Yaw,5=Yaw+Roll,6=Yaw+Pitch+Roll,7=Yaw+Roll+Pitch
	U8 Mode;//0=Disable,1=BodyFix,2=AttSteady,3=PosLock
	F32 TarRate[GimbalServoNum];//Pitch,Roll,Yaw
	F32 TarAtt[GimbalServoNum];//Pitch,Roll,Yaw
	F64 TarPos[3];//Lon,Lat,Alt
	F32 PitchLimit[2];//-,+
	F32 RollLimit[2];//-,+
	F32 YawLimit[2];//-,+
} GimbalCtrlStruct;

/*
	GimbalCalcParaΪ��̨ģʽ������
	AttΪ��������̬(Pitch,Roll,Yaw)
	PosΪ������λ��(Lon,Lat,Alt)
	AngΪ��ǰ��̨�Ƕȣ�������Ҳ��������(Pitch,Roll,Yaw)
*/
S32 GimbalCtrlProcess(GimbalCtrlStruct* GimbalCtrl,F32* Att,F64* Pos,F32* Ang);

#endif

