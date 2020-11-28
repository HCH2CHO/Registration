#include "mainwindow.h"

#include <QApplication>
#include "registration/LasFile.h"
#include "registration/PointCloudObject.h"
#include "registration/icp.h"
#include "registration/artifical_registration.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //las数据读取
    std::string strFilePathName = "C:\\Users\\jiaqy11\\Desktop\\";
    std::string las1 = "039-10011014200921-143558-00309.las";
    std::string las2 = "039-10011014200921-152641-00451.las";
    PointCloudObject pointCloud_target;
    pointCloud_target.m_pointCloud.reset(new PointCloudT);

    PointCloudObject pointCloud_source;
    pointCloud_source.m_pointCloud.reset(new PointCloudT);

    LasFile::ReadLas(strFilePathName+las1, pointCloud_source.m_pointCloud);
    LasFile::ReadLas(strFilePathName+las2, pointCloud_target.m_pointCloud);

    std::cout << pointCloud_source.m_pointCloud->points[0].x<<std::endl;
    std::cout << pointCloud_source.m_pointCloud->points[0].y<<std::endl;


    //registration_icp(pointCloud_source.m_pointCloud,pointCloud_target.m_pointCloud);

    //手动配准
    std::vector<Vector3d> source_input_point;
    std::vector<Vector3d> target_input_point;
    Vector3d p1(198185.7896,2510754.467,72.158012);
    Vector3d p2(198199.7971,2510761.976,72.217613);
    Vector3d p3(198150.5581,2510796.427,71.690308);
    Vector3d p4(197957.8393,2510942.516,69.409859);

    Vector3d p5(197874.3099,2511007.762,68.30162);
    Vector3d p6(197466.1709,2511231.661,62.068573);
    Vector3d p7(197333.267,2511272.127,60.209713);
    Vector3d p8(197245.8874,2511208.406,59.116413);

    Vector3d p9(197661.4316,2511133.433,64.967628);
    Vector3d p10(197848.3018,2511018.094,67.95533);
    Vector3d p11(198106.7492,2510817.743,71.240059);
    Vector3d p12(197241.6241,2510933.692,58.310509);

    Vector3d p13(198237.0484,2510668.271,73.287132);
    Vector3d p14(198099.4792,2510513.97,70.109764);
    Vector3d p15(197763.3733,2510663.936,66.121788);
    Vector3d p16(197675.2731,2510726.628,65.350227);
//    Vector3d test(1,10,2);

//    Vector3d t1 =p1+test;
//    Vector3d t2 =p2+test;
//    Vector3d t3 =p3+test;
//    Vector3d t4 =p4+test;

    Vector3d t1(198185.5138,2510754.849,72.900024);
    Vector3d t2(198199.9969,2510761.985,72.987228);
    Vector3d t3(198150.8439,2510796.195,72.479225);
    Vector3d t4(197957.684,2510942.419,70.217026);

    Vector3d t5(197874.2676,2511007.65,69.100029);
    Vector3d t6(197466.4156,2511231.628,62.853928);
    Vector3d t7(197333.0673,2511272.079,60.900028);
    Vector3d t8(197245.9148,2511208.437,59.901028);

    Vector3d t9(197661.4426,2511133.387,65.733528);
    Vector3d t10(197848.3054,2511018.309,68.700027);
    Vector3d t11(198106.7282,2510817.876,72.093025);
    Vector3d t12(197241.5316,2510933.669,59.055729);

    Vector3d t13(198237.3199,2510668.536,74.10083);
    Vector3d t14(198099.5552,2510514.097,71.037727);
    Vector3d t15(197763.4707,2510663.934,67.072029);
    Vector3d t16(197675.0571,2510726.636,65.990028);

    source_input_point.push_back(p1);
//    source_input_point.push_back(p2);
//    source_input_point.push_back(p3);
//    source_input_point.push_back(p4);

    source_input_point.push_back(p5);
//    source_input_point.push_back(p6);
//    source_input_point.push_back(p7);
//    source_input_point.push_back(p8);
    source_input_point.push_back(p9);
//    source_input_point.push_back(p10);
//    source_input_point.push_back(p11);
//    source_input_point.push_back(p12);
    source_input_point.push_back(p13);
//    source_input_point.push_back(p14);
//    source_input_point.push_back(p15);
//    source_input_point.push_back(p16);

    target_input_point.push_back(t1);
//    target_input_point.push_back(t2);
//    target_input_point.push_back(t3);
//    target_input_point.push_back(t4);

    target_input_point.push_back(t5);
//    target_input_point.push_back(t6);
//    target_input_point.push_back(t7);
//    target_input_point.push_back(t8);
    target_input_point.push_back(t9);
//    target_input_point.push_back(t10);
//    target_input_point.push_back(t11);
//    target_input_point.push_back(t12);
    target_input_point.push_back(t13);
//    target_input_point.push_back(t14);
//    target_input_point.push_back(t15);
//    target_input_point.push_back(t16);

    //设置区域中心点
    Vector3d center(197241.5316,2510933.669,59.055729);
    for(auto &item:source_input_point)
    {
        item = item - center;
    }
    for(auto &item:target_input_point)
    {
        item = item - center;
    }
    //target为控制点/目标点
    calculateMatrix(source_input_point,target_input_point);

    //MainWindow w;
    //w.show();
    return a.exec();
}
