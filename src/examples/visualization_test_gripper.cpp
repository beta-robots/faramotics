
//faramotics includes
#include "../rangeImage.h"

//std
#include <iostream>


//function to move the model
void moveModel(unsigned int ii, Eigen::Transform<double,3,Eigen::Affine> & _pose)
{
    double aa; 
    
    if (ii<=40) 
    {
        aa = (double)ii/100.;
        _pose = Eigen::AngleAxisd( M_PI, Eigen::Vector3d(sin(aa),0,cos(aa)) );
        _pose.translation() = Eigen::Vector3d(1200+(double)ii*5,0,0); 
    }
    
    if ( (ii>40) && (ii<=80) ) 
    {
        aa = (double)ii/100.;
        _pose = Eigen::AngleAxisd( M_PI, Eigen::Vector3d(sin(aa),0,cos(aa)) );
        _pose.translation() = Eigen::Vector3d(1200+(double)40*5,ii-40,0); 
    }
}


int main(int argc, char** argv)
{
    CrangeImage *myDepthCamera;
    Window *myDepthImage;    
    Eigen::Transform<double,3,Eigen::Affine> camera_pose; //camera wrt the model
    vector<float> myDepths;
    string modelFileName;
    unsigned int ii;
    timeval t1,t2;
    double dt;
        
    //model file name
    modelFileName = "/home/andreu/Desktop/Robotiq_F2_85_LD.stl";
    
    //set initial view point
    camera_pose = Eigen::AngleAxisd( M_PI, Eigen::Vector3d(0,0,1) );
    camera_pose.translation() = Eigen::Vector3d(1200,0,0); 
    
    //glut initialization
    faramotics::initGLUT(argc, argv);
    
    //create depth camera and load 3D model
    myDepthCamera = new CrangeImage(ENSENSO_N35);
    myDepthCamera->loadAssimpModel(modelFileName);
    myDepthCamera->printRenderParameters();
    
    //Window to visualize depth image
    myDepthImage = new Window(myDepthCamera->getNumHorizontalPoints(), myDepthCamera->getNumVerticalPoints(), true, "Depth Image");

    //main loop after a pause
    sleep(1);
    for (ii=0; ii<100; ii++)
    {        
        //move
        moveModel(ii,camera_pose); 
        
        //compute depth image
        myDepths.clear();
        myDepthCamera->depthImage(camera_pose,myDepths);
                
        //draw depth image
        myDepthImage->drawDepths(&myDepths.at(0),myDepthCamera->getZNear(),myDepthCamera->getZFar());
        
        //relax
        usleep(200000);
    }
    
    //delete objects
    delete myDepthCamera;
    delete myDepthImage;

    //return
    return 0;
}
