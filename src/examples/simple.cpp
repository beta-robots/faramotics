
//faramotics includes
#include "../src/window.h"
#include "../src/sceneRender.h"

//std
#include <iostream>


int main(int argc, char** argv)
{
//     CsceneRender *view;
    Window *win;
    Pose eye_pose;
    std::string modelFileName;
    
    //init model and initial view point
    modelFileName = "/home/andreu/Desktop/Robotiq_F2_85_LD.stl";
    eye_pose.setPose(200,100,100,M_PI,-0.6,0);
    std::cout << "main(): " << __LINE__ << std::endl; 
        
    //glut initialization
    faramotics::initGLUT(argc, argv);
    std::cout << "main(): " << __LINE__ << std::endl; 
    
    //allocate faramotics object
    win = new Window(1200, 700, true, "faramotics_window");
    //view = new CsceneRender(1200,700,90*M_PI/180.,90*(700./1200.)*(M_PI/180.),10.,600.,"view");
    std::cout << "main(): " << __LINE__ << std::endl; 
    
    //stop 1s
    usleep(1000000);
    
    //return
    delete win; 
    //delete view; 
    std::cout << "main(): " << __LINE__ << std::endl; 
    return 0;
}
