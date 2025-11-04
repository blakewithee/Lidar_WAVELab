/*
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
// segmentation with real scans

int main ()
{

  // Point cloud file instantiation
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  if (pcl::io::loadPCDFile<pcl::PointXYZ> ("/home/bmw25494/Desktop/LiDAR_Scans/SetupTest.pcd", *cloud) == -1) // load the file
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
    if (pt.x > 0.07 || pt.y > 0.9 || pt.y < -0.57 || pt.z > 4 || pt.z < 1.5) // e.g. remove all pts above y = 1.2
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

  //for (const auto& point: *cloud)
  for (const auto& point : cloud->points)

    std::cout << "    " << point.x

              << " "    << point.y

              << " "    << point.z

              << " "    << point.reflectance << std::endl;
  
  // Visualizing point cloud
  pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
  viewer.showCloud (cloud);
  while (!viewer.wasStopped ()) {
  }
  
  // End
  return (0);
}
*/
#define PCL_NO_PRECOMPILE

#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/register_point_struct.h>
#include <pcl/filters/passthrough.h>


// It's good practice to include these for the visualization loop
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// Your custom point type definition (no changes needed here)
struct PointXYZReflectance
{
    PCL_ADD_POINT4D;
    uint16_t reflectance;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} EIGEN_ALIGN16;

POINT_CLOUD_REGISTER_POINT_STRUCT(PointXYZReflectance,
    (float, x, x)
    (float, y, y)
    (float, z, z)
    (uint16_t, reflectance, reflectance)
)

int main()
{
    pcl::PointCloud<PointXYZReflectance>::Ptr cloud(new pcl::PointCloud<PointXYZReflectance>);

    if (pcl::io::loadPCDFile<PointXYZReflectance>("/home/bmw25494/Desktop/LiDAR_Scans/SetupTest.pcd", *cloud) == -1) {
        PCL_ERROR("Couldn't read file\n");
        return -1;
    }
    
    // Create a new cloud to store the filtered results
    pcl::PointCloud<PointXYZReflectance>::Ptr filtered_cloud(new pcl::PointCloud<PointXYZReflectance>);

    // --- Filtering Example ---
    pcl::PassThrough<PointXYZReflectance> pass;
    pass.setInputCloud(cloud);

    // Filter along the X axis (height)
    pass.setFilterFieldName("x");
    pass.setFilterLimits(-0.6, -0.325);
    pass.filter(*filtered_cloud);

    // Now filter along Y (width)
    pcl::PassThrough<PointXYZReflectance> pass_y;
    pass_y.setInputCloud(filtered_cloud);
    pass_y.setFilterFieldName("y");
    pass_y.setFilterLimits(-0.57, -0.25);
    pass_y.filter(*filtered_cloud);

    // Filter along Z (length)
    pcl::PassThrough<PointXYZReflectance> pass_z;
    pass_z.setInputCloud(filtered_cloud);
    pass_z.setFilterFieldName("z");
    pass_z.setFilterLimits(3, 4);
    pass_z.filter(*filtered_cloud);

    std::cout << "Original cloud has " << cloud->size() << " points\n";
    std::cout << "Filtered cloud has " << filtered_cloud->size() << " points\n";

    // --- Save the filtered cloud ---
    pcl::io::savePCDFileBinary("/home/bmw25494/Desktop/LiDAR_Scans/FilteredCloud.pcd", *filtered_cloud);
    
    for (const auto& point : filtered_cloud->points)

    std::cout << "    " << point.x

              << " "    << point.y

              << " "    << point.z

              << " "    << point.reflectance << std::endl;

    // --- Visualization Setup ---
    pcl::visualization::PCLVisualizer::Ptr viewer (new pcl::visualization::PCLVisualizer ("Reflectance Viewer"));
    viewer->setBackgroundColor(0.1, 0.1, 0.1);

    pcl::visualization::PointCloudColorHandlerGenericField<PointXYZReflectance> reflectance_color_handler(filtered_cloud, "reflectance");

    viewer->addPointCloud<PointXYZReflectance>(filtered_cloud, reflectance_color_handler, "reflectance cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "reflectance cloud");

    viewer->resetCamera();

    while (!viewer->wasStopped())
    {
        // Tells the viewer to process events and update the screen
        viewer->spinOnce(100);
    }
}
