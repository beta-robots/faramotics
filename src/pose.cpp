#include "pose.h"

Pose::Pose() :
    point_(0.,0.,0.),
    quaternion_(1.,0.,0.,0.)
{
    //
} 

Pose::Pose(const Eigen::Vector3d & _pt, const Eigen:Quaterniond & _ot) :
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

Pose::~Pose()
{
    //
}

const Eigen::Vector3d & Pose::getPoint() const
{
    return point_; 
}

void Pose::getRotationMatrix(Eigen::Matrix3d & _rotM) const
{
    _rotM = quaternion_.toRotationMatrix(); 
}

void Pose::setPoint(const Eigen::Vector3d & _pt)
{
    point << _pt(0), _pt(1), _pt(2);
}

void Pose::setPoint(double _x, double _y, double _z)
{
    point << _x, _y, _z;
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

}

void Pose::setRotationByEuler(double _yaw, double _pitch, double _roll)
{

}

void Pose::print() const
{

}
