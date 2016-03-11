/*
	��������ʵ�ѭ��FIFO�ж�
	BUAA ���Գ�
	2012-8-23
*/
#ifndef __QUEUE_H
#define __QUEUE_H

#include ".\..\Basic\Basic.h"

//*****Queue of U8*****
struct _BL_QueueU8Struct
{
	U8* _Data;
	S32 Length, _Start, Count;
};
typedef struct _BL_QueueU8Struct BL_QueueU8Struct;

void BL_QueueU8Init(BL_QueueU8Struct* Obj, S32 Length);
void BL_QueueU8Clear(BL_QueueU8Struct* Obj);
void BL_QueueU8Free(BL_QueueU8Struct* Obj);
U8 BL_QueueU8Insert(BL_QueueU8Struct* Obj, U8 C);
U8 BL_QueueU8Fetch(BL_QueueU8Struct* Obj);
U8 BL_QueueU8Read(BL_QueueU8Struct* Obj, S32 Num);

//*****Queue of F32*****
struct _BL_QueueF32Struct
{
	F32* _Data;
	S32 Length, _Start, Count;
};
typedef struct _BL_QueueF32Struct BL_QueueF32Struct;

void BL_QueueF32Init(BL_QueueF32Struct* Obj, S32 Length);
void BL_QueueF32Clear(BL_QueueF32Struct* Obj);
void BL_QueueF32Free(BL_QueueF32Struct* Obj);
F32 BL_QueueF32Insert(BL_QueueF32Struct* Obj, F32 F);
F32 BL_QueueF32Fetch(BL_QueueF32Struct* Obj);
F32 BL_QueueF32Read(BL_QueueF32Struct* Obj, S32 Num);

//*****Queue of Any*****
struct _BL_QueueAnyStruct
{
	U8* _Data;
	S32 Length, _Start, Count, Size;
};
typedef struct _BL_QueueAnyStruct BL_QueueAnyStruct;

//��ʼ���ж�,Size=Ԫ�ش�С(�ֽ�)
void BL_QueueAnyInit (BL_QueueAnyStruct* Obj, S32 Length, S32 Size);
//����ж��е�����
void BL_QueueAnyClear (BL_QueueAnyStruct* Obj);
//�ͷ��ж���ռ�ÿռ�
void BL_QueueAnyFree (BL_QueueAnyStruct* Obj);
//����ֵ=TRUEʱ˵���ж������²���������Ѽ�ռ���ϵ����ݵ�λ��
bool BL_QueueAnyInsert (BL_QueueAnyStruct* Obj, void* Data);
//ȡ����������ݣ����Ϊ���򷵻�0��
void* BL_QueueAnyFetch (BL_QueueAnyStruct* Obj);
//������˳���ȡ���ݣ����ָ��λ�������򷵻�0��
void* BL_QueueAnyRead (BL_QueueAnyStruct* Obj, S32 Num);

#endif

