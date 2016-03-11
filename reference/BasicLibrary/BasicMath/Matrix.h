/*
	�����������
	BUAA Ѧ�Һ�
	2010-1
	BUAA ���Գ�
	2012-8-24
*/

#ifndef _MATRIX_H
#define _MATRIX_H

#include ".\..\Basic\Basic.h"

#define	VNULL	((VEC *)Null)
#define	MNULL	((MAT *)Null)

#define EXCEPTION True
typedef F32 Real;

typedef struct
{
	U16 Dim;//ά��
	Real* Data;
} VEC;

//��M(�������)��N(�������)
typedef struct
{
	U16 M, N;
	Real** Data;
	Real* Base;

	//����:0 ��ͨ��1�Խǣ�2�Գ�
	U8 Property;
} MAT;

//�ڴ����
void* _BL_MatrixMemAllocate(int Size);
U8 _BL_CheckVec(VEC* Vec);
//��������(�Զ�����λ��)
VEC BL_VecCreate(U16 Dim);
//��������(�ֶ�����λ��)
void BL_VecAllocate(VEC* Vec, U16 Dim);
//�������ݿռ�
void BL_VecFree(VEC* Vec);
//ǿ����������λ��(�����н����ڴ����)
U8 BL_VecSetData(VEC* Vec, Real* Data, U16 Dim);
//��������ֵΪһͬһ��ֵ
void BL_VecSetValue(VEC* Vec, Real Value);
//�������и������ݵ�������
U8 BL_VecCopyFromArray(VEC* VecDst, Real* ArraySrc);
//��VecSrc�����ݸ��Ƶ�VecDst��
U8 BL_VecCopy(VEC* VecDst, VEC VecSrc);
//����ָ��λ�õ��������ݵ���һ������ָ��λ��
U8 BL_VecIndexCopy(VEC* VecDst, U16 IndexDst, VEC VecSrc, U16 IndexSrc, U16 Length);
//�������
//VecDst=VecSrc1+VecSrc2
U8 BL_VecAdd(VEC* VecDst, VEC VecSrc1, VEC VecSrc2);
//�������
//VecDst=VecSrc1-VecSrc2
U8 BL_VecSub(VEC* VecDst, VEC VecSrc1, VEC VecSrc2);
//������˸���ֵ
//VecDst=VecSrc*Value
U8 BL_VecXReal(VEC* VecDst, VEC VecSrc, Real Value);
//�����Բ�˸���ֵ
//Vec=Vec*Value
U8 BL_VecSelfXReal(VEC* Vec, Real Value);
//������ƽ������
Real BL_VecSquareLength(VEC* Vec);
//������λ��
U8 BL_VecUnit(VEC* VecDst, VEC* VecSrc);
//�����Ե�λ��
U8 BL_VecUnitSelf(VEC* Vec);
//��ά�������
void  BL_Vec3DimX(VEC* VecDst, VEC VecSrc1,  VEC VecSrc2);
//���������ֵ
//R=Vec1*Vec2
Real  BL_VecXVec(VEC Vec1,  VEC Vec2);
//�����ʽֵ(�ߴ�ϵ����ǰ)
//aX^2+bX+c...
Real BL_VecCalcEq(VEC* Vec, Real X);
//����ֵ���
Real BL_VecSumValue(VEC vec);
//��������(�Զ�����λ��)
MAT BL_MatCreate(U16 M, U16 N);
//��������(�ֶ�����λ��)
void BL_MatAllocate(MAT* Mat, U16 M, U16 N);
//�շ���FALSE
U8 BL_IsMatUnEmpty(MAT* Mat);
//���վ������ݿռ�
void BL_MatFree(MAT* Mat);
//�Ѿ����������е�ֵ��Ϊͬһֵ
U8 BL_MatSetValue(MAT* Mat, Real Value);
//���ƾ���
//MatDst = MatSrc
U8 BL_MatCopy(MAT* MatDst, MAT MatSrc);
//��MatSrc�����Ƶ�MatDst[a][b]��
U8 BL_MatIndexCopy(MAT* MatDst, U16 A, U16 B, MAT MatSrc);
//��Mat��ÿһ��ΪVec
U8 BL_MatRowSetVec(MAT* Mat, VEC Vec);
//��Mat��Ϊ�Խ���Mat[i][i] = value
U8 BL_MatEye(MAT* Mat, Real Value);
//������ת��
//MatDst = (MatSrc)T
U8 BL_MatTran(MAT* MatDst, MAT MatSrc);
//�������
//MatDst = MatSrc1 + MatSrc2
U8 BL_MatAdd(MAT* MatDst, MAT MatSrc1, MAT MatSrc2);
//�������
//MatDst = MatSrc1 - MatSrc2
U8 BL_MatSub(MAT* MatDst, MAT MatSrc1, MAT MatSrc2);
//��������
//MatDst = inv(MatSrc)
//MatTempΪ��������ʱ��Ҫ��֤�㹻��
U8 BL_MatInv(MAT* MatDst, MAT MatSrc, MAT MatTemp);
//�������
//MatDst = MatSrc1 * MatSrc2
U8 BL_MatMul(MAT* MatDst, MAT MatSrc1, MAT MatSrc2);
//�������ʵ��
//MatDst = MatSrc * Value
U8 BL_MatXReal(MAT* MatDst, MAT MatSrc, Real Value);
//�������ʵ��
//Mat = Mat * Value
U8 BL_MatSelfXReal(MAT* Mat, Real Value);
//�����������
//VecDst = MatSrc * Vec
U8 BL_MatXVec(VEC* VecDst, MAT MatSrc, VEC VecSrc);
//�������Ծ���
//VecDst = VecSrc * MatSrc
U8 BL_VecXMat(VEC* VecDst, VEC VecSrc, MAT MatSrc);
//�󶨵�ַΪָ�������ָ����λ��
U8 BL_MatBindAddress(MAT* MatDst, U16 MNew, U16 NNew, MAT MatSrc, U16 MStart, U16 NStart);
//ת��Ϊ�Գƾ���
U8 BL_MatSetSym(MAT* Mat);
//���ǶԽ����ϵ���������
U8 BL_MatSetEye(MAT* Mat);
//�������ֵ��
Real BL_MatSumValue(MAT Mat);

#endif
