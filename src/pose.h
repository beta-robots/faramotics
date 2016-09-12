
#ifndef pose_H
#define pose_H

//Eigen
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

//std
#include <iostream>

class Pose
{
    protected: 
        Eigen::Vector3d point_;
        Eigen:Quaterniond quaternion_;

    public: 
        //constructors 
        Pose(); 
        Pose(const Eigen::Vector3d & _pt, const Eigen:Quaterniond & _qt); 
        Pose(double _x, double _y, double _z, double _qr, double _qi, double _qj, double _qk);
        
        //destructor
        ~Pose(); 
        
       //gets
       const Eigen::Vector3d & getPoint() const;        
       void getRotationMatrix(Eigen::Matrix3d & _rotM) const;
       
       //sets
       void setPoint(const Eigen::Vector3d & _pt);
       void setPoint(double _x, double _y, double _z);
       void setRotationByQuaternion(double _qr, double _qi, double _qj, double _qk);
       void setRotationByQuaternion(const Eigen::Quaterniond & _qt);
       void setRotationByEuler(double _yaw, double _pitch, double _roll); 
       
       //print 
       void print() const;
        
}
#endif
