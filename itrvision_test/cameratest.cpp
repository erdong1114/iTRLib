#include "cameratest.h"
#include "stdio.h"
#define end_of_pos1 780
#define end_of_pos2 1006

using namespace itr_vision;

void printMatrix(itr_math::Matrix a)
{
    printf("The Matrix is:\n");
    for (int i = 0; i < a.GetRow(); ++i)
    {
        for (int j = 0; j < a.GetCol(); ++j)
        {
            printf("%f ", a(i, j));
        }
        printf("\n");
    }
    printf("\n");
}

void printVector(itr_math::Vector a)
{
    printf("The Vector is\n");
    for(int i = 0; i<a.GetDim(); i++)
    {
        printf("%f ",a.GetData()[i]);
    }
    printf("\n");
    printf("\n");
}
void TestCamerain()
{
    CameraInterCalc cam_in;
    Vector PixelPoint(3),CameraPoint(4);
    cam_in.SetPara(0.03,0.0002,0.0002,150,150);
    printf("cam_in.MatP2C\n");
    printMatrix(cam_in.MatP2C);
    printf("cam_in.MatC2P\n");
    printMatrix(cam_in.MatC2P);

    TestCameraexternal(cam_in);
}
void TestCameraexternal(itr_vision::CameraInterCalc &camera_in)
{
    VectorFeaturePoint pointlist1[end_of_pos1];
    VectorFeaturePoint pointlist2[end_of_pos2];
    VectorFeaturePoint mpointlist1[72],mpointlist2[72];
    itr_math::Matrix Hstd(3,3);
    /// read data
    FILE *FP_pos1,*FP_pos2,*FP_desc1,*FP_desc2,*FP_sigma2,*FP_H,*FP_matched;
    FP_pos1 = fopen("pos1.txt", "r");
    FP_pos2 = fopen("pos2.txt", "r");
    FP_desc1 = fopen("desc1.txt", "r");
    FP_desc2 = fopen("desc2.txt", "r");
    FP_sigma2 = fopen("sigmas2.txt", "r");
    FP_matched = fopen("matched.txt", "r");
    FP_H = fopen("H.txt", "r");
    assert(FP_pos1!=NULL);
    assert(FP_pos2!=NULL);
    assert(FP_desc1!=NULL);
    assert(FP_desc2!=NULL);
    assert(FP_sigma2!=NULL);
    assert(FP_H!=NULL);
    assert(FP_matched!=NULL);

    for(S32 i=0; i<end_of_pos1; i++)
    {
        fscanf(FP_pos1,"%f %f",&(pointlist1[i].X),&(pointlist1[i].Y));
        for(S32 j=0; j<128; j++)
        {
            fscanf(FP_desc1,"%f",&pointlist1[i].Feature[j]);
        }
    }
    for(S32 i=0; i<end_of_pos2; i++)
    {
        fscanf(FP_pos2,"%f %f",&pointlist2[i].X,&pointlist2[i].Y);
        fscanf(FP_sigma2,"%f",&pointlist2[i].Quality);
        for(S32 j=0; j<128; j++)
        {
            fscanf(FP_desc2,"%f",&pointlist2[i].Feature[j]);
        }
    }
    for(S32 i=0; i<3; i++)
        for(S32 j=0; j<3; j++)
            fscanf(FP_H,"%f",&Hstd(i,j));
    for(S32 i=0; i<72; i++)
    {
        fscanf(FP_matched,"%f %f %f %f %f",&mpointlist1[i].X,&mpointlist1[i].Y,&mpointlist2[i].X,&mpointlist2[i].Y,&mpointlist2[i].Quality);
        mpointlist1[i].ID=i;
        mpointlist2[i].ID=i;
    }
    fclose(FP_pos1);
    fclose(FP_pos2);
    fclose(FP_desc1);
    fclose(FP_desc2);
    fclose(FP_sigma2);
    fclose(FP_H);
    fclose(FP_matched);

///     qualify data
//    assert(pointlist1[779].X==739&&pointlist1[779].Y==644);
//    assert(pointlist2[1005].X==233&&pointlist2[1005].Y==673);
//    assert(GET_ABS(pointlist2[1005].Quality-2.8284)<0.0001);
//    assert(GET_ABS(pointlist1[779].Feature[125]-0.036629044)<0.001);
//    assert(GET_ABS(pointlist2[1005].Feature[125]-0.13691367)<0.001);
//    assert(GET_ABS(Hstd(2,2)+0.00485)<0.001);

    itr_vision::FeatureMatch domatch;
    S32 matched_num;
    domatch.FeatureMatchDo(pointlist1,end_of_pos1,pointlist2,end_of_pos1,matched_num);
//    printf("matched number: %d\n",matched_num);
//    for(S32 i=0; i<end_of_pos1; i++)
//    {
//        if(pointlist1[i].ID!=-1)
//        {   printf("matched ID: %d\n",pointlist1[i].ID);
//            printf("pos1 : %f\t%f\tpos2 : %f\t%f\n",pointlist1[i].X,pointlist1[i].Y,pointlist2[pointlist1[i].ID].X,pointlist2[pointlist1[i].ID].Y);
//        }
//    }

    Vector pos(3),pos1(3),pos2(3),posstd(2);
    posstd[0]=583;
    posstd[1]=269;
    pos[0]=638;
    pos[1]=59;
    pos[2]=1;
    pos1=Hstd*pos;
    pos1.Mul(1/pos1[2]);
    CameraExterCalc camera_out;
    F32 ratio_x=0,ratio_y=0;

    for(S32 k=0; k<10000; k++)
    {
        camera_out.CalcH(pointlist1,end_of_pos1,pointlist2,end_of_pos1,matched_num);
        //camera_out.CalcH(mpointlist1,72,mpointlist2,72,72);
    //    printf("camera_out.H\n");
    //    printMatrix(camera_out.H);
    //    printf("Hstd\n");
    //    printMatrix(Hstd);

        pos2=camera_out.H*pos;
        pos2.Mul(1/pos2[2]);
        ratio_x+=GET_ABS(posstd[0]-pos2[0]);
        ratio_y+=GET_ABS(posstd[1]-pos2[1]);
    }
    ratio_x=ratio_x/(10000*posstd[0]);
    ratio_y=ratio_y/(10000*posstd[1]);
    printf("std pos:\n");
    printVector(pos1);
    printf("my pos:\n");
    printVector(pos2);
    printf("dis ratio :\nx : %f\ny : %f\n",ratio_x,ratio_y);

    camera_out.CalcMotion(camera_in,10);
}
void CameraTest()
{
    TestCamerain();

    TRACE_INFO("OK cameratest!!");
}
