#ifndef sceneRender_H
#define sceneRender_H

//GLUT
#include <GL/glut.h>

//pose3d geometry
#include "pose3d.h"

//faramotics 
#include "window.h"

//asimp 3d model import
#include <assimp/cimport.h>
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/version.h> //assimp version


using namespace std;

/**
 * \brief Default aperture [rad]
 * 
 * default viewing aperture, [degrees]
 * 
*/
static const float APERTURE = 60.0*M_PI/180.0; /**<*/

/**
 * \brief Minimum depth [meters]
 * 
 * default minimum depth when rendering, [meters]
 * 
*/
static const float DEPTH_MIN = 0.3; /**<*/

/**
 * \brief Maximum depth [meters]
 * 
 * default maximum depth when rendering, [meters]
 * 
*/
static const float DEPTH_MAX = 20; /**<*/

/**
 * \brief Identifiers for the hard coded GL models
 * 
 * Identifiers for the hand coded GL models
 * 
*/
enum hardCodedModels {DEBUG_SCENE = 1, SPHERE, FME_BIG_DOOR};

/**
 * \brief GLUT window to render a 3D model
 *
 * CsceneRender implements a GLUT window where a 3D model can be renderized from a "viewPoint" point of view.
 * THis class inherits from Window.
 * Aspect ratio and rendering volume can be all configured through constructor or through a setRenderParameters() function.
 * View point, referenced to the model origin, can be set as:
 *	- a Cpose3d object (libpose_state_time), or
 * 	- six parameters of a 3d position: (x,y,z,heading,pitch,roll), being the three angles the euler angles following the ZYX convention
 * The 3D model can be load passing a filename of a .obj file or inicating the ID of one of the hand-coded openGL models
*/
class CsceneRender : public Window
{
	protected:	
		/** \brief Window metric width [meters]
		 * 
		 * Window width [meters]
		 * 
		*/				
		float widthM;
		
		/** \brief Window metric height [meters]
		 * 
		 * Window height [meters]
		 * 
		*/						
		float heightM; 
		
		/** \brief Horizontal aperture [rad]
		 * 
		 * Horizontal angular aperture of the window [rad]
		 * 
		*/						
		float hAperture;

		/** \brief Vertical aperture [rad]
		 * 
		 * Vertical angular aperture of the window [rad]
		 * 
		*/								
		float vAperture;
		
		/** \brief Near plane distance [meters]
		 * 
		 * Distance to the near plane limiting the viewing volume [meters]
		 * 
		*/								
		float zNear;

		/** \brief Far plane distance [meters]
		 * 
		 * Distance to the far plane limiting the viewing volume [meters]
		 * 
		*/										
		float zFar;

		/** \brief View point 3D position
		 *
		 * Holds the view point position from which the scene is renderized for this window
		 *
		*/
		Cpose3d viewPoint; 
		
		/** \brief GL model list
		 *
		 * GL model list containing the 3d model
		 *
		*/
		GLuint modelList;

            /** \brief Assimp importer
             *
             * Assimp 3D model importer
             *
            */                                  
            Assimp::Importer importer;
            
      protected:
		/** \brief Initializes GL
		 *
		 * GL required initializations for rendering
		 *
		*/								
		void initGL();

            /** \brief draw imported 3d model
             * 
             * Draw imported 3D model using assimp library
             * 
             **/
            void recursiveAssimpRender (const struct aiScene *sc, const struct aiNode* nd);

	public:          
		/** \brief Default constructor
		 *
		 * Default constructor without parameters
		 *
		*/								
		CsceneRender(bool visible=true);

		/** \brief Parameter constructor
		 *
		 * Parameter constructor with rendering parameters
		 *
		*/										
		CsceneRender(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ, bool visible=true);
		
		/** \brief Default destructor
		 *
		 * Default destructor
		 *
		*/										
		~CsceneRender();

		/** \brief Sets rendering parameters 
		 *
		 * Sets rendering parameters:
		 *	- ww: window width in pixels
		 *	- hh: window heigh in pixels
		 *	- hAp: horizontal angular aperture in radians
		 *	- vAp: vertical angular aperture in radians
		 *	- nearZ: minimum ditance limiting the rendering volume
		 *	- farZ: maximum ditance limiting the rendering volume
		 *
		*/										
		void setRenderParameters(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ);
		
		/** \brief Prints renering parameters
		 *
		 * Prints renering parameters to standard output
		 *
		*/										
		void printRenderParameters();
		
		/** \brief Sets view point
		 *
		 * Sets view point with a Cposition3d 
		 *
		*/										
		void setViewPoint(Cpose3d & vP);
		
		/** \brief Sets view point
		 *
		 * Sets view point with a 6DOF position: (x,y,z,heading,pitch,roll)
		 * Position is with resperct to the model frame
		 * The three angles are the euler angles following the ZYX convention.
		 *
		*/										
		void setViewPoint(double cx, double cy, double cz, double ax, double ay, double az, bool rd=inRADIANS);
		
		/** \brief Renders the model from the viewPoint
		 *
		 * Renders the model from the viewPoint
		 *
		*/										
		virtual void render();

            /** \brief Loads a model from a file 
             *
             * Loads a model from a file 
             *
            */                                                                      
            virtual int loadAssimpModel(const string & modelFile, const bool wireFrame = false);

		/** \brief Loads one of the hardcoded models
		 *
		 * Loads one of the hardcoded models identified with modelID
		 *
		*/										
		virtual void loadHardModel(const int modelID);            
};
#endif
