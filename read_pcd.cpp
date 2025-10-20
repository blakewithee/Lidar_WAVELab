#include <iostream>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h> 
#include <pcl/filters/extract_indices.h>

// Commands:
// in build directory
// cmake ..
// make
// ./pcd_read

// Next Steps: segmentation of main file into another file, real time updates

int main ()
{

  // Point cloud file instantiation
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  if (pcl::io::loadPCDFile<pcl::PointXYZ> ("/home/bmw25494/Desktop/LiDAR_Scans/Test.pcd", *cloud) == -1) // load the file
  {
    PCL_ERROR ("Couldn't read file Test.pcd \n");

    return (-1);
  }

  // Filtering point cloud data (manual bounding)
  pcl::PointIndices::Ptr inliers(new pcl::PointIndices());
  pcl::ExtractIndices<pcl::PointXYZ> extract;
  for (int i = 0; i < (*cloud).size(); i++)
  {
    pcl::PointXYZ pt(cloud->points[i].x, cloud->points[i].y, cloud->points[i].z);
    if (pt.y > 1.2) // e.g. remove all pts above y = 1.2
    {
      inliers->indices.push_back(i);
    }
  }
  extract.setInputCloud(cloud);
  extract.setIndices(inliers);
  extract.setNegative(true);
  extract.filter(*cloud);
  
  // Printing xyz point data
  std::cout << "Loaded "

            << cloud->width * cloud->height

            << " data points from test_pcd.pcd with the following fields: "

            << std::endl;

  for (const auto& point: *cloud)

    std::cout << "    " << point.x

              << " "    << point.y

              << " "    << point.z << std::endl;
  
  // Visualizing point cloud
  pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
  viewer.showCloud (cloud);
  while (!viewer.wasStopped ()) {
  }
  
  // End
  return (0);
}