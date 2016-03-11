/*
	��������ʵ�ѭ��FIFO�ж�
	BUAA ���Գ�
	2012-8-23
*/
#include <stdlib.h>
#include ".\..\Basic\Basic.h"
#include "Queue.h"

//*****Queue of U8*****
//������һ
void _BL_QueueU8StartAdd (BL_QueueU8Struct* Obj)
{
	Obj->_Start++;
	if (Obj->_Start >= Obj->Length)
	{
		Obj->_Start -= Obj->Length;
	}
}

//��ʼ���ж�
void BL_QueueU8Init (BL_QueueU8Struct* Obj, S32 Length)
{
	Obj->_Start = 0;
	Obj->Count = 0;
	Obj->Length = Length;
	Obj->_Data = Null;
	Obj->_Data = BL_MemAllocEx(Length * sizeof (U8));
	assert(Obj->_Data);
	BL_MemClear(Obj->_Data, Length * sizeof (U8));
}

//����ж��е�����
void BL_QueueU8Clear (BL_QueueU8Struct* Obj)
{
	Obj->_Start = 0;
	Obj->Count = 0;
}

//�ͷ��ж���ռ�ÿռ�
void BL_QueueU8Free (BL_QueueU8Struct* Obj)
{
	Obj->Length = 0;
	free (Obj->_Data);
}

//����ֵ=TRUEʱ˵���ж������²���������Ѽ�ռ���ϵ����ݵ�λ��
U8 BL_QueueU8Insert (BL_QueueU8Struct* Obj, U8 C)
{
	U32 t = Obj->_Start + Obj->Count;
	if (t >= Obj->Length)
	{
		t -= Obj->Length;
	}
	Obj->_Data[t] = C;
	if (Obj->Count == Obj->Length)
	{
		_BL_QueueU8StartAdd (Obj);
		return True;
	}
	else
	{
		Obj->Count++;
		return False;
	}
}

//ȡ����������ݣ����Ϊ���򷵻�0��
U8 BL_QueueU8Fetch (BL_QueueU8Struct* Obj)
{
	assert(Obj->Count > 0);
	if (Obj->Count > 0)
	{
		U8 c = Obj->_Data[Obj->_Start];
		_BL_QueueU8StartAdd (Obj);
		Obj->Count--;
		return c;
	}
	else
	{
		return 0;
	}
}

//������˳���ȡ���ݣ����ָ��λ�������򷵻�0��
U8 BL_QueueU8Read (BL_QueueU8Struct* Obj, S32 Num)
{
	assert(Num < Obj->Count);
	if (Num < Obj->Count)
	{
		U32 t = Obj->_Start + Num;
		U8 c;
		if (t >= Obj->Length)
		{
			t -= Obj->Length;
		}
		c = Obj->_Data[t];
		return c;
	}
	return 0;
}

//*****Queue of F32*****
//������һ
void _BL_QueueF32StartAdd (BL_QueueF32Struct* Obj)
{
	Obj->_Start++;
	if (Obj->_Start >= Obj->Length)
	{
		Obj->_Start -= Obj->Length;
	}
}

//��ʼ���ж�
void BL_QueueF32Init (BL_QueueF32Struct* Obj, S32 Length)
{
	Obj->_Start = 0;
	Obj->Count = 0;
	Obj->Length = Length;
	Obj->_Data = Null;
	Obj->_Data = (F32*)BL_MemAllocEx(Length * sizeof (F32) );
	assert(Obj->_Data);
	BL_MemClear(Obj->_Data, Length * sizeof (F32));
}

//����ж��е�����
void BL_QueueF32Clear (BL_QueueF32Struct* Obj)
{
	Obj->_Start = 0;
	Obj->Count = 0;
}

//�ͷ��ж���ռ�ÿռ�
void BL_QueueF32Free (BL_QueueF32Struct* Obj)
{
	Obj->Length = 0;
	free (Obj->_Data);
}

//����ֵ=TRUEʱ˵���ж������²���������Ѽ�ռ���ϵ����ݵ�λ��
F32 BL_QueueF32Insert (BL_QueueF32Struct* Obj, F32 F)
{
	U32 t = Obj->_Start + Obj->Count;
	if (t >= Obj->Length)
	{
		t -= Obj->Length;
	}
	Obj->_Data[t] = F;
	if (Obj->Count == Obj->Length)
	{
		_BL_QueueF32StartAdd (Obj);
		return True;
	}
	else
	{
		Obj->Count++;
		return False;
	}
}

//ȡ����������ݣ����Ϊ���򷵻�0��
F32 BL_QueueF32Fetch (BL_QueueF32Struct* Obj)
{
	assert(Obj->Count > 0);
	if (Obj->Count > 0)
	{
		F32 f = Obj->_Data[Obj->_Start];
		_BL_QueueF32StartAdd (Obj);
		Obj->Count--;
		return f;
	}
	else
	{
		return 0;
	}
}

//������˳���ȡ���ݣ����ָ��λ�������򷵻�0��
F32 BL_QueueF32Read (BL_QueueF32Struct* Obj, S32 Num)
{
	assert(Num < Obj->Count);
	if (Num < Obj->Count)
	{
		U32 t = Obj->_Start + Num;
		F32 f;
		if (t >= Obj->Length)
		{
			t -= Obj->Length;
		}
		f = Obj->_Data[t];
		return f;
	}
	return 0;
}

//*****Queue of Any*****
//������һ
void _BL_QueueAnyStartAdd (BL_QueueAnyStruct* Obj)
{
	Obj->_Start++;
	if (Obj->_Start >= Obj->Length)
	{
		Obj->_Start -= Obj->Length;
	}
}

void* _BL_QueueAnyGetData(BL_QueueAnyStruct* Obj, U32 Offset)
{
	assert(Obj->_Data);
	return &Obj->_Data[Offset*Obj->Size];
}

//��ʼ���ж�,Size=Ԫ�ش�С(�ֽ�)
void BL_QueueAnyInit (BL_QueueAnyStruct* Obj, S32 Length, S32 Size)
{
	Obj->_Start = 0;
	Obj->Count = 0;
	Obj->Length = Length;
	Obj->_Data = Null;
	Obj->Size = Size;

	Obj->_Data = (U8*)BL_MemAllocEx(Length * Size);
	assert(Obj->_Data);
	BL_MemClear(Obj->_Data, Length * Size);
}

//����ж��е�����
void BL_QueueAnyClear (BL_QueueAnyStruct* Obj)
{
	Obj->_Start = 0;
	Obj->Count = 0;
}

//�ͷ��ж���ռ�ÿռ�
void BL_QueueAnyFree (BL_QueueAnyStruct* Obj)
{
	Obj->Length = 0;
	free (Obj->_Data);
}

//����ֵ=TRUEʱ˵���ж������²���������Ѽ�ռ���ϵ����ݵ�λ��
bool BL_QueueAnyInsert (BL_QueueAnyStruct* Obj, void* Data)
{
	U32 t = Obj->_Start + Obj->Count;
	assert(Obj->_Data);
	if (t >= Obj->Length)
	{
		t -= Obj->Length;
	}
	BL_MemCopy(_BL_QueueAnyGetData, Data, Obj->Size);
	if (Obj->Count == Obj->Length)
	{
		_BL_QueueAnyStartAdd (Obj);
		return True;
	}
	else
	{
		Obj->Count++;
		return False;
	}
}

//ȡ����������ݣ����Ϊ���򷵻�0��
void* BL_QueueAnyFetch (BL_QueueAnyStruct* Obj)
{
	assert(Obj->Count > 0);
	if (Obj->Count > 0)
	{
		void* p = _BL_QueueAnyGetData(Obj, Obj->_Start);
		_BL_QueueAnyStartAdd (Obj);
		Obj->Count--;
		return p;
	}
	else
	{
		return Null;
	}
}

//������˳���ȡ���ݣ����ָ��λ�������򷵻�0��
void* BL_QueueAnyRead (BL_QueueAnyStruct* Obj, S32 Num)
{
	assert(Num < Obj->Count);
	if (Num < Obj->Count)
	{
		U32 t = Obj->_Start + Num;
		if (t >= Obj->Length)
		{
			t -= Obj->Length;
		}
		return _BL_QueueAnyGetData(Obj, t);
	}
	return Null;
}

