/*
 * desc: 点云
 * date: 2020.10.30
 * author: zhangzw
*/
//////////////////////////////////////////////////////////////////////////
#ifndef _POINTCLOUDOBJECT_H_H
#define _POINTCLOUDOBJECT_H_H

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <string>

//typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

class PointCloudObject
{
public:
	PointCloudObject();
	virtual ~PointCloudObject();

	void initial();

public:
	PointCloudT::Ptr m_pointCloud;
	std::string m_strFilePathName;  //点云的全路径文件名
	std::string m_strFileName;      //点云的文件名
	std::string m_strDirectory;
	bool m_bVisible;                //点云在 viewer 中是否可见

	// 地图坐标
	double m_dMapMaxX;
	double m_dMapMinX;
	double m_dMapMaxY;
	double m_dMapMinY;
};

#endif
