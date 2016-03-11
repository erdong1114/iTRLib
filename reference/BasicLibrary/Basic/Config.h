/*
	Basic����
	BUAA ���Գ�
	2012-9-23
*/
#ifndef __CONFIG_H
#define __CONFIG_H

//**********Config CORE**********
#define CORE_X86 1
#define CORE_X64 2
#define CORE_ARM7 3
#define CORE_ARM9 4
#define CORE_ARM11 5
#define CORE_CONTEX_M0 6
#define CORE_CONTEX_M3 7
#define CORE_CONTEX_M4 9

//CONFIG:Change to Fix Target CORE
//#define CORE CORE_X86

//**********Config COMPILER**********
#define COMPILER_GCC 1
#define COMPILER_KIEL 2
#define COMPILER_VC 3

//CONFIG:Change to Fix Target COMPILER
//#define COMPILER COMPILER_GCC

//**********Config ASSERT**********
#define NOASSERT 0
#define YESASSERT 1
#define AUTOASSERT 2

//CONFIG:Config to Support Assert or Not
#define ISASSERT AUTOASSERT

#if(ISASSERT==NOASSERT)
//����NDEBUG�Ӷ�ͣ�ö��Թ���
#ifndef NDEBUG
#define NDEBUG
#endif
#elif(ISASSERT==YESASSERT)
//ȡ������NDEBUG�Ӷ����ö��Թ���
#ifdef NDEBUG
#undef NDEBUG
#endif
#elif(ISASSERT==AUTOASSERT)
//��û�ж���DEBUGʱ(�ǵ���)����NDEBUG�Ӷ�ͣ�ö��Թ���
#ifndef DEBUG
#ifndef NDEBUG
#define NDEBUG
#endif
#else
#ifdef NDEBUG
#undef NDEBUG
#endif
#endif
#endif
#include <assert.h>

#define ALIG_MACRO_NO 0
#define ALIG_MACRO_YES 1

#define ALIG_MACRO_USE ALIG_MACRO_YES

#endif

