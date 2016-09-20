
//faramotics
#include "../src/pose.h"

//std
#include <iostream>

int main(int argc, char** argv)
{
    Eigen::Vector3d vf, vl;
    Eigen::Matrix3d rm; 
    LookAtParams laps;
    Pose pose(1,2,3,45*M_PI/180.,0*M_PI/180.,0*M_PI/180.);
    
    //get rotation matrix
    pose.getRotationMatrix(rm);
    std::cout << "Rotation matrix: " << std::endl;
    std::cout << rm << std::endl;
    
    //get forward and left vectors
    pose.getForwardLeft(vf, vl);
    std::cout << "Forward vector: " << std::endl;
    std::cout << vf << std::endl;
    std::cout << "Left vector: " << std::endl;
    std::cout << vl << std::endl;
    
    //get look at params
    pose.getLookAt(laps);
    std::cout << "Look at params: " << std::endl;
    std::cout << laps.eye_ << std::endl;
    std::cout << laps.at_ << std::endl;
    std::cout << laps.up_ << std::endl;
    
    //move forward
    pose.moveForward(10.); 
    pose.print(); 
    
    //rotate about X
    pose.rotateAboutX(M_PI/2.); 
    pose.print(); 
    pose.getRotationMatrix(rm);
    std::cout << "Rotation matrix: " << std::endl;
    std::cout << rm << std::endl;
    
    //rotate about Z
    pose.rotateAboutX(-M_PI/2.); //undo    
    pose.rotateAboutZ(-M_PI/4.); 
    pose.print(); 
    pose.getRotationMatrix(rm);
    std::cout << "Rotation matrix: " << std::endl;
    std::cout << rm << std::endl;
    
    //rotate about Y
    pose.rotateAboutY(-M_PI/2.); 
    pose.print(); 
    pose.getRotationMatrix(rm);
    std::cout << "Rotation matrix: " << std::endl;
    std::cout << rm << std::endl;
    
    return 0;
}
