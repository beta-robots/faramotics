
//faramotics includes
#include "../rangeImage.h"

//PCL
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>

//std
#include <iostream>

int main(int argc, char** argv)
{
    CrangeImage *depth_camera;
    Eigen::Transform<double,3,Eigen::Affine> camera_pose; //camera wrt the model
    string modelFileName;
    std::vector<float> scloud_x, scloud_y, scloud_z; 
    pcl::PointCloud<pcl::PointXYZ>::Ptr synthetic_cloud(new pcl::PointCloud<pcl::PointXYZ>()); //TODO: Should be with normals if faramotics can directly set them
    unsigned correct_points = 0; 
        
    //model file name
    modelFileName = "/home/andreu/Desktop/Robotiq_F2_85_LD.stl";
    
    //set initial view point
    camera_pose = Eigen::AngleAxisd( M_PI, Eigen::Vector3d(0,0,1) );
    camera_pose.translation() = Eigen::Vector3d(1000,0,0); 
    
    //glut initialization
    faramotics::initGLUT(argc, argv);
    
    //create depth camera and load 3D model
    depth_camera = new CrangeImage(ENSENSO_N35);
    depth_camera->loadAssimpModel(modelFileName);
    depth_camera->printRenderParameters();
    
    //Fill a point cloud
    depth_camera->pointCloud(camera_pose, scloud_x, scloud_y, scloud_z);
    synthetic_cloud->width = scloud_x.size(); 
    synthetic_cloud->height = 1; //unorganized point cloud
    synthetic_cloud->resize(scloud_x.size()); //allocate memory for all points
    for (unsigned int ii=0; ii<synthetic_cloud->points.size(); ii++)
    {
        //check nans and infs
        if ( ( !std::isnan(scloud_x[ii]) ) && ( !std::isinf(scloud_x[ii]) ) )    
        {                    
            //set points to cloud, taking into account optical axis in faramotics is X, instead of Z
            synthetic_cloud->points.at(ii).x = scloud_x[ii]/1000.; 
            synthetic_cloud->points.at(ii).y = scloud_y[ii]/1000.; 
            synthetic_cloud->points.at(ii).z = scloud_z[ii]/1000.; 
            correct_points ++; 
        }
    }   
    synthetic_cloud->width = correct_points; 
    synthetic_cloud->resize(correct_points); 
    std::cout << "scloud_x.size(): " << scloud_x.size() << "; correct_points: " << correct_points << std::endl; 
    
    //visualize the point cloud
    pcl::visualization::PCLVisualizer viewer_("Faramotics Point Cloud");  //visualization window
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> p_cloud_color_handler(synthetic_cloud, 255, 255, 255);
    viewer_.addPointCloud (synthetic_cloud, p_cloud_color_handler, "Faramotics Point Cloud");
    viewer_.addCoordinateSystem (1.0);
    viewer_.setBackgroundColor(0.05, 0.05, 0.05, 0); // Setting background to a dark grey
    viewer_.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "Faramotics Point Cloud");
    viewer_.setPosition(300,200); // Setting visualiser window position

    // Display the visualiser until 'q' key is pressed
    while (!viewer_.wasStopped ()) { 
        viewer_.spinOnce ();
    }    
    //delete objects
    delete depth_camera;

    //return
    return 0;
}
