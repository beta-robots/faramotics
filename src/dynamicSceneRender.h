
#ifndef dynamicSceneRender_H
#define dynamicSceneRender_H

//include
#include "sceneRender.h"
#include <vector>

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
		/**
		 * \brief GL model list
		 *
		 * GL model lists containing dynamic objects
		 *
		*/
		GLuint scanHitsList, frameList;


	public:
		/**
		 * \brief Parameter constructor
		 *
		 * Parameter constructor with rendering parameters
		 *
		*/										
		CdynamicSceneRender(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ);
		
		/**
		 * \brief Default destructor
		 *
		 * Default destructor
		 *
		*/										
		~CdynamicSceneRender();
		
		/**
		 * \brief Renders the model from the viewPoint
		 *
		 * Renders the model from the viewPoint
		 *
		*/										
		virtual void render();
		
		/** \brief draws scan points
		 *
		 * Draws scan points
		 *
		*/
		void drawScan(Cpose3d devicePose, vector<float> scan, double aperture, double firstAngle);
		
		/** \brief draws a frame
		 *
		 * Draws a classic RGB (XYZ) frame at position indicated by framePose
		 *
		*/
		void drawPoseAxis(Cpose3d & axis);
		
};
#endif
