#include "mainwindow.h"

#include <QApplication>
#include "registration/LasFile.h"
#include "registration/PointCloudObject.h"
#include "registration/icp.h"
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

    registration_icp(pointCloud_source.m_pointCloud,pointCloud_target.m_pointCloud);


    //MainWindow w;
    //w.show();
    return a.exec();
}
