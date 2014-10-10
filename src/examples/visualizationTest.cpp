
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

int main(int argc, char** argv)
{
    CdynamicSceneRender *myRender;
    CrangeScan2D *myScanner; 
    CrangeImage *myDepthCamera;
    Window *myDepthValues;    
    Cpose3d viewPoint;
    Cpose3d devicePose;
    vector<float> myScan;
    vector<float> myDepths;
    string modelFileName;
    unsigned int ii;
    
    //user entries: model and initial view point
    modelFileName = "../models/campusNordUPC.obj";
    //modelFileName = "../models/secretarysDesk.obj";
    //modelFileName = "../models/curve_garden_bridge.3ds";
    devicePose.setPose(2,8,0.2,0,0,0);
    
    //glut initialization
    glutInit(&argc, argv);
    
    //create a viewer for the 3D model and scan points
    myRender = new CdynamicSceneRender(1200,700,90*M_PI/180,90*700.0*M_PI/(1200.0*180.0),0.2,100);
    myRender->loadAssimpModel(modelFileName,true); //with wireframe
//       myRender->loadHardModel(DEBUG_SCENE);
    //myRender->loadHardModel(SPHERE);
    
    //create scanner and load 3D model
    myScanner = new CrangeScan2D(LEUZE_RS4);//or HOKUYO_UTM30LX_180DEG
    myScanner->loadAssimpModel(modelFileName);
    //myScanner->loadHardModel(DEBUG_SCENE);
    //myScanner->loadHardModel(SPHERE);
    
    //create depth camera and load 3D model
    //myDepthCamera = new CrangeImage(KINECT);
    myDepthCamera = new CrangeImage(SR4000);
    myDepthCamera->loadAssimpModel(modelFileName);
    
    //Window to visualize depth image
    myDepthValues = new Window(176,144,true);

    //main loop after a pause
    sleep(1);
    for (ii=0; ii<1000; ii++)
    {
        //moves the device position
        if (ii<=40)
        {
                //devicePose.rt.setEuler( devicePose.rt.head()-2*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll() );
                devicePose.pt(0) = devicePose.pt(0) + 0.1;
        }
        if ( (ii>40) && (ii<=80) )
        {
                devicePose.pt(0) = devicePose.pt(0) + 0.1;
                devicePose.rt.setEuler(devicePose.rt.head(), devicePose.rt.pitch(), devicePose.rt.roll() + 0.01);
        }
        if ( (ii>80) && (ii<=120) )
        {
                devicePose.pt(0) = devicePose.pt(0) + 0.1;
                devicePose.rt.setEuler(devicePose.rt.head(), devicePose.rt.pitch(), devicePose.rt.roll() - 0.01);
        }
        if ( (ii>120) && (ii<=170) )
        {
                devicePose.rt.setEuler( devicePose.rt.head()+1.8*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll() );
                devicePose.moveForward(0.2);
        }
        if ( (ii>170) && (ii<=220) )
        {
                devicePose.rt.setEuler( devicePose.rt.head()-1.8*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll()-0.05*M_PI/180. );
        }
        if ( (ii>220) && (ii<=300) ) 
        {
                devicePose.pt(0) = devicePose.pt(0) + 0.1;
        }
        if ( (ii>300) && (ii<=500) ) 
        {
                devicePose.rt.setEuler( devicePose.rt.head()-1.*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll()+0.1*M_PI/180. );
                devicePose.moveForward(0.1);
        }
        if ( (ii>500) && (ii<=1000) ) 
        {
                devicePose.rt.setEuler( devicePose.rt.head()+1.*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll()-0.03*M_PI/180. );
                devicePose.moveForward(0.);
        }
        
        //compute scan
        myScan.clear();
        myScanner->computeScan(devicePose,myScan);
        
        //compute depth image
        myDepths.clear();
        myDepthCamera->depthImage(devicePose,myDepths);

        //draws the scene, scan hits and depths
        myRender->drawPoseAxis(devicePose);
        myRender->drawScan(devicePose,myScan,190.*M_PI/180.,95.*M_PI/180.); //draw scan with leuze aperture params
        //myRender->drawDepthPoints(devicePose, myDepths, 640, 480, 57.*M_PI/180., 43.*M_PI/180., (57./2.)*M_PI/180., (43./2.)*M_PI/180.);
        //myRender->drawDepthPoints(devicePose, myDepths, 176, 144, 43.6*M_PI/180., 34.6*M_PI/180., (43.6/2.)*M_PI/180., (34.6/2.)*M_PI/180.);
        myDepthValues->drawDepths(&myDepths.at(0),0.3,5.);
                    
        //locate visualization view point, somewhere behind the device
        viewPoint.setPose(devicePose);
        viewPoint.rt.setEuler( viewPoint.rt.head(), viewPoint.rt.pitch()+20.*M_PI/180., viewPoint.rt.roll() );
        viewPoint.moveForward(-7);
        
        //Set view point and render the scene
        myRender->setViewPoint(viewPoint);
        myRender->render();

        //sleep to have time to see something
        usleep(50000);
    }
    
    //delete objects
    delete myRender;
    delete myScanner;

    return 0;
}
