#include "artifical_registration.h"
#include <iostream>

//target为控制点/目标点
//七参数适用范围，xyz轴旋转角度较小（一般小于10秒,sinx~x），尺度范围较小（<100km，xyz平移值小于1000）
//七参数变换arg=7
//六参数变换arg=6
//四参数变换arg=5
void calculateMatrix(std::vector<Vector3d> source_input_point,std::vector<Vector3d> target_input_point,int arg)
{
    double dx=0;
    double dy=0;
    double dz=0;
    double wx=0;
    double wy=0;
    double wz=0;
    double k=0;

    VectorXd trans_Matrix(arg);
    if(arg == 7)
    {
        trans_Matrix <<dx,dy,dz,k,wx,wy,wz;
    }
    else if(arg == 6)
    {
        trans_Matrix <<dx,dy,dz,wx,wy,wz;
    }
    else if(arg == 5)
    {
        trans_Matrix <<dx,dy,dz,sin(wz),cos(wz);
    }


    int num_s = source_input_point.size();
    int num_t = target_input_point.size();
    if(num_s != num_t)
    {
        return;
    }

    std::vector<Vector3d> L_vector;
    for(int i=0;i<num_s;i++)
    {
        L_vector.push_back(target_input_point[i]-source_input_point[i]);
    }

    std::vector<MatrixXd> B_vector;
    for(int i=0;i<num_s;i++)
    {
        MatrixXd Bi(3,arg);
        double X = source_input_point[i][0];
        double Y = source_input_point[i][1];
        double Z = source_input_point[i][2];

        if(arg == 7)
        {
            Bi << 1,0,0,X,0,-Z,Y,
                  0,1,0,Y,Z,0,-X,
                  0,0,1,Z,-Y,X,0;
        }
        else if(arg == 6)
        {
            Bi << 1,0,0,0,-Z,Y,
                  0,1,0,Z,0,-X,
                  0,0,1,-Y,X,0;
        }
        else if(arg == 5)
        {
            Bi << 1,0,0,Y,X,
                  0,1,0,-X,Y,
                  0,0,1,0,0;
        }
        B_vector.push_back(Bi);
    }

    VectorXd L;
    MatrixXd B;
    L = VectorXd::Zero(3*num_s);
    B = MatrixXd::Zero(3*num_s,arg);
    for(int i=0;i<num_s;i++)
    {
        L.block(i*3,0,3,1)<<L_vector[i];
        B.block(i*3,0,3,arg)<<B_vector[i];
    }

    double r1=0;
    double r2=1;
    VectorXd trans_Matrix_mid = trans_Matrix;
    //std::cout<<trans_Matrix_mid<<std::endl;
    int iteration=0;
    while(r1 != r2)
    {
        trans_Matrix_mid = trans_Matrix;
        trans_Matrix = trans_Matrix_mid+( (B.transpose()*B).inverse() )*B.transpose()*(L-B*trans_Matrix_mid);
        //trans_Matrix(3) = 0;

        MatrixXd R1 = (B*trans_Matrix_mid).transpose()*(B*trans_Matrix_mid)-2*(B*trans_Matrix_mid).transpose()*L;
        MatrixXd R2 = (B*trans_Matrix).transpose()*(B*trans_Matrix)-2*(B*trans_Matrix).transpose()*L;
        r1=abs(R1(0));
        r2=abs(R2(0));
        std::cout<<r1 <<","<<r2<<","<<r1 - r2<<std::endl;
        //结束死循环
        iteration++;
        if(iteration>100)
            break;
    }
    std::cout<<trans_Matrix_mid<<std::endl;
}
