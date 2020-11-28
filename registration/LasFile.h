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

//typedef pcl::PointXYZRGBA PointType;

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
    static void ReadLas(std::string strLasPath, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

//    /**
//     * 读取las文件
//     * @param strLasPath
//     * @param cloud
//     */
//    void ReadLas(std::string strLasPath, pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud);

};


#endif //POINTCLOUD_LASFILE_H
