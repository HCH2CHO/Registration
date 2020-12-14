#include <pcl/memory.h>  // for pcl::make_shared
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/point_representation.h>

#include <pcl/io/pcd_io.h>

//convenient typedefs
typedef pcl::PointXYZ Point_T;
typedef pcl::PointCloud<Point_T> PointCloud_T;
typedef pcl::PointNormal PointNormalT;
typedef pcl::PointCloud<PointNormalT> PointCloudWithNormals;


/**
 * @brief registration_icp      通过ICP算法计算转换矩阵
 * @param pcd_source
 * @param pcd_target
 * @param GlobalTransform       转换矩阵
 */
void registration_icp(PointCloud_T::Ptr pcd_source,PointCloud_T::Ptr pcd_target,Eigen::Matrix4f &GlobalTransform);
