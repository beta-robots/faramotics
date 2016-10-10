
//faramotics
#include "../src/pose.h"

//std
#include <iostream>

int main(int argc, char** argv)
{
    Eigen::Vector3d vf, vl;
    Eigen::Matrix3d rm; 
    LookAtParams laps;
    Pose pose1(1,2,3,45*M_PI/180.,0*M_PI/180.,0*M_PI/180.);
    Pose pose2; 
    
    //get rotation matrix
    pose1.getRotationMatrix(rm);
    std::cout << "Rotation matrix: " << std::endl;
    std::cout << rm << std::endl;
    
    //get forward and left vectors
    pose1.getForwardLeft(vf, vl);
    std::cout << "Forward vector: " << std::endl;
    std::cout << vf << std::endl;
    std::cout << "Left vector: " << std::endl;
    std::cout << vl << std::endl;
    
    //get look at params
    pose1.getLookAt(laps);
    std::cout << "Look at params: " << std::endl;
    std::cout << laps.eye_ << std::endl;
    std::cout << laps.at_ << std::endl;
    std::cout << laps.up_ << std::endl;
    
    //move forward
    pose1.moveForward(10.); 
    pose1.print(); 
    
    //rotate about X
    pose1.rotateAboutX(M_PI/2.); 
    pose1.print(); 
    pose1.getRotationMatrix(rm);
    std::cout << "Rotation matrix: " << std::endl;
    std::cout << rm << std::endl;
    
    //rotate about Z
    pose1.rotateAboutX(-M_PI/2.); //undo    
    pose1.rotateAboutZ(-M_PI/4.); 
    pose1.print(); 
    pose1.getRotationMatrix(rm);
    std::cout << "Rotation matrix: " << std::endl;
    std::cout << rm << std::endl;
    
    //rotate about Y
    pose1.rotateAboutY(-M_PI/2.); 
    pose1.print(); 
    pose1.getRotationMatrix(rm);
    std::cout << "Rotation matrix: " << std::endl;
    std::cout << rm << std::endl;
    
    //copy
    pose2.setPose(pose1); 
    
    return 0;
}
