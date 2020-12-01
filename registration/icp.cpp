/* \author Radu Bogdan Rusu
 * adaptation Raphael Favier*/


#include "icp.h"

#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/filter.h>

#include <pcl/features/normal_3d.h>

#include <pcl/registration/icp.h>
#include <pcl/registration/icp_nl.h>
#include <pcl/registration/transforms.h>

#include <QDebug>

//convenient structure to handle our pointclouds
struct PCD
{
  PointCloudT::Ptr cloud;
  std::string f_name;

  PCD() : cloud (new PointCloudT) {};
};

struct PCDComparator
{
  bool operator () (const PCD& p1, const PCD& p2)
  {
    return (p1.f_name < p2.f_name);
  }
};


// Define a new point representation for < x, y, z, curvature >
class MyPointRepresentation : public pcl::PointRepresentation <PointNormalT>
{
  using pcl::PointRepresentation<PointNormalT>::nr_dimensions_;
public:
  MyPointRepresentation ()
  {
    // Define the number of dimensions
    nr_dimensions_ = 4;
  }

  // Override the copyToFloatArray method to define our feature vector
  virtual void copyToFloatArray (const PointNormalT &p, float * out) const
  {
    // < x, y, z, curvature >
    out[0] = p.x;
    out[1] = p.y;
    out[2] = p.z;
    out[3] = p.curvature;
  }
};


////////////////////////////////////////////////////////////////////////////////
/** \brief Align a pair of PointCloud datasets and return the result
  * \param cloud_src the source PointCloud
  * \param cloud_tgt the target PointCloud
  * \param output the resultant aligned source PointCloud
  * \param final_transform the resultant transform between source and target
  */
void pairAlign (const PointCloudT::Ptr cloud_src, const PointCloudT::Ptr cloud_tgt, PointCloudT::Ptr output, Eigen::Matrix4f &final_transform, bool downsample = false)
{
  //
  // Downsample for consistency and speed
  // \note enable this for large datasets
  PointCloudT::Ptr src (new PointCloudT);
  PointCloudT::Ptr tgt (new PointCloudT);
  pcl::VoxelGrid<PointT> grid;
  if (downsample)
  {
    qDebug() << "cloud size" << cloud_src->size();

    grid.setLeafSize (0.5, 0.5, 0.5);
    grid.setInputCloud (cloud_src);
    grid.filter (*src);

    grid.setInputCloud (cloud_tgt);
    grid.filter (*tgt);

    qDebug() << "filtered cloud size" << src->size()<<endl;
  }
  else
  {
    src = cloud_src;
    tgt = cloud_tgt;
  }


  // Compute surface normals and curvature
  PointCloudWithNormals::Ptr points_with_normals_src (new PointCloudWithNormals);
  PointCloudWithNormals::Ptr points_with_normals_tgt (new PointCloudWithNormals);

  pcl::NormalEstimation<PointT, PointNormalT> norm_est;
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
  norm_est.setSearchMethod (tree);
  norm_est.setKSearch (30);

  norm_est.setInputCloud (src);
  norm_est.compute (*points_with_normals_src);
  pcl::copyPointCloud (*src, *points_with_normals_src);

  norm_est.setInputCloud (tgt);
  norm_est.compute (*points_with_normals_tgt);
  pcl::copyPointCloud (*tgt, *points_with_normals_tgt);

  //
  // Instantiate our custom point representation (defined above) ...
  MyPointRepresentation point_representation;
  // ... and weight the 'curvature' dimension so that it is balanced against x, y, and z
  float alpha[4] = {1.0, 1.0, 1.0, 1.0};
  point_representation.setRescaleValues (alpha);

  //
  // Align
  pcl::IterativeClosestPointNonLinear<PointNormalT, PointNormalT> reg;
  reg.setTransformationEpsilon (0.1);
  // Set the maximum distance between two correspondences (src<->tgt) to 10cm
  // Note: adjust this based on the size of your datasets
  reg.setMaxCorrespondenceDistance (3);
  // Set the point representation
  reg.setPointRepresentation (pcl::make_shared<const MyPointRepresentation> (point_representation));

  reg.setInputSource (points_with_normals_src);
  reg.setInputTarget (points_with_normals_tgt);


  //
  // Run the same optimization in a loop and visualize the results
  Eigen::Matrix4f Ti = Eigen::Matrix4f::Identity (), prev, targetToSource;
  PointCloudWithNormals::Ptr reg_result = points_with_normals_src;
  reg.setMaximumIterations (5);
  for (int i = 0; i < 30; ++i)
  {
    qDebug() << "Iteration Nr." << i;

    // save cloud for visualization purpose
    points_with_normals_src = reg_result;

    // Estimate
    reg.setInputSource (points_with_normals_src);
    reg.align (*reg_result);

        //accumulate transformation between each Iteration
    Ti = reg.getFinalTransformation () * Ti;

        //if the difference between this transformation and the previous one
        //is smaller than the threshold, refine the process by reducing
        //the maximal correspondence distance
    if (std::abs ((reg.getLastIncrementalTransformation () - prev).sum ()) < reg.getTransformationEpsilon ())
      reg.setMaxCorrespondenceDistance (reg.getMaxCorrespondenceDistance () - 0.01);
    qDebug() <<"Fitness:"<<reg.getFitnessScore()<<",LastIncremental:"<<(reg.getLastIncrementalTransformation () - prev).sum ();
    prev = reg.getLastIncrementalTransformation ();
  }

    //
  // Get the transformation from target to source
  targetToSource = Ti.inverse();
  std::cout<<targetToSource<<std::endl;

  //
  // Transform target back in source frame
  pcl::transformPointCloud (*cloud_tgt, *output, targetToSource);

  //add the source to the transformed target
  *output += *cloud_src;

  final_transform = targetToSource;
 }


//void registration_icp(int argc, char** argv)
void registration_icp(PointCloudT::Ptr pcd_source,PointCloudT::Ptr pcd_target)
{
  // Load data
  std::vector<PCD, Eigen::aligned_allocator<PCD> > data;
  //loadData (argc, argv, data);

  PCD s;s.cloud = pcd_source;s.f_name = "source_las";
  PCD t;t.cloud = pcd_target;t.f_name ="target_las";
  data.push_back (s);
  data.push_back (t);


  PointCloudT::Ptr result (new PointCloudT), source, target;
  Eigen::Matrix4f GlobalTransform = Eigen::Matrix4f::Identity (), pairTransform;

  for (std::size_t i = 1; i < data.size (); ++i)
  {
    source = data[i-1].cloud;
    target = data[i].cloud;

    PointCloudT::Ptr temp (new PointCloudT);
    qDebug()<<  data[i-1].f_name.c_str () << static_cast<std::size_t>(source->size ()) << data[i].f_name.c_str () <<static_cast<std::size_t>(target->size ());
    pairAlign (source, target, temp, pairTransform, true);

    //transform current pair into the global transform
    pcl::transformPointCloud (*temp, *result, GlobalTransform);

    //update the global transform
    GlobalTransform *= pairTransform;

    //save aligned pair, transformed into the first cloud's frame
    std::stringstream ss;
    ss << i << ".pcd";
    pcl::io::savePCDFile (ss.str(), *result, true);
  }
  qDebug()<<"Finished";
}
