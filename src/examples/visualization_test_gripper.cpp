
//faramotics includes
#include "../src/window.h"
#include "../src/sceneRender.h"
#include "../src/dynamicSceneRender.h"
#include "../src/rangeSector.h"
#include "../src/rangeScan2D.h"
#include "../src/rangeImage.h"

//std
#include <iostream>


//function to move the model
void moveModel(unsigned int ii, Pose & _pose)
{
    if (ii<=40) {
        _pose.moveForward(0.1); }

    if ( (ii>40) && (ii<=80) ) {
        _pose.moveForward(0.1);
        _pose.rotateAboutX(0.01);  }
  
}


int main(int argc, char** argv)
{
    CdynamicSceneRender *myRender;
    CrangeImage *myDepthCamera;
    Window *myDepthImage;    
    Pose camera_pose;
    Pose gripper_pose;
    vector<double> myDepths;
    string modelFileName;
    unsigned int ii;
    timeval t1,t2;
    double dt;
        
    //init model and initial view point
    modelFileName = "/home/andreu/Desktop/Robotiq_F2_85_LD.stl";
    //camera_pose.setPose(200,100,100,M_PI,-0.6,0);
    camera_pose.setPose(1400,100,100,M_PI,-0.6,0);
    
    //glut initialization
    faramotics::initGLUT(argc, argv);
    
    //create a viewer for the 3D model and scan points
//     myRender = new CdynamicSceneRender(1200,700,90*M_PI/180.,90*(700./1200.)*(M_PI/180.),10.,600.);
//     myRender->loadAssimpModel(modelFileName,true); //with wireframe
//     myRender->printRenderParameters();
    
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
        
        //Set view point and render the scene
//         myRender->setViewPoint(camera_pose);
//         myRender->render();        
        
        //compute depth image
        myDepths.clear();
        myDepthCamera->depthImage(camera_pose,myDepths);
                
        //draw depth image
        myDepthImage->drawDepths(&myDepths.at(0),myDepthCamera->getZNear(),myDepthCamera->getZFar());
        
        //relax
        usleep(200000);
    }
    
    //delete objects
//     delete myRender;
    delete myDepthCamera;
    delete myDepthImage;
    std::cout << "main(): " << __LINE__ << std::endl; 

    //return
    return 0;
}
