/*
 *
 *   Copyright (C) 2013 BUAA iTR Research Center. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * matrix.cc
 *  Created on: 2013-9-14
 *      Author: zhouyi
 */

#include <stddef.h>
#include "../platform/platform.h"
#include "math.h"

//for SVD use
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define NR_END 0
#define tol  0.00001
#define FREE_ARG char*
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
static double dmaxarg1,dmaxarg2;
#define DMAX(a,b) (dmaxarg1=(a),dmaxarg2=(b),(dmaxarg1) > (dmaxarg2) ?(dmaxarg1) : (dmaxarg2))
static int iminarg1,iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ?(iminarg1) : (iminarg2))

/// ////////////////////////////////////////////////////////
//svd form google
//#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : - fabs(a))

static double maxarg1,maxarg2;
#define FMAX(a,b) (maxarg1 = (a),maxarg2 = (b),(maxarg1) > (maxarg2) ? (maxarg1) : (maxarg2))

//static int iminarg1,iminarg2;
//#define IMIN(a,b) (iminarg1 = (a),iminarg2 = (b),(iminarg1 < (iminarg2) ? (iminarg1) : iminarg2))

static double sqrarg;
#define SQR(a) ((sqrarg = (a)) == 0.0 ? 0.0 : sqrarg * sqrarg)
namespace itr_math
{
/*
 * 初始化一个指定行列数的空矩阵(自动分配内存)
 */
Matrix::Matrix(S32 Row, S32 Col)
{
    row=0;
    col=0;
    data=NULL;
    localData=false;
    Init(Row,Col);
}
/*
 * 初始化一个指定行列数的矩阵(以传入的指针为数据区,不分配本地内存)
 */
Matrix::Matrix(S32 Row, S32 Col, F32* Data)
{
    row=0;
    col=0;
    data=NULL;
    localData=false;
    Init(Row,Col,Data);
}
/*
 * 初始化一个指定阶数的方阵(自动分配内存)
 */
Matrix::Matrix(S32 RowCol)
{
    row=0;
    col=0;
    data=NULL;
    localData=false;
    Init(RowCol);
}
/*
 * 初始化完全一样的矩阵(Clone)
 */
Matrix::Matrix(const Matrix& Mat)
{
    assert(NumericalObj!=NULL && CalculateObj!=NULL);
    data = new F32[Mat.GetRow() * Mat.GetCol()]();
    assert(data!=NULL);
    row = Mat.GetRow();
    col = Mat.GetCol();
    localData = true;
    this->CopyFrom(Mat.data);
}
/*
 * 用于初始化列表的空构造函数
 * 在构造后需手动调用Init函数
 */
Matrix::Matrix()
{
    row=0;
    col=0;
    data=NULL;
    localData=false;
}
/*
 * 回收自动分配的内存
 */
Matrix::~Matrix()
{
    if (localData == true)
    {
        delete data;
    }
}

//**********后初始化函数**********
/*
* 初始化一个指定行列数的空矩阵(自动分配内存)
*/
void Matrix::Init(S32 Row, S32 Col)
{
    assert(data==NULL);
    assert(NumericalObj!=NULL && CalculateObj!=NULL);
    assert(Row>0 && Col>0);
    data = new F32[Row * Col]();
    assert(data!=NULL);
    row = Row;
    col = Col;
    localData = true;
}
/*
 * 初始化一个指定行列数的矩阵(以传入的指针为数据区)
 */
void Matrix::Init(S32 Row, S32 Col, F32* Data)
{
    assert(data==NULL);
    assert(NumericalObj!=NULL && CalculateObj!=NULL);
    assert(Row>0 && Col>0);
    data = Data;
    assert(data != NULL);
    row = Row;
    col = Col;
    localData = false;
}
/*
 * 初始化一个指定阶数的方阵(自动分配内存)
 */
void Matrix::Init(S32 RowCol)
{
    assert(data==NULL);
    assert(NumericalObj!=NULL && CalculateObj!=NULL);
    assert(RowCol>0);
    data = new F32[RowCol * RowCol]();
    assert(data!=NULL);
    row = RowCol;
    col = RowCol;
    localData = true;
}

//**********初等变换**********
/*
 * 将RowPosSource行加RowPosTarget行
 */
void Matrix::AddRow(S32 RowPosSource, S32 RowPosTarget)
{
    assert(RowPosSource >= 0 && RowPosSource < row);
    assert(RowPosTarget >= 0 && RowPosTarget < row);
    CalculateObj->Add(data + RowPosSource* col, data + RowPosTarget * col, col,data + RowPosTarget * col);
}
/*
 * 将Data加至RowPosTarget行
 */
void Matrix::AddRow(F32* Data, S32 RowPosTarget)
{
    assert(Data!=NULL);
    assert(RowPosTarget >= 0 && RowPosTarget < row);
    CalculateObj->Add(Data, data + RowPosTarget * col, col, data + RowPosTarget * col);
}
/*
 * 将RowPosSource行减至RowPosTarget行
 */
void Matrix::SubRow(S32 RowPosSource, S32 RowPosTarget)
{
    assert(RowPosSource >= 0 && RowPosSource < row);
    assert(RowPosTarget >= 0 && RowPosTarget < row);
    CalculateObj->Sub( data + RowPosTarget * col,data + RowPosSource* col, col,data + RowPosTarget * col);
}
/*
 * 将Data减至RowPosTarget行
 */
void Matrix::SubRow(F32* Data, S32 RowPosTarget)
{
    assert(Data!=NULL);
    assert(RowPosTarget >= 0 && RowPosTarget < row);
    CalculateObj->Sub(data + RowPosTarget * col, Data, col,data + RowPosTarget  * col);
}
/*
 * 将RowPosTarget行乘以K
 */
void Matrix::MulRow(F32 K, S32 RowPosTarget)
{
    assert(RowPosTarget >= 0 && RowPosTarget < row);
    CalculateObj->Scale(data + RowPosTarget  * col, K, col, data + RowPosTarget * col);
}
//Swap Row
/*
 * 交换RowNoA行和RowNoB行
 */
void Matrix::SwapRow(S32 RowPosA, S32 RowPosB)
{
    assert(RowPosA >= 0 && RowPosA < row);
    assert(RowPosB >= 0 && RowPosB < row);
    MemorySwap(data + RowPosA * col, data + RowPosB * col, col*sizeof(F32));
}
/*
 * 将ColNoAdd列加至ColNoResult列
 */
void Matrix::AddCol(S32 ColPosSource, S32 ColPosTarget)
{
    assert(ColPosSource >= 0 && ColPosSource < col);
    assert(ColPosTarget >= 0 && ColPosTarget < col);
    for (S32 i = 0; i < row; i++)
    {
        data[ColPosTarget]+=data[ColPosSource];
        ColPosSource+=col;
        ColPosTarget+=col;
    }
}
/*
 * 将Data加至ColNoResult列
 */
void Matrix::AddCol(F32* Data, S32 ColPosTarget)
{
    assert(Data!=NULL);
    assert(ColPosTarget >= 0 && ColPosTarget<col);

    for (S32 i = 0; i < row; i++)
    {
        data[ColPosTarget]+=Data[i];
        ColPosTarget+=col;
    }
}
/*
 * 将ColNoSub列减至ColNoResult列
 */
void Matrix::SubCol(S32 ColPosSource, S32 ColPosTarget)
{
    assert(ColPosSource >= 0 && ColPosSource < col);
    assert(ColPosTarget >= 0 && ColPosTarget < col);
    for (S32 i = 0; i < row; i++)
    {
        data[ColPosTarget]-=data[ColPosSource];
        ColPosSource+=col;
        ColPosTarget+=col;
    }
}
/*
 * 将Data减至ColNoResult列
 */
void Matrix::SubCol(F32* Data, S32 ColPosTarget)
{
    assert(Data!=NULL);
    assert(ColPosTarget >= 0 && ColPosTarget<col);

    for (S32 i = 0; i < row; i++)
    {
        data[ColPosTarget]-=Data[i];
        ColPosTarget+=col;
    }
}
/*
 * 将ColNoResult列乘以K
 */
void Matrix::MulCol(F32 K, S32 ColPosTarget)
{
    assert(ColPosTarget >= 0 && ColPosTarget < col);
    for (S32 i = 0; i < row; i++)
    {
        data[ColPosTarget ] *= K;
        ColPosTarget+=col;
    }
}
/*
 * 交换ColNoA列和ColNoB列
 */
void Matrix::SwapCol(S32 ColPosA, S32 ColPosB)
{
    assert(ColPosA >= 0 && ColPosA < col);
    assert(ColPosB >= 0 && ColPosB < col);
    F32 temp;
    for (S32 i = 0; i < row; i++)
    {
        temp=data[ColPosB];
        data[ColPosB]=data[ColPosA];
        data[ColPosA]=temp;
        ColPosA+=col;
        ColPosB+=col;
    }
}
//**********常量相关计算**********
/*
 * 全部元素加上K
 */
void Matrix::AllAdd(F32 K)
{
    CalculateObj->Offset(data , K, col*row, data );
}
/*
 * 全部元素乘以K
 */
void Matrix::AllMul(F32 K)
{
    CalculateObj->Scale(data , K, col*row, data);
}
//**********矩阵相关计算**********
/*
 * 加上矩阵MatrixAdd
 */
void Matrix::Add(const Matrix& Mat)
{
    CalculateObj->Add(this->data, Mat.GetData(), this->row * this->col, this->data);
}
/*
 * 减去矩阵MatrixSub
 */
void Matrix::Sub(const Matrix& Mat)
{
    CalculateObj->Sub(this->data, Mat.GetData(), this->row * this->col, this->data);
}
/*
 * 右乘矩阵Mat并将结果存至MatResult
 */
void Matrix::Mul(const Matrix& Mat, Matrix& MatResult) const
{
    assert(this->col == Mat.row);
    assert(this->row == MatResult.row);
    assert(Mat.col == MatResult.col);
    F32* tempVect = new F32[Mat.row];
    F32* tempVectAns = new F32[MatResult.row];
    for (S32 i = 0; i < Mat.col; i++)
    {
        Mat.CopyColTo(i,tempVect);
        for (S32 k = 0; k < MatResult.row; k++)
            CalculateObj->MultiSum(data + k * col, tempVect, Mat.row, tempVectAns[k]);
        MatResult.CopyColFrom(i , tempVectAns);
    }
    delete tempVect;
    delete tempVectAns;
}

Matrix Matrix::operator*(const Matrix& Mat) const
{
    assert(Mat.row == col);
    Matrix MatResult(row, Mat.col);
    Mul(Mat, MatResult);
    return MatResult;
}

Matrix Matrix::operator+(const Matrix& Mat) const
{
    assert(row == Mat.row);
    assert(col == Mat.col);
    Matrix Result(*this);
    Result.Add(Mat);
    return Result;
}
Matrix Matrix::operator-(const Matrix& Mat) const
{
    assert(row == Mat.row);
    assert(col == Mat.col);
    Matrix Result(*this);
    Result.Sub(Mat);
    return Result;
}
Matrix Matrix::operator=(const Matrix& Mat)
{
    assert(row == Mat.row);
    assert(col == Mat.col);
    this->CopyFrom(Mat.data);
    return *this;
}
Vector Matrix::operator*(const Vector& vec) const
{
    assert(this->col==vec.GetDim());
    Vector VecResult(this->row);
    for (S32 i = 0; i < this->row; i++)
    {
        CalculateObj->MultiSum(this->data + i * this->col, vec.GetData(), vec.GetDim(),
                               VecResult.GetData()[i]);
    }
    return VecResult;
}
//**********常用操作**********
/*
 * 用于抽取矩阵中的某列
 */
void Matrix::ColExtract(F32* Data, S32 Offset, S32 Interval, S32 Length, F32* Result)
{
    for (S32 i = 0; i < Length; i++)
    {
        Result[i] = Data[Offset + Interval * i];
    }
}
/******************************
*******Advanced Function*******
*******************************/
/*
 * 求矩阵逆并将结果放至MatResult
 */
BOOL Matrix::Inv(Matrix& MatResult) const
{
    S32 i, j, k;
    Matrix MatTemp(this->row, this->col);
    F32 lMax, temp;
    S32 tempN;//,tempM, ;
    //tempM = MatTemp.row;
    tempN = MatTemp.col;
    MemoryCopy(MatTemp.data, this->data, this->row * this->col * sizeof(F32));
    MatResult.Clear();
    MatResult.SetDiag(1);
    for (i = 0; i < this->row; i++)
    {
        //寻找主元
        lMax = MatTemp.data[i * tempN + i];
        k = i;
        for (j = i + 1; j < this->row; j++)
        {
            if (GET_ABS(MatTemp.data[j*tempN+i]) > GET_ABS(lMax))
            {
                lMax = MatTemp.data[j * tempN + i];
                k = j;
            }
        }
        //如果主元所在行不是第i行，进行行变换
        if (k != i)
        {
            for (j = 0; j < this->col; j++)
            {
                temp = MatTemp.data[i * tempN + j];
                MatTemp.data[i * tempN + j] = MatTemp.data[k * tempN + j];
                MatTemp.data[k * tempN + j] = temp;
                temp = MatResult.data[i * MatResult.col + j];
                MatResult.data[i * MatResult.col + j] = MatResult.data[k * MatResult.col + j];
                MatResult.data[k * MatResult.col + j] = temp;
            }
        }
        //判断主元是否是0，如果是，则矩阵不是满秩矩阵，不存在逆
        if (MatTemp.data[i * tempN + i] == 0)
        {
            return false;
        }
        //使对角线元素为1
        temp = MatTemp.data[i * tempN + i];
        for (j = 0; j < this->col; j++)
        {
            MatTemp.data[i * tempN + j] = MatTemp.data[i * tempN + j] / temp;
            MatResult.data[i * tempN + j] = MatResult.data[i * tempN + j] / temp;
        }
        for (j = 0; j < this->row; j++)
        {
            if (j != i)
            {
                temp = MatTemp.data[j * tempN + i];
                for (k = 0; k < this->col; k++)
                {
                    MatTemp.data[j * tempN + k] = MatTemp.data[j * tempN + k]
                                                  - MatTemp.data[i * tempN + k] * temp;
                    MatResult.data[j * tempN + k] = MatResult.data[j * tempN + k]
                                                    - MatResult.data[i * tempN + k] * temp;
                }
            }
        }
    }
    return true;
}
/*
 * 求矩阵逆
 */
Matrix Matrix::Inv() const
{
    Matrix MatResult(this->row,this->col);
    if(Inv(MatResult))
        return MatResult;
    else
        return NULL;
}
/*
 * 求矩阵转置并将结果放至MatResult
 */
void Matrix::Tran(Matrix& MatResult) const
{
    assert(MatResult.row == this->col && MatResult.col == this->row);
    S32 i, j;
    for (i = 0; i < this->row; i++)
    {
        for (j = 0; j < this->col; j++)
            MatResult.data[j * MatResult.col + i] = this->data[i * this->col + j];
    }
}
/*
 * 求矩阵转置
 */
Matrix Matrix::Tran() const
{
    Matrix TranAns(this->col,this->row);
    Tran(TranAns);
    return TranAns;
}
/*******************************************************************************
Singular value decomposition program, svdcmp, from "Numerical Recipes in C"
(Cambridge Univ. Press) by W.H. Press, S.A. Teukolsky, W.T. Vetterling,
and B.P. Flannery
*******************************************************************************/
/*
*SVD 之用
*/
F32 pythag(F32 a, F32 b)
// compute (a2 + b2)^1/2 without destructive underflow or overflow
{
    F32 absa,absb;
    absa=fabs(a);
    absb=fabs(b);
    if (absa > absb)
        return absa*sqrt(1.0+(absb/absa)*(absb/absa));
    else
        return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+(absa/absb)*(absa/absb)));
}
/*
* SVD 奇异值分解
*/
void Matrix::Svdcmp(itr_math::Vector &W, itr_math::Matrix &V)
{
    S32 m=this->row;
    S32 n=this->col;
    S32 flag,i,its,j,jj,k,l,nm;
    double anorm,c,f,g,h,s,scale,x,y,z;
    double *w,*v,*thisdata,*rv1;
    w=new double[n]();
    v=new double[n*n]();
    thisdata=new double[m*n]();
    rv1=new double[n]();
    for(i=0; i<m; i++)
    {   for(j=0; j<n; j++)
        {
            thisdata[i*n+j]=(double)this->data[i*n+j];
        }
    }

   // itr_math::Vector rv1(n);
    g=scale=anorm=0.0; // Householder reduction to bidiagonal form
    for (i=1;i<=n;i++)
    {
        l=i+1;
        rv1[i-1]=scale*g;
        g=s=scale=0.0;
        if (i <= m)
        {
            for (k=i;k<=m;k++)/// /////!!!!!!!!!!!!!!!!!!!!!(k=i;k<=m;k++)
                scale += fabs(thisdata[(k-1)*n+i-1]);
            if (scale)
            {
                for (k=i;k<=m;k++)
                {
                    thisdata[(k-1)*n+i-1] /= scale;
                    s += thisdata[(k-1)*n+i-1]*thisdata[(k-1)*n+i-1];
                }
                f=thisdata[(i-1)*n+i-1];
                g = -SIGN(sqrt(s),f);
                h=f*g-s;
                thisdata[(i-1)*n+i-1]=f-g;
                for (j=l;j<=n;j++)
                {
                    for (s=0.0,k=i;k<=m;k++)
                        s += thisdata[(k-1)*n+i-1]*thisdata[(k-1)*n+j-1];
                f=s/h;
                for (k=i;k<=m;k++)
                    thisdata[(k-1)*n+j-1] += f*thisdata[(k-1)*n+i-1];
                }
                for (k=i;k<=m;k++)
                    thisdata[(k-1)*n+i-1] *= scale;
            }
        }
        w[i-1]=scale *g;
        g=s=scale=0.0;
        if (i<=m&&i!=n)
        {
            for (k=l;k<=n;k++)
                scale += fabs(thisdata[(i-1)*n+k-1]);
            if (scale) {
                for (k=l;k<=n;k++)
                {
                    thisdata[(i-1)*n+k-1]/= scale;
                    s += thisdata[(i-1)*n+k-1]*thisdata[(i-1)*n+k-1];
                }
                f=thisdata[(i-1)*n+l-1];
                g = -SIGN(sqrt(s),f);
                h=f*g-s;
                thisdata[(i-1)*n+l-1]=f-g;
                for (k=l;k<=n;k++)
                    rv1[k-1]=thisdata[(i-1)*n+k-1]/h;
                for (j=l;j<=m;j++)
                {
                    for (s=0.0,k=l;k<=n;k++)
                        s += thisdata[(j-1)*n+k-1]*thisdata[(i-1)*n+k-1];
                    for (k=l;k<=n;k++)
                        thisdata[(j-1)*n+k-1] += s*rv1[k-1];
                }
                for (k=l;k<=n;k++)
                    thisdata[(i-1)*n+k-1] *= scale;
            }
        }
        anorm = DMAX(anorm,(fabs(w[i-1])+fabs(rv1[i-1])));
    }

    for (i=n;i>0;i--)
    { // Accumulation of right-hand transformations.
        if (i < n)
        {
            if (g)
            {
                for (j=l;j<=n;j++) // Double division to avoid possible underflow.
                    v[(j-1)*n+i-1]=(thisdata[(i-1)*n+j-1]/thisdata[(i-1)*n+l-1])/g;
                for (j=l;j<=n;j++)
                {
                    for (s=0.0,k=l;k<=n;k++)
                        s += thisdata[(i-1)*n+k-1]*v[(k-1)*n+j-1];
                    for (k=l;k<=n;k++)
                        v[(k-1)*n+j-1] += s*v[(k-1)*n+i-1];
                }
            }
            for (j=l;j<=n;j++)
                v[(i-1)*n+j-1]=v[(j-1)*n+i-1]=0.0;
        }
        v[(i-1)*n+i-1]=1.0;
        g=rv1[i-1];
        l=i;
    }

    for (i=IMIN(m,n);i>=1;i--)
    { // Accumulation of left-hand transformations.
        l=i+1;
        g=w[i-1];
        for (j=l;j<=n;j++)
            thisdata[(i-1)*n+j-1]=0.0;
        if (g)
        {
            g=1.0/g;
            for (j=l;j<=n;j++)
            {
                for (s=0.0,k=l;k<=m;k++)
                    s += thisdata[(k-1)*n+i-1]*thisdata[(k-1)*n+j-1];
                f=(s/thisdata[(i-1)*n+i-1])*g;
                for (k=i;k<=m;k++)
                    thisdata[(k-1)*n+j-1] += f*thisdata[(k-1)*n+i-1];
            }
            for (j=i;j<=m;j++)
                thisdata[(j-1)*n+i-1] *= g;
        }
        else
            for (j=i;j<=m;j++)
                thisdata[(j-1)*n+i-1]=0.0;
        ++thisdata[(i-1)*n+i-1];
    }

    for (k=n;k>=1;k--)
    { // Diagonalization of the bidiagonal form.
        for (its=1;its<=30;its++)
        {
            flag=1;
            for (l=k;l>=1;l--)
            { // Test for splitting.
                nm=l-1; // Note that rv1[1-1] is always zero.
                if ((double)(fabs(rv1[l-1])+anorm) == anorm)
                {
                    flag=0;
                    break;
                }
                if ((double)(fabs(w[nm-1])+anorm) == anorm)
                    break;
            }
            if (flag)
            {
                c=0.0; // Cancellation of rv1[l-1], if l > 1.
                s=1.0;
                for (i=l;i<=k;i++)
                {
                    f=s*rv1[i-1];
                    rv1[i-1]=c*rv1[i-1];
                    if ((double)(fabs(f)+anorm) == anorm)
                        break;
                    g=w[i-1];
                    h=pythag(f,g);
                    w[i-1]=h;
                    h=1.0/h;
                    c=g*h;
                    s = -f*h;
                    for (j=1;j<=m;j++)
                    {
                        y=thisdata[(j-1)*n+nm-1];
                        z=thisdata[(j-1)*n+i-1];
                        thisdata[(j-1)*n+nm-1]=y*c+z*s;
                        thisdata[(j-1)*n+i-1]=z*c-y*s;
                    }
                }
            }
            z=w[k-1];
            if (l == k)
            { // Convergence.
                if (z < 0.0)
                { // Singular value is made nonnegative.
                    w[k-1] = -z;
                    for (j=1;j<=n;j++)
                        v[(j-1)*n+k-1] = -v[(j-1)*n+k-1];
                }
                break;
            }
            if (its == 30)
                printf("no convergence in 30 svdcmp iterations");
            x=w[l-1]; // Shift from bottom 2-by-2 minor.
            nm=k-1;
            y=w[nm-1];
            g=rv1[nm-1];
            h=rv1[k-1];
            f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
            g=pythag(f,1.0);
            f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
            c=s=1.0; // Next QR transformation:
            for (j=l;j<=nm;j++)
            {
                i=j+1;
                g=rv1[i-1];
                y=w[i-1];
                h=s*g;
                g=c*g;
                z=pythag(f,h);
                rv1[j-1]=z;
                c=f/z;
                s=h/z;
                f=x*c+g*s;
                g = g*c-x*s;
                h=y*s;
                y *= c;
                for (jj=1;jj<=n;jj++)
                {
                    x=v[(jj-1)*n+j-1];
                    z=v[(jj-1)*n+i-1];
                    v[(jj-1)*n+j-1]=x*c+z*s;
                    v[(jj-1)*n+i-1]=z*c-x*s;
                }
                z=pythag(f,h);
                w[j-1]=z; // Rotation can be arbitrary if z = 0.
                if (z)
                {
                    z=1.0/z;
                    c=f*z;
                    s=h*z;
                }
                f=c*g+s*y;
                x=c*y-s*g;
                for (jj=1;jj<=m;jj++)
                {
                    y=thisdata[(jj-1)*n+j-1];
                    z=thisdata[(jj-1)*n+i-1];
                    thisdata[(jj-1)*n+j-1]=y*c+z*s;
                    thisdata[(jj-1)*n+i-1]=z*c-y*s;
                }
            }
            rv1[l-1]=0.0;
            rv1[k-1]=f;
            w[k-1]=x;
        }
    }
    double tmpdouble;
    for(i=0; i<n-1; i++)
    {
        tmpdouble=w[i];
        for(j=i+1;j<n;j++)
            if(w[j]>tmpdouble)
            {
                w[i]=w[j];
                w[j]=tmpdouble;
                tmpdouble=w[i];
            }
    }

    for(i=0; i<m; i++)
    {   for(j=0; j<m; j++)
        {
            this->data[i*n+j]=(F32)thisdata[i*n+j];
        }
    }
    for(i=0; i<n; i++)
    {
        W[i]=(F32)w[i];
    }
    for(i=0; i<n; i++)
    {   for(j=0; j<n; j++)
        {
            V(i,j)=(F32)v[i*n+j];
        }
    }
    delete[] w;
    delete[] v;
    delete[] thisdata;
    delete[] rv1;
}


void Matrix::Det(F32 &reslut) /*求矩阵行列式*/
{
    assert(row>0);
    assert(col>0);
    assert(row==col);
    S32 i,j,k,p,r;
    F32 temp=1,temp1=1,s=0,s1=0;

     if(col==2)
     {
         for(i=0;i<row;i++)
            for(j=0;j<col;j++)
                if((i+j)%2)
                    temp1*=this->data[i*col+j];
                else
                    temp*=this->data[i*col+j];
         reslut=temp-temp1;
     }
     else
     {
         for(k=0;k<col;k++)
         {
             for(i=0,j=k;i<row&&j<col;i++,j++)
                temp*=this->data[i*col+j];
             if(row-i)
             {
                 for(p=row-i,r=row-1;p>0;p--,r--)
                    temp*=this->data[r*col+p-1];
             }
             s+=temp;
             temp=1;
         }

         for(k=col-1;k>=0;k--)
         {
             for(i=0,j=k;i<row&&j>=0;i++,j--)
                temp1*=this->data[i*col+j];
             if(row-i)
             {
                for(p=row-1,r=i;r<row;p--,r++)
                    temp1*=this->data[r*col+p];
             }
             s1+=temp1;
             temp1=1;
         }
         reslut=s-s1;
     }

}
void  Matrix::pinv(Matrix &MatResult) const
{
    assert(row>0);
    assert(col>0);
    assert(row>=col);
    Matrix V(col,col);
    Matrix A(row,col),U(row,row);
    Vector s(col);
    A.CopyFrom(0,0,col,row,data);
    A.Svdcmp(s,V);
    for(S32 j=0; j<row; j++)
        for(S32 k=0; k<col; k++)
            U(j,k)=A(j,k);

    S32 i;
    for( i=0; i<col; i++)
    {
        if(fabs(s[i])<tol)
            break;
    }
    if(i==0)
        MatResult.Set(0);
    else
    {
        for(S32 j=0;j<i;j++)
            s[j]=1/s[j];
        Matrix S(i,i),Vt(col,i),Ut(i,row);
        S.SetDiag(s.GetData());

        V.CopyTo(0,0,i,col,Vt.GetData());
        Matrix At(U.GetCol(),U.GetRow());
        U.Tran(At);
        At.CopyTo(0,0,row,i,Ut.GetData());

        MatResult=Vt*S*Ut;
    }
}
/// //////////////////////////////////////////////////////////////////////
/// SVD FROM BAIDU

int Matrix::svd_frombaidu(itr_math::Matrix &U, itr_math::Vector &W, itr_math::Matrix &V)
{
    int m=this->row, n=this->col;
    int withu=1, withv=1;
    int i,j,k,l,l1,iter,retval;
    double c,f,g,h,s,x,y,z,*e;
    double eps=0.000001;
    double tole=0.000001;

    double *u,*v,*q;

    v=new double[n*n]();
    e = new double[n]();
    u=new double[m*n]();
    q=new double[n]();
    retval = 0;
    /* Copy 'a' to 'u' */
    for (i=0;i<m;i++) {
        for (j=0;j<n;j++)
            u[i*n + j] = (double)this->data[i*n+j];
    }
    /* Householder's reduction to bidiagonal form. */
    g = x = 0.0;
    for (i=0;i<n;i++) {
    e[i] = g;
    s = 0.0;
    l = i+1;
    for (j=i;j<m;j++)
    s += (u[j*n+i]*u[j*n+i]);
    if (s < tole)
    g = 0.0;
    else {
    f = u[i*n+i];
    g = (f < 0) ? sqrt(s) : -sqrt(s);
    h = f * g - s;
    u[i*n+i] = f - g;
    for (j=l;j<n;j++) {
    s = 0.0;
    for (k=i;k<m;k++)
    s += (u[k*n+i] * u[k*n+j]);
    f = s / h;
    for (k=i;k<m;k++)
    u[k*n+j] += (f * u[k*n+i]);
    }
    /* end j */
    }
    /* end s */
    q[i] = g;
    s = 0.0;
    for (j=l;j<n;j++)
    s += (u[i*n+j] * u[i*n+j]);
    if (s < tole)
    g = 0.0;
    else {
    f = u[i*n+i+1];
    g = (f < 0) ? sqrt(s) : -sqrt(s);
    h = f * g - s;
    u[i*n+i+1] = f - g;
    for (j=l;j<n;j++)
    e[j] = u[i*n+j]/h;
    for (j=l;j<m;j++) {
    s = 0.0;
    for (k=l;k<n;k++)
    s += (u[j*n+k] * u[i*n+k]);
    for (k=l;k<n;k++)
    u[j*n+k] += (s * e[k]);
    }
    /* end j */
    }
    /* end s */
    y = fabs(q[i]) + fabs(e[i]);
    if (y > x)
    x = y;
    }
    /* end i */
    /* accumulation of right-hand transformations */
    if (withv) {
    for (i=n-1;i>=0;i--) {
    if (g != 0.0) {
    h = u[i*n+i+1] * g;
    for (j=l;j<n;j++)
    v[j*n+i] = u[i*n+j]/h;
    for (j=l;j<n;j++) {
    s = 0.0;
    for (k=l;k<n;k++)
    s += (u[i*n+k] * v[k*n+j]);
    for (k=l;k<n;k++)
    v[k*n+j] += (s * v[k*n+i]);
    }
    /* end j */
    }
    /* end g */
    for (j=l;j<n;j++)
    v[i*n+j] = v[j*n+i] = 0.0;
    v[i*n+i] = 1.0;
    g = e[i];
    l = i;
    }
    /* end i */
    }
    /* end withv, parens added for clarity */
    /* accumulation of left-hand transformations */
    if (withu) {
    for (i=n;i<m;i++) {
    for (j=n;j<m;j++)
    u[i*n+j] = 0.0;
    u[i*n+i] = 1.0;
    }
    }
    if (withu) {
    for (i=n-1;i>=0;i--) {
    l = i + 1;
    g = q[i];
    for (j=l;j<m;j++) /* upper limit was 'n' */
    u[i*n+j] = 0.0;
    if (g != 0.0) {
    h = u[i*n+i] * g;
    for (j=l;j<m;j++) {
    /* upper limit was 'n' */
    s = 0.0;
    for (k=l;k<m;k++)
    s += (u[k*n+i] * u[k*n+j]);
    f = s / h;
    for (k=i;k<m;k++)
    u[k*n+j] += (f * u[k*n+i]);
    }
    /* end j */
    for (j=i;j<m;j++)
    u[j*n+i] /= g;
    }
    /* end g */
    else {
    for (j=i;j<m;j++)
    u[j*n+i] = 0.0;
    }
    u[i*n+i] += 1.0;
    }
    /* end i*/
    }
    /* end withu, parens added for clarity */
    /* diagonalization of the bidiagonal form */
    eps *= x;
    for (k=n-1;k>=0;k--) {
    iter = 0;
    test_f_splitting:
    for (l=k;l>=0;l--) {
    if (fabs(e[l]) <= eps) goto test_f_convergence;
    if (fabs(q[l-1]) <= eps) goto cancellation;
    }
    /* end l */
    /* cancellation of e[l] if l > 0 */
    cancellation:
    c = 0.0;
    s = 1.0;
    l1 = l - 1;
    for (i=l;i<=k;i++) {
    f = s * e[i];
    e[i] *= c;
    if (fabs(f) <= eps) goto test_f_convergence;
    g = q[i];
    h = q[i] = sqrt(f*f + g*g);
    c = g / h;
    s = -f / h;
    if (withu) {
    for (j=0;j<m;j++) {
    y = u[j*n+l1];
    z = u[j*n+i];
    u[j*n+l1] = y * c + z * s;
    u[j*n+i] = -y * s + z * c;
    }
    /* end j */
    }
    /* end withu, parens added for clarity */
    }
    /* end i */
    test_f_convergence:
    z = q[k];
    if (l == k) goto convergence;
    /* shift from bottom 2x2 minor */
    iter++;
    if (iter > 30) {
    retval = k;
    break;
    }
    x = q[l];
    y = q[k-1];
    g = e[k-1];
    h = e[k];
    f = ((y-z)*(y+z) + (g-h)*(g+h)) / (2*h*y);
    g = sqrt(f*f + 1.0);
    f = ((x-z)*(x+z) + h*(y/((f<0)?(f-g):(f+g))-h))/x;
    /* next QR transformation */
    c = s = 1.0;
    for (i=l+1;i<=k;i++) {
    g = e[i];
    y = q[i];
    h = s * g;
    g *= c;
    e[i-1] = z = sqrt(f*f+h*h);
    c = f / z;
    s = h / z;
    f = x * c + g * s;
    g = -x * s + g * c;
    h = y * s;
    y *= c;
    if (withv) {
    for (j=0;j<n;j++) {
    x = v[j*n+i-1];
    z = v[j*n+i];
    v[j*n+i-1] = x * c + z * s;
    v[j*n+i] = -x * s + z * c;
    }
    /* end j */
    }
    /* end withv, parens added for clarity */
    q[i-1] = z = sqrt(f*f + h*h);
    c = f/z;
    s = h/z;
    f = c * g + s * y;
    x = -s * g + c * y;
    if (withu) {
    for (j=0;j<m;j++) {
    y = u[j*n+i-1];
    z = u[j*n+i];
    u[j*n+i-1] = y * c + z * s;
    u[j*n+i] = -y * s + z * c;
    }
    /* end j */
    }
    /* end withu, parens added for clarity */
    }
    /* end i */
    e[l] = 0.0;
    e[k] = f;
    q[k] = x;
    goto test_f_splitting;
    convergence:
    if (z < 0.0) {
    /* q[k] is made non-negative */
    q[k] = - z;
    if (withv) {
    for (j=0;j<n;j++)
    v[j*n+k] = -v[j*n+k];
    }
    /* end withv, parens added for clarity */
    }
    /* end z */
    }
    /* end k */
    for(i=0; i<m; i++)
    {   for(j=0; j<m; j++)
        {
            this->data[i*n+j]=(F32)u[i*n+j];
        }
    }
    for(i=0; i<n; i++)
    {
        W[i]=(F32)q[i];
    }
    for(i=0; i<n; i++)
    {   for(j=0; j<n; j++)
        {
            V(i,j)=(F32)v[i*n+j];
        }
    }
    delete[] v;
    delete[] e;
    delete[] u;
    delete[] q;
    return retval;
}

/// ////////////////////////////////////////////////////////////////////////////////////////////////////
/// SVD FROM GOOGLE


// calculates sqrt( a^2 + b^2 ) with decent precision
//double pythag(double a, double b);
double pythag(double a, double b) {
  double absa,absb;

  absa = fabs(a);
  absb = fabs(b);

  if(absa > absb)
    return(absa * sqrt(1.0 + SQR(absb/absa)));
  else
    return(absb == 0.0 ? 0.0 : absb * sqrt(1.0 + SQR(absa / absb)));
}

/*
  Modified from Numerical Recipes in C
  Given a matrix a[nRows][nCols], svdcmp() computes its singular value
  decomposition, A = U * W * Vt.  A is replaced by U when svdcmp
  returns.  The diagonal matrix W is output as a vector w[nCols].
  V (not V transpose) is output as the matrix V[nCols][nCols].
*/
int itr_math::Matrix::svdcmp( itr_math::Vector &W, itr_math::Matrix &V) {
    int nRows=this->row;
    int nCols=this->col;
    int flag,i,its,j,jj,k,l,nm;
    double anorm,c,f,g,h,s,scale,x,y,z;

    double *w,*v,*a,*rv1;
    w=new double[nCols]();
    v=new double[nCols*nCols]();
    a=new double[nRows*nCols]();
    rv1=new double[nCols]();

  for(i=0; i<nRows; i++)
    {   for(j=0; j<nCols; j++)
        {
            a[i*nCols+j]=(double)this->data[i*nRows+j];
        }
    }
  g = scale = anorm = 0.0;
  for(i=0;i<nCols;i++) {
    l = i+1;
    rv1[i] = scale*g;
    g = s = scale = 0.0;
    if(i < nRows) {
      for(k=i;k<nRows;k++) scale += fabs(a[(k)*nCols+i]);
      if(scale) {
	for(k=i;k<nRows;k++) {
	  a[(k)*nCols+i] /= scale;
	  s += a[(k)*nCols+i] * a[(k)*nCols+i];
	}
	f = a[(i)*nCols+i];
	g = -SIGN(sqrt(s),f);
	h = f * g - s;
	a[(i)*nCols+i] = f - g;
	for(j=l;j<nCols;j++) {
	  for(s=0.0,k=i;k<nRows;k++) s += a[(k)*nCols+i] * a[(k)*nCols+j];
	  f = s / h;
	  for(k=i;k<nRows;k++) a[(k)*nCols+j] += f * a[(k)*nCols+i];
	}
	for(k=i;k<nRows;k++) a[(k)*nCols+i] *= scale;
      }
    }
    w[i] = scale * g;
    g = s = scale = 0.0;
    if(i < nRows && i != nCols-1) {
      for(k=l;k<nCols;k++) scale += fabs(a[(i)*nCols+k]);
      if(scale)  {
	for(k=l;k<nCols;k++) {
	  a[(i)*nCols+k] /= scale;
	  s += a[(i)*nCols+k] * a[(i)*nCols+k];
	}
	f = a[(i)*nCols+l];
	g = - SIGN(sqrt(s),f);
	h = f * g - s;
	a[(i)*nCols+l] = f - g;
	for(k=l;k<nCols;k++) rv1[k] = a[(i)*nCols+k] / h;
	for(j=l;j<nRows;j++) {
	  for(s=0.0,k=l;k<nCols;k++) s += a[(j)*nCols+k] * a[(i)*nCols+k];
	  for(k=l;k<nCols;k++) a[(j)*nCols+k] += s * rv1[k];
	}
	for(k=l;k<nCols;k++) a[(i)*nCols+k] *= scale;
      }
    }
    anorm = FMAX(anorm, (fabs(w[i]) + fabs(rv1[i])));
  }

  for(i=nCols-1;i>=0;i--) {
    if(i < nCols-1) {
      if(g) {
	for(j=l;j<nCols;j++)
	  v[(j)*nCols+i] = (a[(i)*nCols+j] / a[(i)*nCols+l]) / g;
	for(j=l;j<nCols;j++) {
	  for(s=0.0,k=l;k<nCols;k++) s += a[(i)*nCols+k] * v[(k)*nCols+j];
	  for(k=l;k<nCols;k++) v[(k)*nCols+j] += s * v[(k)*nCols+i];
	}
      }
      for(j=l;j<nCols;j++) v[(i)*nCols+j] = v[(j)*nCols+i] = 0.0;
    }
    v[(i)*nCols+i] = 1.0;
    g = rv1[i];
    l = i;
  }

  for(i=IMIN(nRows,nCols) - 1;i >= 0;i--) {
    l = i + 1;
    g = w[i];
    for(j=l;j<nCols;j++) a[(i)*nCols+j] = 0.0;
    if(g) {
      g = 1.0 / g;
      for(j=l;j<nCols;j++) {
	for(s=0.0,k=l;k<nRows;k++) s += a[(k)*nCols+i] * a[(k)*nCols+j];
	f = (s / a[(i)*nCols+i]) * g;
	for(k=i;k<nRows;k++) a[(k)*nCols+j] += f * a[(k)*nCols+i];
      }
      for(j=i;j<nRows;j++) a[(j)*nCols+i] *= g;
    }
    else
      for(j=i;j<nRows;j++) a[(j)*nCols+i] = 0.0;
    ++a[(i)*nCols+i];

  }

  for(k=nCols-1;k>=0;k--) {
    for(its=0;its<30;its++) {
      flag = 1;
      for(l=k;l>=0;l--) {
	nm = l-1;
	if((fabs(rv1[l]) + anorm) == anorm) {
	  flag =  0;
	  break;
	}
	if((fabs(w[nm]) + anorm) == anorm) break;
      }
      if(flag) {
	c = 0.0;
	s = 1.0;
	for(i=l;i<=k;i++) {
	  f = s * rv1[i];
	  rv1[i] = c * rv1[i];
	  if((fabs(f) + anorm) == anorm) break;
	  g = w[i];
	  h = pythag(f,g);
	  w[i] = h;
	  h = 1.0 / h;
	  c = g * h;
	  s = -f * h;
	  for(j=0;j<nRows;j++) {
	    y = a[(j)*nCols+nm];
	    z = a[(j)*nCols+i];
	    a[(j)*nCols+nm] = y * c + z * s;
	    a[(j)*nCols+i] = z * c - y * s;
	  }
	}
      }
      z = w[k];
      if(l == k) {
	if(z < 0.0) {
	  w[k] = -z;
	  for(j=0;j<nCols;j++) v[(j)*nCols+k] = -v[(j)*nCols+k];
	}
	break;
      }
      if(its == 29) printf("no convergence in 30 svdcmp iterations\n");
      x = w[l];
      nm = k-1;
      y = w[nm];
      g = rv1[nm];
      h = rv1[k];
      f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
      g = pythag(f,1.0);
      f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g,f))) - h)) / x;
      c = s = 1.0;
      for(j=l;j<=nm;j++) {
	i = j+1;
	g = rv1[i];
	y = w[i];
	h = s * g;
	g = c * g;
	z = pythag(f,h);
	rv1[j] = z;
	c = f/z;
	s = h/z;
	f = x * c + g * s;
	g = g * c - x * s;
	h = y * s;
	y *= c;
	for(jj=0;jj<nCols;jj++) {
	  x = v[(jj)*nCols+j];
	  z = v[(jj)*nCols+i];
	  v[(jj)*nCols+j] = x * c + z * s;
	  v[(jj)*nCols+i] = z * c - x * s;
	}
	z = pythag(f,h);
	w[j] = z;
	if(z) {
	  z = 1.0 / z;
	  c = f * z;
	  s = h * z;
	}
	f = c * g + s * y;
	x = c * y - s * g;
	for(jj=0;jj < nRows;jj++) {
	  y = a[(jj)*nCols+j];
	  z = a[(jj)*nCols+i];
	  a[(jj)*nCols+j] = y * c + z * s;
	  a[(jj)*nCols+i] = z * c - y * s;
	}
      }
      rv1[l] = 0.0;
      rv1[k] = f;
      w[k] = x;
    }
  }


    for(i=0; i<nRows; i++)
    {   for(j=0; j<nRows; j++)
        {
            this->data[i*nCols+j]=(F32)a[i*nCols+j];
        }
    }
    for(i=0; i<nCols; i++)
    {
        W[i]=(F32)w[i];
    }
    for(i=0; i<nCols; i++)
    {   for(j=0; j<nCols; j++)
        {
            V(i,j)=(F32)v[i*nCols+j];
        }
    }
    delete[] w;
    delete[] v;
    delete[] a;
    delete[] rv1;

    return(0);
}
}
// namespace itr_math
