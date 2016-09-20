
#ifndef dynamicSceneRender_H
#define dynamicSceneRender_H

//faramotics include
#include "sceneRender.h"

//std include
#include <vector>

/**
 * \brief GLUT window to render a 3D model
 *
 * CdynamicSceneRender implements a GLUT window where a 3D model can be renderized from a "viewPoint" point of view.
 * Window size, aspect ratio and rendering volume can be all configured through constructor or through a setRenderParameters() function.
 * View point, referenced to the model origin, can be set as:
 *	- a Pose object (libposition_3d), or
 * 	- six parameters of a 3d position: (x,y,z,heading,pitch,roll), being the three angles the euler angles following the ZYX convention
 * The 3D model can be load passing a filename of a .obj file or inicating the ID of one of the hand-coded openGL models
*/
class CdynamicSceneRender : public CsceneRender
{
    protected:
        /** \brief GL model list
        *
        * GL model lists containing dynamic objects
        *
        */
        GLuint frameList, frameVectorList, scanHitsList, depthPointsList, cornersList, landmarkList;

    public:
        /** \brief Parameter constructor
        * 
        *
        * Parameter constructor with rendering parameters
        *
        */										
        CdynamicSceneRender(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ, const std::string & label = "faramotics");
		
        /** \brief Default destructor
        *
        * Default destructor
        *
        */										
        virtual ~CdynamicSceneRender();
		
        /** \brief Renders the model from the viewPoint
        * 
        *
        * Renders the model from the viewPoint
        *
        */										
        virtual void render();
            
        /** \brief draws a frame
        *
        * Draws a classic RGB (XYZ) frame at pose indicated by _frame
        *
        */
        void drawPoseAxis(const Pose & _frame);

        /** \brief draws some frames
        *
        * Draws some classic RGB (XYZ) frames at position indicated by framePose
        *
        */
        //void drawPoseAxisVector(const vector<Cpose3d> & axis_vector);
		
        /** \brief draws scan points
        *
        * Draws scan points
        *
        */
        void drawScan(const Pose & _device_pose, const vector<float> & scan, const double aperture, const double firstAngle);
        void drawScan(const Pose & _device_Pose, const vector<double> & scan, const double aperture, const double firstAngle);

        /** \brief corners points
		*
		* Draws corners points
		*
		*/
        void drawCorners(const Pose & _device_pose, const vector<double> & corners, double height=0.1, double radius=0.05);

        /** \brief corners points
		*
		* Draws corners points
		*
		*/
        void drawLandmarks(const vector<double> & landmarks, double height=0.5, double radius=0.02);

        /** \brief draws depth image
        *
        * Draws depth image
        *
        */
        void drawDepthPoints(const Pose & _device_pose, const vector<float> & depths, const unsigned int nPointsH , const unsigned int nPointsV, const double apertureH, const double apertureV, const double firstAngleH, const double firstAngleV);
};
#endif

