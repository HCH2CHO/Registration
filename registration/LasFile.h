/**
 * las文件的相关处理
 * Created by zzw on 8/20/20.
 */
#ifndef POINTCLOUD_LASFILE_H
#define POINTCLOUD_LASFILE_H

#include <string>
#include <iostream>
#include <sstream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/common/common.h>
#include <pcl/common/transforms.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>

typedef pcl::PointXYZRGBA PointType;

class LasFile {
public:
    LasFile();
    virtual ~LasFile();

public:
    /**
     * 读取las文件信息
     * @param strLasFile
     * @param cloud
     */
    void ReadLasHeader(std::string strLasFile, pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud);

    /**
     * 读取las文件
     * @param strLasPath
     * @param cloud
     */
    static void ReadLas(std::string strLasPath, pcl::PointCloud<PointType> &cloud);

    /**
     * 读取las文件
     * @param strLasPath
     * @param cloud
     */
    static void ReadLas(std::string strLasPath, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

    /**
     * 读取las文件
     * @param strLasPath
     * @param cloud
     */
    void ReadLas(std::string strLasPath, pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud);

    /**
     * 点云切割，将一个点云文件las按点数切分成多个小的las文件
     * @param strInputFilePath 输入文件全路径
     * @param strOutputFilePath 输出文件路径
     */
    void DivisionLasFileByPointNum(std::string strInputFilePath, std::string strOutputFilePath);
};


#endif //POINTCLOUD_LASFILE_H
