
//C includes for sleep()
#include "unistd.h"

//faramotics includes
#include "../src/window.h"
#include "../src/sceneRender.h"
#include "../src/dynamicSceneRender.h"
#include "../src/rangeSector.h"
#include "../src/rangeScan2D.h"
#include "../src/rangeImage.h"


using namespace std;

void motion(unsigned int ii, Cpose3d & pose)
{
    if (ii<=40)
    {
        //pose.rt.setEuler( pose.rt.head()-2*M_PI/180., pose.rt.pitch(), pose.rt.roll() );
        pose.pt(0) = pose.pt(0) + 0.1;
    }
    if ( (ii>40) && (ii<=80) )
    {
        pose.pt(0) = pose.pt(0) + 0.1;
        pose.rt.setEuler(pose.rt.head(), pose.rt.pitch(), pose.rt.roll() + 0.01);
    }
    if ( (ii>80) && (ii<=120) )
    {
        pose.pt(0) = pose.pt(0) + 0.1;
        pose.rt.setEuler(pose.rt.head(), pose.rt.pitch(), pose.rt.roll() - 0.015);
    }
    if ( (ii>120) && (ii<=170) )
    {
        pose.rt.setEuler( pose.rt.head()+1.8*M_PI/180., pose.rt.pitch(), pose.rt.roll() );
        pose.moveForward(0.2);
    }
    if ( (ii>170) && (ii<=220) )
    {
        pose.rt.setEuler( pose.rt.head()-1.8*M_PI/180., pose.rt.pitch(), pose.rt.roll()-0.05*M_PI/180. );
    }
    if ( (ii>220) && (ii<=310) ) 
    {
        pose.pt(0) = pose.pt(0) + 0.1;
    }
    if ( (ii>310) && (ii<=487) ) 
    {
        pose.rt.setEuler( pose.rt.head()-1.*M_PI/180., pose.rt.pitch(), pose.rt.roll()+0.1*M_PI/180. );
        pose.moveForward(0.1);
    }
    if ( (ii>487) && (ii<=582) ) 
    {
        pose.moveForward(0.2);
    }
    if ( (ii>582) && (ii<=700) ) 
    {
        pose.pt(2) = pose.pt(2) + 0.001;
        pose.rt.setEuler( pose.rt.head()-1.*M_PI/180., pose.rt.pitch(), pose.rt.roll());
        pose.moveForward(0.1);
    }
    
}

int main(int argc, char** argv)
{
    CdynamicSceneRender *myRender;
    CrangeScan2D *myScanner; 
    CrangeImage *myDepthCamera;
    Window *myDepthImage;    
    Cpose3d viewPoint;
    Cpose3d devicePose;
    vector<float> myScan;
    vector<float> myDepths;
    string modelFileName;
    unsigned int ii;
    
    //user entries: model and initial view point
    modelFileName = "../models/campusNordUPC.obj";
    devicePose.setPose(2,8,0.2,0,0,0);
    
    //glut initialization
    glutInit(&argc, argv);
    
    //create a viewer for the 3D model and scan points
    myRender = new CdynamicSceneRender(1200,700,90*M_PI/180,90*700.0*M_PI/(1200.0*180.0),0.2,100);
    myRender->loadAssimpModel(modelFileName,true); //with wireframe
    //myRender->loadHardModel(DEBUG_SCENE);
    //myRender->loadHardModel(SPHERE);
    
    //create scanner and load 3D model
    myScanner = new CrangeScan2D(HOKUYO_UTM30LX_180DEG);//or LEUZE_RS4
    myScanner->loadAssimpModel(modelFileName);
    //myScanner->loadHardModel(DEBUG_SCENE);
    //myScanner->loadHardModel(SPHERE);
    
    //create depth camera and load 3D model
    myDepthCamera = new CrangeImage(KINECT); // or SR4000
    myDepthCamera->loadAssimpModel(modelFileName);
    
    //Window to visualize depth image
    myDepthImage = new Window(myDepthCamera->getNumHorizontalPoints(), myDepthCamera->getNumVerticalPoints(), true);

    //main loop after a pause
    sleep(1);
    for (ii=0; ii<1000; ii++)
    {
        //moves the device position
        motion(ii, devicePose);
        
        //compute scan
        myScan.clear();
        myScanner->computeScan(devicePose,myScan);
        
        //compute depth image
        myDepths.clear();
        myDepthCamera->depthImage(devicePose,myDepths);

        //draws the device frame, scan hits and depth image
        myRender->drawPoseAxis(devicePose);
        myRender->drawScan(devicePose,myScan,180.*M_PI/180.,90.*M_PI/180.); //draw scan with leuze aperture params
        myDepthImage->drawDepths(&myDepths.at(0),0.7,5.);
                    
        //locate visualization view point, somewhere behind the device
        viewPoint.setPose(devicePose);
        viewPoint.rt.setEuler( viewPoint.rt.head(), viewPoint.rt.pitch()+20.*M_PI/180., viewPoint.rt.roll() );
        viewPoint.moveForward(-7);
        
        //Set view point and render the scene
        myRender->setViewPoint(viewPoint);
        myRender->render();

        //sleep to have time to see something
        //usleep(20000);
    }
    
    //delete objects
    delete myRender;
    delete myScanner;

    return 0;
}
