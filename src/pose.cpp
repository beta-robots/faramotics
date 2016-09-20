#include "pose.h"

Pose::Pose() :
    point_(0.,0.,0.),
    quaternion_(1.,0.,0.,0.)
{
    //
} 

Pose::Pose(const Eigen::Vector3d & _pt, const Eigen::Quaterniond & _qt) :
    point_(_pt),
    quaternion_(_qt)
{
    //
}

Pose::Pose(double _x, double _y, double _z, double _qr, double _qi, double _qj, double _qk) :
    point_(_x,_y,_z),
    quaternion_(_qr,_qi,_qj,_qk)
{
    //
}

Pose::Pose(double _x, double _y, double _z, double _yaw, double _pitch, double _roll) :
    point_(_x,_y,_z)
{
    setRotationByEuler(_yaw,_pitch,_roll); 
}

Pose::~Pose()
{
    //
}

const Eigen::Vector3d & Pose::getPoint() const
{
    return point_; 
}

double Pose::x() const
{
    return point_(0);
}

double Pose::y() const
{
    return point_(1);
}

double Pose::z() const
{
    return point_(2);
}


void Pose::getRotationMatrix(Eigen::Matrix3d & _rotM) const
{
    _rotM = quaternion_.toRotationMatrix(); 
}

void Pose::getForwardLeft(Eigen::Vector3d & _fwd, Eigen::Vector3d & _lft) const
{
    Eigen::Matrix3d rM; 
    
    rM = quaternion_.toRotationMatrix(); 
    _fwd << rM(0,0),rM(1,0),rM(2,0);
    _lft << rM(0,1),rM(1,1),rM(2,1);            
}

void Pose::getLookAt(LookAtParams & _laps) const
{
    Eigen::Matrix3d rM; 
    
    //eye location
    _laps.eye_ << point_(0),point_(1),point_(2);
    
    //at point. Get forward axis (X) of this pose and prolongate it 100 units away
    rM = quaternion_.toRotationMatrix(); 
    _laps.at_ << rM(0,0)*100., rM(1,0)*100., rM(2,0)*100.;
    
    //up vector.  Z axis
    _laps.up_ << rM(0,2),rM(1,2),rM(2,2);    
}

void Pose::setPoint(const Eigen::Vector3d & _pt)
{
    point_ << _pt(0), _pt(1), _pt(2);
}

void Pose::setPoint(double _x, double _y, double _z)
{
    point_ << _x, _y, _z;
}

void Pose::setRotationByQuaternion(const Eigen::Quaterniond & _qt)
{
    quaternion_.w() = _qt.w(); 
    quaternion_.x() = _qt.x(); 
    quaternion_.y() = _qt.y(); 
    quaternion_.z() = _qt.z(); 
}

void Pose::setRotationByQuaternion(double _qr, double _qi, double _qj, double _qk)
{
    quaternion_.w() = _qr; 
    quaternion_.x() = _qi; 
    quaternion_.y() = _qj; 
    quaternion_.z() = _qk;    
}

void Pose::setRotationByEuler(double _yaw, double _pitch, double _roll)
{
    Eigen::Matrix3d matY, matP, matR;
    
    matY << cos(_yaw), -sin(_yaw), 0., 
            sin(_yaw),  cos(_yaw), 0., 
            0.,         0.,        1.;
                            
    matP << cos(_pitch), 0., sin(_pitch), 
            0.,          1., 0., 
           -sin(_pitch), 0., cos(_pitch);
                              
    matR <<  1., 0.,          0., 
            0., cos(_roll), -sin(_roll), 
            0., sin(_roll),  cos(_roll);
                                
    quaternion_ = matY*matP*matR; //final orientation is the composition of these three rotations
}

void Pose::setPose(const Pose & _pose)
{
    point_ << _pose.point_(0), _pose.point_(1), _pose.point_(2);
    quaternion_.w() = _pose.quaternion_.w(); 
    quaternion_.x() = _pose.quaternion_.x(); 
    quaternion_.y() = _pose.quaternion_.y();
    quaternion_.z() = _pose.quaternion_.z(); 
}

void Pose::setPose(double _x, double _y, double _z, double _qr, double _qi, double _qj, double _qk)
{
    setPoint(_x,_y,_z);
    setRotationByQuaternion(_qr,_qi,_qj,_qk); 
}

void Pose::setPose(double _x, double _y, double _z, double _yaw, double _pitch, double _roll)
{
    setPoint(_x,_y,_z);
    setRotationByEuler(_yaw,_pitch,_roll); 
}

void Pose::moveForward(double _range)
{
    Eigen::Matrix3d rM; 
    
    rM = quaternion_.toRotationMatrix(); 
    point_ << point_(0) + rM(0,0)*_range, point_(1) + rM(1,0)*_range,point_(2) + rM(2,0)*_range;
}

void Pose::rotateAboutX(double _angle)
{
    //build a quaternion rotating _angle about X
    Eigen::Quaterniond qt; 
    qt.w() = cos(_angle/2.); 
    qt.x() = sin(_angle/2.);
    qt.y() = 0.;
    qt.z() = 0.;
    
    //update quaternion_ class member with the rotation
    quaternion_ = quaternion_ * qt; 
}

void Pose::rotateAboutY(double _angle)
{
    //build a quaternion rotating _angle about X
    Eigen::Quaterniond qt; 
    qt.w() = cos(_angle/2.); 
    qt.x() = 0.;
    qt.y() = sin(_angle/2.);
    qt.z() = 0.;
    
    //update quaternion_ class member with the rotation
    quaternion_ = quaternion_ * qt; 
}

void Pose::rotateAboutZ(double _angle)
{
    //build a quaternion rotating _angle about X
    Eigen::Quaterniond qt; 
    qt.w() = cos(_angle/2.); 
    qt.x() = 0.;
    qt.y() = 0.;
    qt.z() = sin(_angle/2.);
    
    //update quaternion_ class member with the rotation
    quaternion_ = quaternion_ * qt; 
}


void Pose::print() const
{
    std::cout << point_.transpose() << std::endl;  
    std::cout << quaternion_.w() << "," << 
                 quaternion_.x() << "," << 
                 quaternion_.y() << "," << 
                 quaternion_.z() << std::endl; 
}
