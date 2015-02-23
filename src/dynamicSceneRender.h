
#ifndef dynamicSceneRender_H
#define dynamicSceneRender_H

//std include
#include <vector>

//faramotics include
#include "sceneRender.h"

/**
 * \brief GLUT window to render a 3D model
 *
 * CdynamicSceneRender implements a GLUT window where a 3D model can be renderized from a "viewPoint" point of view.
 * Window size, aspect ratio and rendering volume can be all configured through constructor or through a setRenderParameters() function.
 * View point, referenced to the model origin, can be set as:
 *	- a Cposition3d object (libposition_3d), or
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
        GLuint frameList, scanHitsList, depthPointsList;

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
        * Draws a classic RGB (XYZ) frame at position indicated by framePose
        *
        */
        void drawPoseAxis(Cpose3d & axis);            
		
        /** \brief draws scan points
        *
        * Draws scan points
        *
        */
        void drawScan(Cpose3d & devicePose, const vector<float> & scan, const double aperture, const double firstAngle);

        /** \brief draws depth image
        *
        * Draws depth image
        *
        */
        void drawDepthPoints(Cpose3d & devicePose, const vector<float> & depths, const unsigned int nPointsH , const unsigned int nPointsV, const double apertureH, const double apertureV, const double firstAngleH, const double firstAngleV);
};
#endif
