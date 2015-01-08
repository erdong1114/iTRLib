#ifndef CAMERAINTERCALC_H
#define CAMERAINTERCALC_H

#include "itrbase.h"

using namespace itr_math;

namespace itr_vision
{
/**
* \brief �ṩ����ڲ���������������������������任.
* \note ��������ϵ����:ԭ����ͼ�����Ͻ�,X������,Y������.
* \note �������ϵ����:ԭ���ڹ�������,X������,Y������.
* \note C2P = A = [f/dx 0 u0 0; 0 f/dy v0 0; 0 0 1 0];
* \note z[u v 1]' = A*[x y z 1]'
*/
class CameraInterCalc
{
public:
    /**
    * \brief ��ʼ����׼������ڲ�������
    */
    CameraInterCalc();
    /**
    * \brief �ͷ�
    */
    virtual ~CameraInterCalc();
    /**
    * \brief ����Clone���캯��
    */
    CameraInterCalc(const CameraInterCalc &other);
    /**
    * \brief ͨ�����ý���ķ������ɾ���
    * \param F ����(��λ:��)
    * \param dX ˮƽ��������λ�ֱ���(��λ:��/����)
    * \param dY ��ֱ��������λ�ֱ���(��λ:��/����)
    * \param u0 ˮƽ���ͼ���������(��λ:����)
    * \param v0 ��ֱ���ͼ���������(��λ:����)
    */
    void SetPara(F32 F,F32 dX,F32 dY,F32 u0,F32 v0);
    /**
    * \brief ͨ�����ý���ķ������ɾ���
    * \param fX ˮƽ���򽹾�(��λ:����)
    * \param fY ��ֱ���򽹾�(��λ:����)
    * \param u0 ˮƽ���ͼ���������(��λ:����)
    * \param v0 ��ֱ���ͼ���������(��λ:����)
    */
    void SetParaWithoutF(F32 fX,F32 fY,F32 u0,F32 v0);
    /**
    * \brief ͨ�������ӳ��нǵķ������ɾ���
    * \param degX ˮƽ�����ӳ��н�(��λ:�Ƕ�)
    * \param degY ��ֱ�������ӳ��н�(��λ:�Ƕ�)
    * \param u0 ˮƽ���ͼ���������(��λ:����)
    * \param v0 ��ֱ���ͼ���������(��λ:����)
    */
    void SetPara(F32 degX,F32 degY,F32 u0,F32 v0);
    /**
    * \brief ������굽���������ת��
    * \param CameraPoint ������������ϵ����([x,y,z],��λ:��)
    * \param PixelPoint �������������ϵ([u,v,1],��λ:����)
    * \param ����ֵ��1Ϊ�ɹ���0Ϊʧ��
    */
    BOOL CalcC2P(const Vector &CameraPoint,Vector &PixelPoint);
    /**
    * \brief �������굽��������ת��
    * \param PixelPoint �������������ϵ([u,v,1],��λ:����)
    * \param Z Ϊ��������µļٶ�����
    * \param CameraPoint ������������ϵ����([x,y,z,1],��λ:��)
    * \param ����ֵ��1Ϊ�ɹ���0Ϊʧ��
    */
    BOOL CalcP2C(const Vector &PixelPoint,F32 Z,Vector &CameraPoint);
    /**
    * \brief ��������������Ŀ��������ļн�
    * \param Point �������������
    * \param Ang_x Ŀ����������x����ļн�
    * \param Ang_y Ŀ����������y����ļн�
    */
    void CalcAng(const Point2D &Point, F32 &Ang_x, F32 &Ang_y);
    /**
    * \note ������굽���������ת�����󡪡�����ڲ���
    */
    Matrix MatC2P;
    /**
    * \note �������굽��������ת������
    */
    Matrix MatP2C;
protected:
private:
    /**
    * \note �ڵ���SetPara����Ч
    */
    BOOL isMatC2PAvailable;
    /**
    * \note �ڵ���CalcP2Cʱ�Զ�ת����ʹ����Ч(����isMatC2PAvailable��Чʱ����Ч)
    */
    BOOL isMatP2CAvailable;
};
}

#endif // CAMERAINTERCALC_H
