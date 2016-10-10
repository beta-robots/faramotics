
#ifndef pose_H
#define pose_H

//Eigen
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

//std
#include <iostream>

/** \brief LookAtParams struct
 *
 * Pose (viewpoints) represented in a useful way for some OpenGL calls. 
 * Parameters are as follows: 
 *  \param eye_ (ex,ey,ez): eye location point. 
 *  \param at_ (ax,ay,az): target point location ('at').
 *  \param up_ (ux,uy,uz): up vector (normalized) of the viewpoint position.
 *
*/
struct LookAtParams
{
    Eigen::Vector3d eye_; //eye location
    Eigen::Vector3d at_; //target point ('at')
    Eigen::Vector3d up_; //up vector
};

/** \brief Class Pose
 * 
 * 3D pose: point and orientation
 * 
 **/
class Pose
{
    protected: 
        Eigen::Vector3d point_;
        Eigen::Quaterniond quaternion_;

    public: 
        //constructors 
        Pose(); 
        Pose(const Eigen::Vector3d & _pt, const Eigen::Quaterniond & _qt); 
        Pose(double _x, double _y, double _z, double _qr, double _qi, double _qj, double _qk);
        Pose(double _x, double _y, double _z, double _yaw, double _pitch, double _roll);
        
        //destructor
        ~Pose(); 
        
        //gets
        const Eigen::Vector3d & getPoint() const; 
        double x() const; 
        double y() const; 
        double z() const; 
        void getRotationMatrix(Eigen::Matrix3d & _rotM) const;
        void getForwardLeft(Eigen::Vector3d & _fwd, Eigen::Vector3d & _lft) const;
        void getLookAt(LookAtParams & _laps) const;
       
        //sets
        void setPoint(const Eigen::Vector3d & _pt);
        void setPoint(double _x, double _y, double _z);
        void setRotationByQuaternion(double _qr, double _qi, double _qj, double _qk);
        void setRotationByQuaternion(const Eigen::Quaterniond & _qt);
        void setRotationByEuler(double _yaw, double _pitch, double _roll);
        void setPose(const Pose & _pose); 
        void setPose(double _x, double _y, double _z, double _qr, double _qi, double _qj, double _qk);
        void setPose(double _x, double _y, double _z, double _yaw, double _pitch, double _roll);
        void setPose(const Eigen::Matrix4d & _H); 
       
        //motion
        void moveForward(double _range);
        void moveUp(double _range); 
        void rotateAboutX(double _angle);
        void rotateAboutY(double _angle); 
        void rotateAboutZ(double _angle);         
        
        //print 
        void print() const;
            
};
#endif
